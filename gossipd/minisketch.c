#include "config.h"
#include <bitcoin/short_channel_id.h>
#include <common/status.h>
#include <common/type_to_string.h>
#include <external/minisketch/include/minisketch.h>
#include <gossipd/minisketch.h>
#include <gossipd/routing.h>
//#include <wire/peer_wiregen.h> //FIXME: do I still use this?


#if EXPERIMENTAL_FEATURES
#ifndef MINISKETCH_CAPACITY
#define MINISKETCH_CAPACITY 160
#endif

void init_minisketch(struct routing_state *rstate)
{
        rstate->minisketch = minisketch_create(64, 0, MINISKETCH_CAPACITY);
        rstate->sketch_entries = 0;
        rstate->sketch_cannounce_entries = 0;
        rstate->sketch_nannounce_entries = 0;
        rstate->sketch_cupdate_entries = 0;
}
/* registered through tal_add_destructor via destroy_routing_state */
void destroy_minisketch(struct routing_state *rstate)
{
        minisketch_destroy(rstate->minisketch);
}

static void apply_bits(u64 *val, size_t *bitoff, u64 v, size_t bits)
{
	/* v must not be greater than bits long */
	assert((v & ~((1ULL << bits)-1)) == 0);
	*val |= (v << *bitoff);
	*bitoff += bits;
}

/* use this to find canonical channel to describe node_announcement in sketch */
struct chan *node_least_channel(struct node *node)
{
	struct chan_map_iter i;
	struct chan *c;

	for (c = first_chan(node, &i); c; c = next_chan(node, &i)) {
		if (is_chan_public(c))
			return c;
	}
	return NULL;
}

//static u64 sketch_encode(const struct chan *chan)
/* encode gossip for minisketch inclusion */
u64 minisketch_encode(u8 type,
                  struct short_channel_id short_channel_id, u8 side,
                  u32 timestamp)
{
        u64 val = 0;
        size_t bitoff = 0;
        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
         * Each minisketch gossip entry is encoded in 64 bits in the following
         * manner:
         * 1. The two lowest bits, N0, specify gossip type:
         *   * 0 for channel_announcement
         *   * 1 for channel_update
         *   * 2 for node_announcement
         * 2. The next lowest bit, N1, specifies the channel side of the
         *    gossip.
         *   * In the case of a channel announcement, bit N1 is ignored.
         *   * In the case of a channel update, N1 refers to the `direction`
         *     bit of the channel update message.
         *   * In the case of a node announcement, N1=0 indicates that the
         *     entry refers to node_id_1 of the described channel. N1=1 refers
         *     to node_id_2.
         * 3. The next lowest N2=24 bits encode the block height of the
         *    channel's funding transaction.
         * 4. The next lowest N3=15 bits encode the transaction index of the
         *    funding transaction.
         * 5. The next lowest N4=10 bits are the output index of the funds
         *    consumed by the funding transaction.
         * 6. The remaining N5=12 bits encode the gossip timestamp in the
         *    form:
         *    timestamp % ((2^N5)-1)
         */
        size_t n0 = 2;
        size_t n1 = 1;
        size_t n2 = 24;
        size_t n3 = 15;
        size_t n4 = 10;
        size_t n5 = 12;
        apply_bits(&val, &bitoff, type, n0);
        apply_bits(&val, &bitoff, side, n1);
        apply_bits(&val, &bitoff, short_channel_id_blocknum(&short_channel_id), n2);
        apply_bits(&val, &bitoff, short_channel_id_txnum(&short_channel_id), n3);
        apply_bits(&val, &bitoff, short_channel_id_outnum(&short_channel_id), n4);
        apply_bits(&val, &bitoff, timestamp % ((1 << n5) - 1), n5);
        return val;
}

u64 minisketch_encode_cupdate(struct chan *chan, u8 direction,u32 timestamp){
        return minisketch_encode(1, chan->scid, direction, timestamp);
}

u64 minisketch_encode_cannounce(struct chan *chan){
        /* No timestamp associate with channel_announce. */
        return minisketch_encode(0, chan->scid, 0, 0xFFFFFFFF);
        //return minisketch_encode(0, chan->scid, 0, timestamp);
}

u64 minisketch_encode_nannounce(struct chan *chan,u32 timestamp, u8 side){
        return minisketch_encode(2, chan->scid, side, timestamp);
}

u8 minisketch_decode_type(u64 minisketch_entry){
        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
         * Each minisketch gossip entry is encoded in 64 bits in the following
         * manner:
         * 1. The two lowest bits, N0, specify gossip type:
         *   * 0 for channel_announcement
         *   * 1 for channel_update
         *   * 2 for node_announcement
         */
        u8 minisketch_gossip_type;
        minisketch_gossip_type = minisketch_entry & 0x3;
        /*FIXME: wire type may be more useful here. */
        switch(minisketch_gossip_type){
        case 0:
                return 0;
        case 1:
                return 1;
        case 2:
                return 2;
        /*FIXME:return something sensical*/
        }
        return 255;
}

void init_minisketch_channels(struct chan *chan)
{
        /* Unused sketch entries. Minisketch entries may not be 0. */
        chan->minisketch_channel_announcement = 0;
        chan->minisketch_channel_update[0] = 0;
        chan->minisketch_channel_update[1] = 0;
}

void init_minisketch_node(struct node *node)
{
        /* Unused sketch entries. Minisketch entries may not be 0. */
        node->minisketch_node_announcement = 0;
}

bool minisketch_add_to_sketch(struct routing_state *rstate, u64 minisketch_entry)
{
        /* Uninitialized sketch entries may not be added. */
        if (!minisketch_entry)
                return false;

        switch (minisketch_decode_type(minisketch_entry)){
        case 0:
                rstate->sketch_cannounce_entries++;
                break;
        case 1:
                rstate->sketch_cupdate_entries++;
                break;
        case 2:
                rstate->sketch_nannounce_entries++;
                break;
        case 255:
                /* invalid type in sketch encoding*/
                return false;
        }
        minisketch_add_uint64(rstate->minisketch, minisketch_entry);
        rstate->sketch_entries++;
        return true;
}

bool minisketch_sub_from_sketch(struct routing_state *rstate, u64 minisketch_entry)
{
        /* Uninitialized sketch entries may not be added. */
        if (!minisketch_entry)
                return false;

        switch (minisketch_decode_type(minisketch_entry)){
        case 0:
                rstate->sketch_cannounce_entries--;
                break;
        case 1:
                rstate->sketch_cupdate_entries--;
                break;
        case 2:
                rstate->sketch_nannounce_entries--;
                break;
        case 255:
                /* invalid type in sketch encoding*/
                return false;
        }
        minisketch_add_uint64(rstate->minisketch, minisketch_entry);
        rstate->sketch_entries--;
        return true;
}

/* add cannounce to a chan and add sketch entry to minisketch*/
bool minisketch_add_cannounce(struct routing_state *rstate,
                              struct chan *chan)
{
        /*call this alongside routing_add_channel_announcement()*/
        u64 ms;
        ms = minisketch_encode_cannounce(chan);
        /* Should not have a previous channel announcement. But it would mess
        up our entire minisketch if it's not handled.*/
        if (chan->minisketch_channel_announcement){
                minisketch_sub_from_sketch(rstate, chan->minisketch_channel_announcement);
        }
        chan->minisketch_channel_announcement = ms;
        /* add entry to minisketch */
        minisketch_add_to_sketch(rstate, ms);
        return true;
}
void entry_decode(u64 ms_entry){
        u8 type = minisketch_decode_type(ms_entry);
        struct short_channel_id scid;
        u8 side;
        u32 timestamp;
        const char * hex;
        switch (type){
        case 0:
                minisketch_decode_cannounce(ms_entry, &scid, &timestamp);
                hex = type_to_string(NULL, struct short_channel_id, &scid);
                status_debug("minisketch: cannounce entry %lX decodes to SCID %s, at ts %u.",
                             ms_entry,
                             hex,
                             timestamp);
                tal_free(hex);
                break;
        case 1:
                minisketch_decode_cupdate(ms_entry, &scid, &side, &timestamp);
                hex = type_to_string(NULL, struct short_channel_id, &scid);
                status_debug("minisketch: cupdate entry %lX decodes to SCID %s/%u, at ts %u.",
                             ms_entry,
                             hex,
                             side,
                             timestamp);
                tal_free(hex);
                break;
        case 2:
                minisketch_decode_nannounce(ms_entry, &scid, &side, &timestamp);
                hex = type_to_string(NULL, struct short_channel_id, &scid);
                status_debug("minisketch: nannounce entry %lX decodes to SCID %s/%u, at ts %u.",
                             ms_entry,
                             hex,
                             side,
                             timestamp);
                tal_free(hex);
                break;
        }
}
bool minisketch_handle_cannounce(struct routing_state *rstate,
                                 struct chan *chan,
                                 u32 timestamp){
        /* called from add_channel_announce_to_broadcast (routing) */
        u64 ms;
        ms = minisketch_encode_cannounce(chan);
        /* Avoid unintentionally duplicating the sketch entry */
        if(chan->minisketch_channel_announcement){
                if(!minisketch_sub_from_sketch(rstate, chan->minisketch_channel_announcement))
                        return false;
        }
        if (!minisketch_add_to_sketch(rstate, ms))
                return false;
        chan->minisketch_channel_announcement = ms;
        const char * hex = type_to_string(NULL, struct short_channel_id, &chan->scid);
        status_debug("minisketch: cannounce sketch entry %lX added for chan %s",ms,hex);
        tal_free(hex);
        //FIXME: just a sanity check for now.
        entry_decode(ms);
        return true;
}
bool minisketch_handle_cupdate(struct routing_state *rstate,
                            struct chan *chan,
                            u8 side,
                            u32 timestamp)
{
        /* called from routing_add_channel_update */
        u64 ms;
        ms = minisketch_encode_cupdate(chan, side, timestamp);
        /* remove old entry from minisketch if it has one*/
        if (chan->minisketch_channel_update[side]){
                if(!minisketch_sub_from_sketch(rstate, chan->minisketch_channel_update[side]))
                        return false;
        }
        if (!minisketch_add_to_sketch(rstate, ms))
                return false;
        chan->minisketch_channel_update[side] = ms;
        //char * hex = tal_hexstr(NULL,(void *)&chan->scid.u64,sizeof(chan->scid));
        /* FIXME: May use tmpctx here */
        const char * hex = type_to_string(NULL, struct short_channel_id, &chan->scid);
        status_debug("minisketch: cupdate sketch entry %lX added for chan %s/%u", ms, hex,
                     side);
        tal_free(hex);
        //FIXME: just a sanity check for now.
        entry_decode(ms);
        return true;
}

bool minisketch_handle_nannounce(struct routing_state *rstate,
                                 const struct node_id *node_id,
                                 u32 timestamp)
{
        /* called from handle_node_announcement */
        struct node *node;
        node = get_node(rstate, node_id);
        /* get node's first channel for cannonical identification.
           Must also be updated when channel is closed.*/
        struct chan_map_iter i;
	struct chan *c, *low_scid_chan;
        c = low_scid_chan = first_chan(node, &i);
        for (; c ; c = next_chan(node, &i)) {
                if (c->scid.u64 < low_scid_chan->scid.u64){
                        low_scid_chan = c;
                }
        }
        if (!low_scid_chan)
                return false;
        u8 side;
        if (node == low_scid_chan->nodes[0])
                side = 0;
        else if (node == low_scid_chan->nodes[1])
                side = 1;
        else
                return false;
        u64 ms;
        ms = minisketch_encode_nannounce(low_scid_chan, timestamp, side);

        /* remove old entry from minisketch if it has one*/
        if (node->minisketch_node_announcement){
                if(!minisketch_sub_from_sketch(rstate,node->minisketch_node_announcement))
                        return false;
        }
        node->minisketch_node_announcement = ms;
        if(!minisketch_add_to_sketch(rstate, ms))
                return false;
        const char * scidhex = type_to_string(NULL, struct short_channel_id, &low_scid_chan->scid);
        const char * nodehex = type_to_string(NULL, struct node_id, node_id);
        status_debug("minisketch: nannounce sketch entry %lX added for node %s referencing chan %s/%u",
                     ms, nodehex, scidhex, side);
        tal_free(scidhex);
        tal_free(nodehex);
        //FIXME: just a sanity check for now.
        entry_decode(ms);
        return true;
}
static u64 pull_bits(u64 *val, size_t *bitoff, size_t bits)
{
	u64 ret;

	ret = *val;
	ret &= ((u64)1 << bits) - 1;
	*val >>= bits;
	*bitoff += bits;
	return ret;
}
/* decode a channel announcement minisketch entry into a short_channel_id */
bool minisketch_decode_cannounce(u64 ms_entry,
                                 struct short_channel_id *scid,
                                 u32 *timestamp){
        size_t bitoff = 0;
        u32 scid_blk, scid_tx, scid_out;
        bool status;
        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
         * Each minisketch gossip entry is encoded in 64 bits in the following
         * manner:
         * 1. The two lowest bits, N0, specify gossip type:
         *   * 0 for channel_announcement
         *   * 1 for channel_update
         *   * 2 for node_announcement
         */
        assert(pull_bits(&ms_entry, &bitoff, 2) == 0);

        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
         * 2. The next lowest bit, N1, specifies the channel side of the
         *    gossip.
         *   * In the case of a channel announcement, bit N1 is ignored.
         */
        pull_bits(&ms_entry, &bitoff, 1);

        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
         * 3. The next lowest N2=24 bits encode the block height of the
         *    channel's funding transaction.
         */
        scid_blk = pull_bits(&ms_entry, &bitoff, 24);

        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
         * 4. The next lowest N3=15 bits encode the transaction index of the
         *    funding transaction.
         */
        scid_tx = pull_bits(&ms_entry, &bitoff, 15);

         /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
         * 5. The next lowest N4=10 bits are the output index of the funds
         *    consumed by the funding transaction.
         */
        scid_out = pull_bits(&ms_entry, &bitoff, 10);
         /* Ignore the timestamp section for cannounce*/
         /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
         * 6. The remaining N5=12 bits encode the gossip timestamp in the
         *    form:
         *    timestamp % ((2^N5)-1)
         */
        status = mk_short_channel_id(scid, scid_blk, scid_tx, scid_out);
        return status;
}
/* decode a channel update minisketch entry into a short_channel_id
 * Also find relevant gossip entry here? */
bool minisketch_decode_cupdate(u64 ms_entry,
                               struct short_channel_id *scid,
                               u8 *side,
                               u32 *timestamp){
        bool status;
        size_t bitoff = 0;
        u32 scid_blk, scid_tx, scid_out;
        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
        * Each minisketch gossip entry is encoded in 64 bits in the following
        * manner:
        * 1. The two lowest bits, N0, specify gossip type:
        *   * 0 for channel_announcement
        *   * 1 for channel_update
        *   * 2 for node_announcement
        */
        assert(pull_bits(&ms_entry, &bitoff, 2) == 1);

        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
        * 2. The next lowest bit, N1, specifies the channel side of the
        *    gossip.
        *   * In the case of a channel announcement, bit N1 is ignored.
        */
        *side = (u8) pull_bits(&ms_entry, &bitoff, 1);

        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
        * 3. The next lowest N2=24 bits encode the block height of the
        *    channel's funding transaction.
        */
        scid_blk = pull_bits(&ms_entry, &bitoff, 24);

        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
        * 4. The next lowest N3=15 bits encode the transaction index of the
        *    funding transaction.
        */
        scid_tx = pull_bits(&ms_entry, &bitoff, 15);

        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
        * 5. The next lowest N4=10 bits are the output index of the funds
        *    consumed by the funding transaction.
        */
        scid_out = pull_bits(&ms_entry, &bitoff, 10);
        /* Ignore the timestamp section for cannounce*/
        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
        * 6. The remaining N5=12 bits encode the gossip timestamp in the
        *    form:
        *    timestamp % ((2^N5)-1)
        */
        status = (bool) mk_short_channel_id(scid, scid_blk, scid_tx, scid_out);
        /* FIXME: use block height */
        *timestamp = pull_bits(&ms_entry, &bitoff, 12);
        return status;
}
bool minisketch_decode_nannounce(u64 ms_entry,
                               struct short_channel_id *scid,
                               u8 *side,
                               u32 *timestamp){
        bool status;
        size_t bitoff = 0;
        u32 scid_blk, scid_tx, scid_out;
        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
        * Each minisketch gossip entry is encoded in 64 bits in the following
        * manner:
        * 1. The two lowest bits, N0, specify gossip type:
        *   * 0 for channel_announcement
        *   * 1 for channel_update
        *   * 2 for node_announcement
        */
        assert(pull_bits(&ms_entry, &bitoff, 2) == 2);

        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
        * 2. The next lowest bit, N1, specifies the channel side of the
        *    gossip.
        *   * In the case of a channel announcement, bit N1 is ignored.
        */
        *side = (u8) pull_bits(&ms_entry, &bitoff, 1);

        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
        * 3. The next lowest N2=24 bits encode the block height of the
        *    channel's funding transaction.
        */
        scid_blk = pull_bits(&ms_entry, &bitoff, 24);

        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
        * 4. The next lowest N3=15 bits encode the transaction index of the
        *    funding transaction.
        */
        scid_tx = pull_bits(&ms_entry, &bitoff, 15);

        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
        * 5. The next lowest N4=10 bits are the output index of the funds
        *    consumed by the funding transaction.
        */
        scid_out = pull_bits(&ms_entry, &bitoff, 10);
        /* Ignore the timestamp section for cannounce*/
        /* BOLT-a8fb7f221228f561be14421aebf030972c0c6862 #7:
        * 6. The remaining N5=12 bits encode the gossip timestamp in the
        *    form:
        *    timestamp % ((2^N5)-1)
        */
        status = (bool) mk_short_channel_id(scid, scid_blk, scid_tx, scid_out);
        /* FIXME: use block height */
        *timestamp = pull_bits(&ms_entry, &bitoff, 12);
        return status;
}
#else
void init_minisketch(struct routing_state *rstate)
{
}

void destroy_minisketch(struct routing_state *rstate)
{
}

struct chan *node_least_channel(struct node *node)
{
        return NULL;
}
void init_minisketch_channels(struct chan *chan)
{
}
void init_minisketch_node(struct node *node)
{
}
u64 minisketch_encode(u8 type,
                                    struct short_channel_id short_channel_id,
                                    u8 side,
                                    u32 timestamp)
{
        return 0;
}
u64 minisketch_encode_cupdate(struct chan *chan, u8 side,
                                            u32 timestamp)
{
        return 0;
}
u64 minisketch_encode_cannounce(struct chan *chan)
{
        return 0;
}
u64 minisketch_encode_nannounce(struct chan *chan,u32 timestamp, u8 side)
{
        return 0;
}
u8 minisketch_decode_type(u64 minisketch_entry)
{
        return 255;
}
bool minisketch_add_to_sketch(struct routing_state *rstate,
                                            u64 minisketch_entry)
{
        return NULL;
}
bool minisketch_sub_from_sketch(struct routing_state *rstate,
                                              u64 minisketch_entry)
{
        return false;
}
bool minisketch_add_cannounce(struct routing_state *rstate,
                                            struct chan *chan)
{
        return false;
}
void entry_decode(u64 ms_entry){
}
bool minisketch_handle_cannounce(struct routing_state *rstate,
                                               struct chan *chan,
                                               u32 timestamp){
        return false;
}
bool minisketch_handle_cupdate(struct routing_state *rstate,
                                             struct chan *chan,
                                             u8 side,
                                             u32 timestamp)
{
        return false;
}
bool minisketch_handle_nannounce(struct routing_state *rstate,
                                               const struct node_id *node_id,
                                               u32 timestamp)
{
        return false;
}
bool minisketch_decode_cupdate(u64 ms_entry,
                               struct short_channel_id *scid,
                               u8 *side,
                               u32 *timestamp){
        return NULL;
}
bool minisketch_decode_nannounce(u64 ms_entry,
                               struct short_channel_id *scid,
                               u8 *side,
                               u32 *timestamp){
        return NULL;
}
#endif /* EXPERIMENTAL_FEATURES */
