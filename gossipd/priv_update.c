#include "config.h"
#include <bitcoin/short_channel_id.h>
#include <common/node_id.h>
#include <gossipd/priv_update.h>
#include <wire/wire.h>

void towire_remote_priv_update(u8 **pptr,
			       const struct remote_priv_update *update)
{
	towire_node_id(pptr, &update->source_node);
	towire_u64(pptr, update->scid.u64);
	towire_u32(pptr, update->fee_base);
	towire_u32(pptr, update->fee_ppm);
	towire_u16(pptr, update->cltv_delta);
	towire_amount_msat(pptr, update->htlc_minimum_msat);
	towire_amount_msat(pptr, update->htlc_maximum_msat);
}

void fromwire_remote_priv_update(const u8 **cursor, size_t *max,
			         struct remote_priv_update *update)
{
	fromwire_node_id(cursor, max, &update->source_node);
	update->scid.u64 = fromwire_u64(cursor, max);
	update->fee_base = fromwire_u32(cursor, max);
	update->fee_ppm = fromwire_u32(cursor, max);
	update->cltv_delta = fromwire_u16(cursor, max);
	update->htlc_minimum_msat = fromwire_amount_msat(cursor, max);
	update->htlc_maximum_msat = fromwire_amount_msat(cursor, max);
}
