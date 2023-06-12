#ifndef LIGHTNING_GOSSIPD_PRIV_UPDATE_H
#define LIGHTNING_GOSSIPD_PRIV_UPDATE_H
#include "config.h"
#include <bitcoin/short_channel_id.h>
#include <ccan/short_types/short_types.h>
#include <common/amount.h>
#include <common/node_id.h>
#include <wire/wire.h>

/* structure for wire transmission of remote channel update parameters to ld */
struct remote_priv_update {
	struct node_id source_node;
	struct short_channel_id scid;
	u32 fee_base;
	u32 fee_ppm;
	u16 cltv_delta;
	struct amount_msat htlc_minimum_msat;
	struct amount_msat htlc_maximum_msat;
};

void towire_remote_priv_update(u8 **pptr,
			       const struct remote_priv_update *update);

void fromwire_remote_priv_update(const u8 **cursor, size_t *max,
				 struct remote_priv_update *update);

#endif /* LIGHTNING_GOSSIPD_PRIV_UPDATE_H */
