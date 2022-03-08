#ifndef LIGHTNING_GOSSIPD_MINISKETCH_H
#define LIGHTNING_GOSSIPD_MINISKETCH_H
#include "config.h"

struct chan;
struct node;
struct routing_state;

#if EXPERIMENTAL_FEATURES

void init_minisketch(struct routing_state *rstate);
void destroy_minisketch(struct routing_state *rstate);
struct chan *node_least_channel(struct node *node);
u64 minisketch_encode(u8 type,
                  struct short_channel_id short_channel_id, u8 side,
                  u32 timestamp);
u64 minisketch_encode_cupdate(struct chan *chan, u8 direction,u32 timestamp);
u64 minisketch_encode_cannounce(struct chan *chan,u32 timestamp);
u64 minisketch_encode_nannounce(struct chan *chan,u32 timestamp, u8 side);
u8 minisketch_decode_type(u64 minisketch_entry);
void init_minisketch_channels(struct chan *chan);
void init_minisketch_node(struct node *node);
bool minisketch_add_to_sketch(struct routing_state *rstate, u64 minisketch_entry);

#else /* Noop versions */
static inline void init_minisketch(struct routing_state *rstate)
{
}

static inline void destroy_minisketch(struct routing_state *rstate)
{
}

static inline struct chan *node_least_channel(struct node *node)
{
}
static inline u64 minisketch_implementation(const minisketch* sketch) {sketch_encode(u8 type,
                  struct short_channel_id short_channel_id, u8 side,
                  u32 timestamp)
{
        return NULL;
}
static inline u64 minisketch_encode_cupdate(struct chan *chan, u8 direction,
                                            u32 timestamp)
{
        return NULL;
}
static inline u64 minisketch_encode_cannounce(struct chan *chan,u32 timestamp)
{
        return NULL;
}
u64 minisketch_encode_nannounce(struct chan *chan,u32 timestamp, u8 side)
{
        return NULL;
}
u8 minisketch_decode_type(u64 minisketch_entry)
{
        return NULL;
}
void init_minisketch_channels(struct chan *chan)
{
}
void init_minisketch_node(struct node *node)
{
}
bool minisketch_add_to_sketch(struct routing_state *rstate, u64 minisketch_entry)
{
        return NULL;
}
#endif /* EXPERIMENTAL_FEATURES */
#endif /* LIGHTNING_GOSSIPD_MINISKETCH_H */
