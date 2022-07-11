/* Test a canned gossmap we had troubl with */
#include "config.h"
#include "../amount.c"
#include "../fp16.c"
#include "../gossmap.c"
#include "../node_id.c"
#include "../pseudorand.c"
#include <ccan/read_write_all/read_write_all.h>
#include <common/channel_type.h>
#include <common/setup.h>
#include <common/utils.h>
#include <stdio.h>

/* AUTOGENERATED MOCKS START */
/* Generated stub for fromwire_bigsize */
bigsize_t fromwire_bigsize(const u8 **cursor UNNEEDED, size_t *max UNNEEDED)
{ fprintf(stderr, "fromwire_bigsize called!\n"); abort(); }
/* Generated stub for fromwire_channel_id */
bool fromwire_channel_id(const u8 **cursor UNNEEDED, size_t *max UNNEEDED,
			 struct channel_id *channel_id UNNEEDED)
{ fprintf(stderr, "fromwire_channel_id called!\n"); abort(); }
/* Generated stub for towire_bigsize */
void towire_bigsize(u8 **pptr UNNEEDED, const bigsize_t val UNNEEDED)
{ fprintf(stderr, "towire_bigsize called!\n"); abort(); }
/* Generated stub for towire_channel_id */
void towire_channel_id(u8 **pptr UNNEEDED, const struct channel_id *channel_id UNNEEDED)
{ fprintf(stderr, "towire_channel_id called!\n"); abort(); }
/* AUTOGENERATED MOCKS END */

/* Canned gossmap, taken from od -tx1 -Anone -v < /tmp/ltests-rtchpzh1/test_peers_1/lightning-2/regtest/gossip_store  | sed 's/ / 0x/g'| cut -c2- | sed -e 's/ /, /' -e 's/$/,/' */
static u8 canned_map[] = {
	0x0a,0x00,0x00,0x01,0xbc,0x4a,0xe8,0x33,0xa0,0x00,0x00,0x00,0x00,0x10,0x08,0x00,
	0x00,0x00,0x00,0x00,0x0f,0x42,0x40,0x01,0xb0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x22,0x6e,
	0x46,0x11,0x1a,0x0b,0x59,0xca,0xaf,0x12,0x60,0x43,0xeb,0x5b,0xbf,0x28,0xc3,0x4f,
	0x3a,0x5e,0x33,0x2a,0x1f,0xc7,0xb2,0xb7,0x3c,0xf1,0x88,0x91,0x0f,0x00,0x00,0x67,
	0x00,0x00,0x01,0x00,0x01,0x02,0x2d,0x22,0x36,0x20,0xa3,0x59,0xa4,0x7f,0xf7,0xf7,
	0xac,0x44,0x7c,0x85,0xc4,0x6c,0x92,0x3d,0xa5,0x33,0x89,0x22,0x1a,0x00,0x54,0xc1,
	0x1c,0x1e,0x3c,0xa3,0x1d,0x59,0x02,0x66,0xe4,0x59,0x8d,0x1d,0x3c,0x41,0x5f,0x57,
	0x2a,0x84,0x88,0x83,0x0b,0x60,0xf7,0xe7,0x44,0xed,0x92,0x35,0xeb,0x0b,0x1b,0xa9,
	0x32,0x83,0xb3,0x15,0xc0,0x35,0x18,0x03,0x1b,0x84,0xc5,0x56,0x7b,0x12,0x64,0x40,
	0x99,0x5d,0x3e,0xd5,0xaa,0xba,0x05,0x65,0xd7,0x1e,0x18,0x34,0x60,0x48,0x19,0xff,
	0x9c,0x17,0xf5,0xe9,0xd5,0xdd,0x07,0x8f,0x03,0x1b,0x84,0xc5,0x56,0x7b,0x12,0x64,
	0x40,0x99,0x5d,0x3e,0xd5,0xaa,0xba,0x05,0x65,0xd7,0x1e,0x18,0x34,0x60,0x48,0x19,
	0xff,0x9c,0x17,0xf5,0xe9,0xd5,0xdd,0x07,0x8f,0x00,0x00,0x00,0x8e,0xf7,0xf5,0x09,
	0x1e,0x00,0x00,0x00,0x00,0x10,0x06,0x00,0x8a,0x01,0x02,0x53,0x0b,0x21,0x64,0xa7,
	0xd8,0x03,0x4f,0x50,0xb8,0x16,0x9e,0xaa,0x73,0x6b,0xe0,0x70,0x36,0x16,0xc5,0xb1,
	0x04,0xd7,0xfd,0xc0,0x8f,0x68,0x72,0xd7,0x3e,0x38,0x72,0x05,0x7c,0x00,0x81,0x44,
	0x96,0xa2,0x22,0x08,0x15,0xdd,0x88,0x86,0xe7,0xb5,0x97,0x45,0x73,0x46,0x35,0xde,
	0xf0,0x62,0x11,0x22,0x1f,0xea,0x62,0xd0,0x40,0x6f,0x60,0x06,0x22,0x6e,0x46,0x11,
	0x1a,0x0b,0x59,0xca,0xaf,0x12,0x60,0x43,0xeb,0x5b,0xbf,0x28,0xc3,0x4f,0x3a,0x5e,
	0x33,0x2a,0x1f,0xc7,0xb2,0xb7,0x3c,0xf1,0x88,0x91,0x0f,0x00,0x00,0x67,0x00,0x00,
	0x01,0x00,0x01,0x62,0xc4,0xdb,0x93,0x01,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x0a,0x00,0x00,0x00,0x00,0x3b,
	0x02,0x33,0x80,0x00,0x00,0x00,0x8e,0x6f,0xbe,0x57,0x16,0x00,0x00,0x00,0x00,0x10,
	0x06,0x00,0x8a,0x01,0x02,0x0c,0x6c,0x84,0xfe,0xfd,0x31,0xe5,0x56,0x63,0xce,0xea,
	0x62,0x3c,0x82,0x3c,0xf8,0xb8,0xae,0x6d,0x8a,0xc7,0x60,0x44,0xe5,0x0d,0xaf,0xdd,
	0x76,0xae,0x54,0x08,0xec,0x13,0x45,0xa0,0x69,0x60,0x89,0x74,0x88,0xe1,0xe8,0xcb,
	0xcc,0x03,0xe3,0x1b,0x4a,0x77,0x17,0xfe,0xa1,0xe1,0xa3,0x42,0x39,0x2d,0xda,0x7b,
	0x68,0xf3,0xd5,0x97,0x88,0x06,0x22,0x6e,0x46,0x11,0x1a,0x0b,0x59,0xca,0xaf,0x12,
	0x60,0x43,0xeb,0x5b,0xbf,0x28,0xc3,0x4f,0x3a,0x5e,0x33,0x2a,0x1f,0xc7,0xb2,0xb7,
	0x3c,0xf1,0x88,0x91,0x0f,0x00,0x00,0x67,0x00,0x00,0x01,0x00,0x01,0x62,0xc4,0xdb,
	0x93,0x01,0x01,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x01,0x00,0x00,0x00,0x0a,0x00,0x00,0x00,0x00,0x3b,0x02,0x33,0x80,
};

static void check_cannounce(const u8 *cannounce,
			    const struct short_channel_id *scid,
			    const struct node_id *n1,
			    const struct node_id *n2)
{
	secp256k1_ecdsa_signature sig;
	u8 *features;
	struct bitcoin_blkid chain_hash;
	struct short_channel_id actual_scid;
	struct node_id actual_n1, actual_n2;
	struct pubkey k;

	assert(fromwire_channel_announcement(cannounce, cannounce,
					     &sig, &sig, &sig, &sig,
					     &features, &chain_hash,
					     &actual_scid,
					     &actual_n1,
					     &actual_n2,
					     &k, &k));
	assert(short_channel_id_eq(&actual_scid, scid));
	if (node_id_cmp(n1, n2) < 0) {
		assert(node_id_eq(&actual_n1, n1));
		assert(node_id_eq(&actual_n2, n2));
	} else {
		assert(node_id_eq(&actual_n1, n2));
		assert(node_id_eq(&actual_n2, n1));
	}
}

int main(int argc, char *argv[])
{
	int fd;
	char *gossfile;
	struct gossmap *map;
	struct node_id l1, l2;
	struct short_channel_id scid12;
	struct amount_sat capacity;
	u32 timestamp, fee_base_msat, fee_proportional_millionths;
	u8 message_flags, channel_flags;
	struct amount_msat htlc_minimum_msat, htlc_maximum_msat;
	u8 *cann;

	common_setup(argv[0]);

	fd = tmpdir_mkstemp(tmpctx, "run-gossip_canned.XXXXXX", &gossfile);
	assert(write_all(fd, canned_map, sizeof(canned_map)));

	map = gossmap_load(tmpctx, gossfile, NULL);
	assert(map);

	/* There is an unannounced channel 1<->2 (103x1x1) */
	assert(node_id_from_hexstr("0266e4598d1d3c415f572a8488830b60f7e744ed9235eb0b1ba93283b315c03518", 66, &l1));
	assert(node_id_from_hexstr("022d223620a359a47ff7f7ac447c85c46c923da53389221a0054c11c1e3ca31d59", 66, &l2));
	assert(gossmap_find_node(map, &l1));
	assert(gossmap_find_node(map, &l2));

	assert(short_channel_id_from_str("103x1x1", 7, &scid12));
	assert(gossmap_find_chan(map, &scid12));
	assert(gossmap_find_chan(map, &scid12)->private);
	assert(gossmap_chan_get_capacity(map, gossmap_find_chan(map, &scid12),
					 &capacity));
	assert(amount_sat_eq(capacity, AMOUNT_SAT(1000000)));

	gossmap_chan_get_update_details(map, gossmap_find_chan(map, &scid12),
					0,
					&timestamp,
					&message_flags,
					&channel_flags,
					&fee_base_msat,
					&fee_proportional_millionths,
					&htlc_minimum_msat,
					&htlc_maximum_msat);
	assert(timestamp == 1657068435);
	assert(message_flags == 1);
	assert(channel_flags == 0);
	assert(fee_base_msat == 1);
	assert(fee_proportional_millionths == 10);
	assert(amount_msat_eq(htlc_minimum_msat, AMOUNT_MSAT(0)));
	assert(amount_msat_eq(htlc_maximum_msat, AMOUNT_MSAT(990000000)));

	gossmap_chan_get_update_details(map, gossmap_find_chan(map, &scid12),
					1,
					&timestamp,
					&message_flags,
					&channel_flags,
					&fee_base_msat,
					&fee_proportional_millionths,
					&htlc_minimum_msat,
					&htlc_maximum_msat);
	assert(timestamp == 1657068435);
	assert(message_flags == 1);
	assert(channel_flags == 1);
	assert(fee_base_msat == 1);
	assert(fee_proportional_millionths == 10);
	assert(amount_msat_eq(htlc_minimum_msat, AMOUNT_MSAT(0)));
	assert(amount_msat_eq(htlc_maximum_msat, AMOUNT_MSAT(990000000)));

	assert(tal_bytelen(gossmap_chan_get_features(tmpctx, map,
						     gossmap_find_chan(map, &scid12))) == 0);

	cann = gossmap_chan_get_announce(tmpctx, map,
					 gossmap_find_chan(map, &scid12));
	check_cannounce(cann, &scid12, &l1, &l2);
	common_shutdown();
}
