/*
 ============================================================================
 Name        : hev-scgi-handler-hello.c
 Author      : Heiher <admin@heiher.info>
 Version     : 0.0.1
 Copyright   : Copyright (C) 2011 everyone.
 Description : 
 ============================================================================
 */

#include <gmodule.h>

#include "hev-scgi-handler.h"
#include "hev-scgi-task.h"
#include "hev-scgi-request.h"
#include "hev-scgi-response.h"

#define HEV_SCGI_HANDLER_HELLO_NAME		"HevSCGIHandlerHello"
#define HEV_SCGI_HANDLER_HELLO_VERSION	"0.0.1"
#define HEV_SCGI_HANDLER_HELLO_PATTERN	"^/scgi/hello.scgi$"

static void hev_scgi_handler_default_response_write_header_handler(gpointer user,
			gpointer user_data);
static void hev_scgi_handler_default_output_stream_write_async_handler(GObject *source_object,
			GAsyncResult *res, gpointer user_data);

G_MODULE_EXPORT const gchar * hev_scgi_handler_module_get_name(HevSCGIHandler *self)
{
	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);
	return HEV_SCGI_HANDLER_HELLO_NAME;
}

G_MODULE_EXPORT const gchar * hev_scgi_handler_module_get_version(HevSCGIHandler *self)
{
	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);
	return HEV_SCGI_HANDLER_HELLO_VERSION;
}

G_MODULE_EXPORT const gchar * hev_scgi_handler_module_get_pattern(HevSCGIHandler *self)
{
	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);
	return HEV_SCGI_HANDLER_HELLO_PATTERN;
}

G_MODULE_EXPORT void hev_scgi_handler_module_handle(HevSCGIHandler *self, GObject *scgi_task)
{
	GObject *scgi_response = NULL;
	GOutputStream *output_stream = NULL;
	GHashTable *res_hash_table = NULL;

	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	g_object_ref(scgi_task);

	scgi_response = hev_scgi_task_get_response(HEV_SCGI_TASK(scgi_task));
	output_stream = hev_scgi_response_get_output_stream(HEV_SCGI_RESPONSE(scgi_response));
	res_hash_table = hev_scgi_response_get_header_hash_table(HEV_SCGI_RESPONSE(scgi_response));

	g_hash_table_insert(res_hash_table, "Status", g_strdup("200 OK"));
	g_hash_table_insert(res_hash_table, "Content-Type", g_strdup("text/html"));
	hev_scgi_response_write_header(HEV_SCGI_RESPONSE(scgi_response),
				hev_scgi_handler_default_response_write_header_handler, scgi_task);
}

static void hev_scgi_handler_default_response_write_header_handler(gpointer user,
			gpointer user_data)
{
	HevSCGITask *scgi_task = HEV_SCGI_TASK(user_data);
	HevSCGIHandler *self = HEV_SCGI_HANDLER(hev_scgi_task_get_handler(scgi_task));
	GObject *scgi_request = NULL;
	GObject *scgi_response = NULL;
	GInputStream *input_stream = NULL;
	GOutputStream *output_stream = NULL;
	GHashTable *req_hash_table = NULL;
	GString *str = g_string_new(NULL);

	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	scgi_request = hev_scgi_task_get_request(HEV_SCGI_TASK(scgi_task));
	scgi_response = hev_scgi_task_get_response(HEV_SCGI_TASK(scgi_task));

	input_stream = hev_scgi_request_get_input_stream(HEV_SCGI_REQUEST(scgi_request));
	output_stream = hev_scgi_response_get_output_stream(HEV_SCGI_RESPONSE(scgi_response));

	req_hash_table = hev_scgi_request_get_header_hash_table(HEV_SCGI_REQUEST(scgi_request));

	g_object_set_data(G_OBJECT(scgi_task), "str", str);
	g_string_printf(str, "<strong>Handler:</strong> %s %s<br />"
				"<strong>RequestURI:</strong> %s<br />"
				"<strong>RemoteAddr:</strong> %s<br />"
				"<strong>RemotePort:</strong> %s<br />",
				hev_scgi_handler_get_name(HEV_SCGI_HANDLER(self)),
				hev_scgi_handler_get_version(HEV_SCGI_HANDLER(self)),
				g_hash_table_lookup(req_hash_table, "REQUEST_URI"),
				g_hash_table_lookup(req_hash_table, "REMOTE_ADDR"),
				g_hash_table_lookup(req_hash_table, "REMOTE_PORT"));
	g_output_stream_write_async(output_stream, str->str, str->len, 0, NULL,
				hev_scgi_handler_default_output_stream_write_async_handler,
				scgi_task);
}

static void hev_scgi_handler_default_output_stream_write_async_handler(GObject *source_object,
			GAsyncResult *res, gpointer user_data)
{
	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	g_output_stream_write_finish(G_OUTPUT_STREAM(source_object),
				res, NULL);

	g_string_free(g_object_get_data(user_data, "str"), TRUE);
	g_object_unref(user_data);
}

