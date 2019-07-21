#include "gen_test.h"
#include "gen_print.h"

#include <assert.h>
#include <stdio.h>

#include <common/utils.h>

/* AUTOGENERATED MOCKS START */
/* Generated stub for fromwire_peektype */
int fromwire_peektype(const u8 *cursor UNNEEDED)
{ fprintf(stderr, "fromwire_peektype called!\n"); abort(); }
/* Generated stub for printwire_amount_msat */
void printwire_amount_msat(const char *fieldname UNNEEDED, const struct amount_msat *msat UNNEEDED)
{ fprintf(stderr, "printwire_amount_msat called!\n"); abort(); }
/* Generated stub for printwire_tlvs */
void printwire_tlvs(const char *tlv_name UNNEEDED, const u8 **cursor UNNEEDED, size_t *plen UNNEEDED,
		    const struct tlv_print_record_type types[] UNNEEDED, size_t num_types UNNEEDED)
{ fprintf(stderr, "printwire_tlvs called!\n"); abort(); }
/* Generated stub for printwire_u16 */
void printwire_u16(const char *fieldname UNNEEDED, const u16 *v UNNEEDED)
{ fprintf(stderr, "printwire_u16 called!\n"); abort(); }
/* Generated stub for printwire_u32 */
void printwire_u32(const char *fieldname UNNEEDED, const u32 *v UNNEEDED)
{ fprintf(stderr, "printwire_u32 called!\n"); abort(); }
/* Generated stub for printwire_u64 */
void printwire_u64(const char *fieldname UNNEEDED, const u64 *v UNNEEDED)
{ fprintf(stderr, "printwire_u64 called!\n"); abort(); }
/* Generated stub for printwire_u8_array */
void printwire_u8_array(const char *fieldname UNNEEDED, const u8 **cursor UNNEEDED, size_t *plen UNNEEDED, size_t len UNNEEDED)
{ fprintf(stderr, "printwire_u8_array called!\n"); abort(); }
/* AUTOGENERATED MOCKS END */

int main(void)
{
	setup_locale();
	void *ctx = tal(NULL, char);

	struct tlv_n1 *n1 = tlv_n1_new(ctx);
	struct tlv_n2 *n2 = tlv_n2_new(ctx);
	struct tlv_n3 *n3 = tlv_n3_new(ctx);

	assert(n1);
	assert(n2);
	assert(n3);

	tal_free(ctx);
}
