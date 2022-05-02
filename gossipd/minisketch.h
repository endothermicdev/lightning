#ifndef LIGHTNING_GOSSIPD_MINISKETCH_H
#define LIGHTNING_GOSSIPD_MINISKETCH_H
#include "config.h"

struct chan;
struct node;
struct routing_state;
struct node_id;

//#if EXPERIMENTAL_FEATURES

void init_minisketch(struct routing_state *rstate);
void destroy_minisketch(struct routing_state *rstate);
struct chan *node_least_channel(struct node *node);
u64 minisketch_encode(u8 type,
                      struct short_channel_id short_channel_id, u8 side,
                      u32 timestamp);
u64 minisketch_encode_cupdate(struct chan *chan, u8 side,u32 timestamp);
u64 minisketch_encode_cannounce(struct chan *chan);
u64 minisketch_encode_nannounce(struct chan *chan,u32 timestamp, u8 side);
u8 minisketch_decode_type(u64 minisketch_entry);
void init_minisketch_channels(struct chan *chan);
void init_minisketch_node(struct node *node);
bool minisketch_add_to_sketch(struct routing_state *rstate,
                              u64 minisketch_entry);
bool minisketch_sub_from_sketch(struct routing_state *rstate,
                                u64 minisketch_entry);
bool minisketch_add_cannounce(struct routing_state *rstate,
                              struct chan *chan);
void entry_decode(u64 ms_entry);
bool minisketch_handle_cannounce(struct routing_state *rstate,
                                 struct chan *chan,
                                 u32 timestamp);
bool minisketch_handle_cupdate(struct routing_state *rstate,
                               struct chan *chan,
                               u8 side,
                               u32 timestamp);
bool minisketch_handle_nannounce(struct routing_state *rstate,
                                 const struct node_id *node_id,
                                 u32 timestamp);
bool minisketch_decode_cannounce(u64 ms_entry,
                                 struct short_channel_id *scid,
                                 u32 *timestamp);
bool minisketch_decode_cupdate(u64 ms_entry,
                               struct short_channel_id *scid,
                               u8 *side,
                               u32 *timestamp);
bool minisketch_decode_nannounce(u64 ms_entry,
                               struct short_channel_id *scid,
                               u8 *side,
                               u32 *timestamp);

//#else /* Noop versions */
// static inline void init_minisketch(struct routing_state *rstate)
// {
// }
//
// static inline void destroy_minisketch(struct routing_state *rstate)
// {
// }
//
// static inline struct chan *node_least_channel(struct node *node)
// {
//         return NULL;
// }
// static inline void init_minisketch_channels(struct chan *chan)
// {
// }
// static inline void init_minisketch_node(struct node *node)
// {
// }
// static inline u64 minisketch_encode(u8 type,
//                                     struct short_channel_id short_channel_id,
//                                     u8 side,
//                                     u32 timestamp)
// {
//         return 0;
// }
// static inline u64 minisketch_encode_cupdate(struct chan *chan, u8 side,
//                                             u32 timestamp)
// {
//         return 0;
// }
// static inline u64 minisketch_encode_cannounce(struct chan *chan)
// {
//         return 0;
// }
// static inline u64 minisketch_encode_nannounce(struct chan *chan,u32 timestamp, u8 side)
// {
//         return 0;
// }
// static inline u8 minisketch_decode_type(u64 minisketch_entry)
// {
//         return 255;
// }
// static inline bool minisketch_add_to_sketch(struct routing_state *rstate,
//                                             u64 minisketch_entry)
// {
//         return NULL;
// }
// static inline bool minisketch_sub_from_sketch(struct routing_state *rstate,
//                                               u64 minisketch_entry)
// {
//         return false;
// }
// static inline bool minisketch_add_cannounce(struct routing_state *rstate,
//                                             struct chan *chan)
// {
//         return false;
// }
// static inline void entry_decode(u64 ms_entry){
// }
// static inline bool minisketch_handle_cannounce(struct routing_state *rstate,
//                                                struct chan *chan,
//                                                u32 timestamp){
//         return false;
// }
// static inline bool minisketch_handle_cupdate(struct routing_state *rstate,
//                                              struct chan *chan,
//                                              u8 side,
//                                              u32 timestamp)
// {
//         return false;
// }
// static inline bool minisketch_handle_nannounce(struct routing_state *rstate,
//                                                const struct node_id *node_id,
//                                                u32 timestamp)
// {
//         return false;
// }
// static inline bool minisketch_decode_cupdate(u64 ms_entry,
//                                struct short_channel_id *scid,
//                                u8 *side,
//                                u32 *timestamp){
//         return NULL;
// }
// static inline bool minisketch_decode_nannounce(u64 ms_entry,
//                                struct short_channel_id *scid,
//                                u8 *side,
//                                u32 *timestamp){
//         return NULL;
// }
//#endif /* EXPERIMENTAL_FEATURES */
#endif /* LIGHTNING_GOSSIPD_MINISKETCH_H */
