#include "config.h"
#include "../log.c"
#include <common/setup.h>

/* AUTOGENERATED MOCKS START */
/* Generated stub for command_fail */
struct command_result *command_fail(struct command *cmd UNNEEDED, enum jsonrpc_errcode code UNNEEDED,
				    const char *fmt UNNEEDED, ...)

{ fprintf(stderr, "command_fail called!\n"); abort(); }
/* Generated stub for command_param_failed */
struct command_result *command_param_failed(void)

{ fprintf(stderr, "command_param_failed called!\n"); abort(); }
/* Generated stub for command_success */
struct command_result *command_success(struct command *cmd UNNEEDED,
				       struct json_stream *response)

{ fprintf(stderr, "command_success called!\n"); abort(); }
/* Generated stub for fmt_node_id */
char *fmt_node_id(const tal_t *ctx UNNEEDED, const struct node_id *id UNNEEDED)
{ fprintf(stderr, "fmt_node_id called!\n"); abort(); }
/* Generated stub for fromwire_bigsize */
bigsize_t fromwire_bigsize(const u8 **cursor UNNEEDED, size_t *max UNNEEDED)
{ fprintf(stderr, "fromwire_bigsize called!\n"); abort(); }
/* Generated stub for fromwire_channel_id */
bool fromwire_channel_id(const u8 **cursor UNNEEDED, size_t *max UNNEEDED,
			 struct channel_id *channel_id UNNEEDED)
{ fprintf(stderr, "fromwire_channel_id called!\n"); abort(); }
/* Generated stub for fromwire_node_id */
void fromwire_node_id(const u8 **cursor UNNEEDED, size_t *max UNNEEDED, struct node_id *id UNNEEDED)
{ fprintf(stderr, "fromwire_node_id called!\n"); abort(); }
/* Generated stub for json_add_hex_talarr */
void json_add_hex_talarr(struct json_stream *result UNNEEDED,
			 const char *fieldname UNNEEDED,
			 const tal_t *data UNNEEDED)
{ fprintf(stderr, "json_add_hex_talarr called!\n"); abort(); }
/* Generated stub for json_add_node_id */
void json_add_node_id(struct json_stream *response UNNEEDED,
				const char *fieldname UNNEEDED,
				const struct node_id *id UNNEEDED)
{ fprintf(stderr, "json_add_node_id called!\n"); abort(); }
/* Generated stub for json_add_num */
void json_add_num(struct json_stream *result UNNEEDED, const char *fieldname UNNEEDED,
		  unsigned int value UNNEEDED)
{ fprintf(stderr, "json_add_num called!\n"); abort(); }
/* Generated stub for json_add_str_fmt */
void json_add_str_fmt(struct json_stream *js UNNEEDED,
		      const char *fieldname UNNEEDED,
		      const char *fmt UNNEEDED, ...)
{ fprintf(stderr, "json_add_str_fmt called!\n"); abort(); }
/* Generated stub for json_add_string */
void json_add_string(struct json_stream *js UNNEEDED,
		     const char *fieldname UNNEEDED,
		     const char *str TAKES UNNEEDED)
{ fprintf(stderr, "json_add_string called!\n"); abort(); }
/* Generated stub for json_add_timestr */
void json_add_timestr(struct json_stream *result UNNEEDED, const char *fieldname UNNEEDED,
			  struct timespec ts UNNEEDED)
{ fprintf(stderr, "json_add_timestr called!\n"); abort(); }
/* Generated stub for json_array_end */
void json_array_end(struct json_stream *js UNNEEDED)
{ fprintf(stderr, "json_array_end called!\n"); abort(); }
/* Generated stub for json_array_start */
void json_array_start(struct json_stream *js UNNEEDED, const char *fieldname UNNEEDED)
{ fprintf(stderr, "json_array_start called!\n"); abort(); }
/* Generated stub for json_object_end */
void json_object_end(struct json_stream *js UNNEEDED)
{ fprintf(stderr, "json_object_end called!\n"); abort(); }
/* Generated stub for json_object_start */
void json_object_start(struct json_stream *ks UNNEEDED, const char *fieldname UNNEEDED)
{ fprintf(stderr, "json_object_start called!\n"); abort(); }
/* Generated stub for json_stream_log_suppress_for_cmd */
void json_stream_log_suppress_for_cmd(struct json_stream *js UNNEEDED,
					    const struct command *cmd UNNEEDED)
{ fprintf(stderr, "json_stream_log_suppress_for_cmd called!\n"); abort(); }
/* Generated stub for json_stream_success */
struct json_stream *json_stream_success(struct command *cmd UNNEEDED)
{ fprintf(stderr, "json_stream_success called!\n"); abort(); }
/* Generated stub for log_level_name */
const char *log_level_name(enum log_level level UNNEEDED)
{ fprintf(stderr, "log_level_name called!\n"); abort(); }
/* Generated stub for log_level_parse */
bool log_level_parse(const char *levelstr UNNEEDED, size_t len UNNEEDED,
		     enum log_level *level UNNEEDED)
{ fprintf(stderr, "log_level_parse called!\n"); abort(); }
/* Generated stub for notify_log */
void notify_log(struct lightningd *ld UNNEEDED, const struct log_entry *l UNNEEDED)
{ fprintf(stderr, "notify_log called!\n"); abort(); }
/* Generated stub for notify_warning */
void notify_warning(struct lightningd *ld UNNEEDED, struct log_entry *l UNNEEDED)
{ fprintf(stderr, "notify_warning called!\n"); abort(); }
/* Generated stub for param */
bool param(struct command *cmd UNNEEDED, const char *buffer UNNEEDED,
	   const jsmntok_t params[] UNNEEDED, ...)
{ fprintf(stderr, "param called!\n"); abort(); }
/* Generated stub for pubkey_from_node_id */
bool pubkey_from_node_id(struct pubkey *key UNNEEDED, const struct node_id *id UNNEEDED)
{ fprintf(stderr, "pubkey_from_node_id called!\n"); abort(); }
/* Generated stub for towire_bigsize */
void towire_bigsize(u8 **pptr UNNEEDED, const bigsize_t val UNNEEDED)
{ fprintf(stderr, "towire_bigsize called!\n"); abort(); }
/* Generated stub for towire_channel_id */
void towire_channel_id(u8 **pptr UNNEEDED, const struct channel_id *channel_id UNNEEDED)
{ fprintf(stderr, "towire_channel_id called!\n"); abort(); }
/* Generated stub for towire_node_id */
void towire_node_id(u8 **pptr UNNEEDED, const struct node_id *id UNNEEDED)
{ fprintf(stderr, "towire_node_id called!\n"); abort(); }
/* AUTOGENERATED MOCKS END */

int main(int argc, char *argv[])
{
	struct log_book *lb;
	struct logger *l;

	common_setup(argv[0]);

	lb = new_log_book(NULL,
			  (sizeof(struct log_entry) + sizeof("test XXXXXX"))
			  *100);
	l = new_logger(lb, lb, NULL, "test %s", "prefix");

	assert(streq(log_prefix(l), "test prefix"));

	for (size_t i = 0; i < 100; i++)
		log_debug(l, "test %06zi", i);

	assert(lb->num_entries == 100);
	for (size_t i = 0; i < 100; i++) {
		assert(lb->log[i].level == LOG_DBG);
		assert(lb->log[i].skipped == 0);
		assert(lb->log[i].nc == NULL);
		assert(lb->log[i].prefix->refcnt == 101);
		assert(streq(lb->log[i].prefix->prefix, "test prefix"));
		assert(streq(lb->log[i].log, tal_fmt(lb, "test %06zi", i)));
		assert(lb->log[i].io == NULL);
	}

	log_debug(l, "final test message");
	assert(lb->num_entries < 100);
	assert(lb->num_entries > 11);

	/* last 10% must be preserved exactly (with final and pruning
	 * msg appended) */
	for (size_t i = 91; i < 100; i++) {
		size_t pos = lb->num_entries - 2 - (100 - i);
		assert(streq(lb->log[pos].log, tal_fmt(lb, "test %06zi", i)));
	}
	assert(streq(lb->log[lb->num_entries - 2].log, "final test message"));

	/* Sum should still reflect 102 total messages */
	size_t total = 0;
	for (size_t i = 0; i < lb->num_entries; i++)
		total += 1 + lb->log[i].skipped;
	assert(total == 102);

	/* Freeing (last) log frees logbook */
	tal_free(l);
	common_shutdown();
}
