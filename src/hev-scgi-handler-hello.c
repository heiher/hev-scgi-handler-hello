/*
 ============================================================================
 Name        : hev-scgi-handler-hello.c
 Author      : Heiher <admin@heiher.info>
 Version     : 0.0.2
 Copyright   : Copyright (C) 2012 everyone.
 Description : 
 ============================================================================
 */

#include <hev-scgi-1.0.h>

#include "hev-scgi-handler-hello.h"

#define HEV_SCGI_HANDLER_HELLO_NAME		"HevSCGIHandlerHello"
#define HEV_SCGI_HANDLER_HELLO_VERSION	"0.0.2"

enum
{
	PROP_0,
	PROP_CONFIG,
	N_PROPERTIES
};

static GParamSpec *hev_scgi_handler_hello_properties[N_PROPERTIES] = { NULL };

#define HEV_SCGI_HANDLER_HELLO_GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), HEV_TYPE_SCGI_HANDLER_HELLO, HevSCGIHandlerHelloPrivate))

typedef struct _HevSCGIHandlerHelloPrivate HevSCGIHandlerHelloPrivate;

struct _HevSCGIHandlerHelloPrivate
{
	GKeyFile *config;
	gchar *alias;
	gchar *pattern;
};

static void hev_scgi_handler_iface_init(HevSCGIHandlerInterface * iface);

G_DEFINE_DYNAMIC_TYPE_EXTENDED(HevSCGIHandlerHello, hev_scgi_handler_hello, G_TYPE_OBJECT, 0,
			G_IMPLEMENT_INTERFACE_DYNAMIC(HEV_TYPE_SCGI_HANDLER, hev_scgi_handler_iface_init));

static void hev_scgi_response_write_header_async_handler(GObject *source_object,
			GAsyncResult *res, gpointer user_data);
static void hev_scgi_handler_hello_output_stream_write_async_handler(GObject *source_object,
			GAsyncResult *res, gpointer user_data);

void hev_scgi_handler_hello_reg_type(GTypeModule *module)
{
	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	if(G_TYPE_INVALID == HEV_TYPE_SCGI_HANDLER_HELLO)
	  hev_scgi_handler_hello_register_type(module);
}

static const gchar * hev_scgi_handler_hello_get_alias(HevSCGIHandler *handler);
static const gchar * hev_scgi_handler_hello_get_name(HevSCGIHandler *handler);
static const gchar * hev_scgi_handler_hello_get_version(HevSCGIHandler *handler);
static const gchar * hev_scgi_handler_hello_get_pattern(HevSCGIHandler *handler);
static void hev_scgi_handler_hello_handle(HevSCGIHandler *self, GObject *scgi_task);

static void hev_scgi_handler_hello_dispose(GObject *obj)
{
	HevSCGIHandlerHello *self = HEV_SCGI_HANDLER_HELLO(obj);
	HevSCGIHandlerHelloPrivate *priv = HEV_SCGI_HANDLER_HELLO_GET_PRIVATE(self);

	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	G_OBJECT_CLASS(hev_scgi_handler_hello_parent_class)->dispose(obj);
}

static void hev_scgi_handler_hello_finalize(GObject *obj)
{
	HevSCGIHandlerHello *self = HEV_SCGI_HANDLER_HELLO(obj);
	HevSCGIHandlerHelloPrivate *priv = HEV_SCGI_HANDLER_HELLO_GET_PRIVATE(self);

	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	if(priv->config)
	{
		g_key_file_free(priv->config);
		priv->config = NULL;
	}

	if(priv->alias)
	{
		g_free(priv->alias);
		priv->alias = NULL;
	}

	if(priv->pattern)
	{
		g_free(priv->pattern);
		priv->pattern = NULL;
	}

	G_OBJECT_CLASS(hev_scgi_handler_hello_parent_class)->finalize(obj);
}

static void hev_scgi_handler_hello_class_finalize(HevSCGIHandlerHelloClass *klass)
{
	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);
}

static GObject * hev_scgi_handler_hello_constructor(GType type, guint n, GObjectConstructParam *param)
{
	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	return G_OBJECT_CLASS(hev_scgi_handler_hello_parent_class)->constructor(type, n, param);
}

static void hev_scgi_handler_hello_constructed(GObject *obj)
{
	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	G_OBJECT_CLASS(hev_scgi_handler_hello_parent_class)->constructed(obj);
}

static void hev_scgi_handler_hello_set_property(GObject *obj,
			guint prop_id, const GValue *value, GParamSpec *pspec)
{
	HevSCGIHandlerHello *self = HEV_SCGI_HANDLER_HELLO(obj);
	HevSCGIHandlerHelloPrivate *priv = HEV_SCGI_HANDLER_HELLO_GET_PRIVATE(self);

	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	switch(prop_id)
	{
	case PROP_CONFIG:
			priv->config = g_value_get_pointer(value);
			break;
	default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void hev_scgi_handler_hello_get_property(GObject *obj,
			guint prop_id, GValue *value, GParamSpec *pspec)
{
	HevSCGIHandlerHello *self = HEV_SCGI_HANDLER_HELLO(obj);
	HevSCGIHandlerHelloPrivate *priv = HEV_SCGI_HANDLER_HELLO_GET_PRIVATE(self);

	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	switch(prop_id)
	{
	case PROP_CONFIG:
			g_value_set_pointer(value, priv->config);
			break;
	default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void hev_scgi_handler_hello_class_init(HevSCGIHandlerHelloClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	obj_class->constructor = hev_scgi_handler_hello_constructor;
	obj_class->constructed = hev_scgi_handler_hello_constructed;
	obj_class->dispose = hev_scgi_handler_hello_dispose;
	obj_class->finalize = hev_scgi_handler_hello_finalize;

	obj_class->set_property = hev_scgi_handler_hello_set_property;
	obj_class->get_property = hev_scgi_handler_hello_get_property;

	hev_scgi_handler_hello_properties[PROP_CONFIG] =
		g_param_spec_pointer ("config", "Config", "The module config",
					G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
	g_object_class_install_properties(obj_class, N_PROPERTIES,
				hev_scgi_handler_hello_properties);

	g_type_class_add_private(klass, sizeof(HevSCGIHandlerHelloPrivate));
}

static void hev_scgi_handler_iface_init(HevSCGIHandlerInterface * iface)
{
	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	iface->get_alias = hev_scgi_handler_hello_get_alias;
	iface->get_name = hev_scgi_handler_hello_get_name;
	iface->get_version = hev_scgi_handler_hello_get_version;
	iface->get_pattern = hev_scgi_handler_hello_get_pattern;
	iface->handle = hev_scgi_handler_hello_handle;
}

static void hev_scgi_handler_hello_init(HevSCGIHandlerHello *self)
{
	HevSCGIHandlerHelloPrivate *priv = HEV_SCGI_HANDLER_HELLO_GET_PRIVATE(self);

	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	priv->config = NULL;
}

static const gchar * hev_scgi_handler_hello_get_alias(HevSCGIHandler *handler)
{
	HevSCGIHandlerHello *self = HEV_SCGI_HANDLER_HELLO(handler);
	HevSCGIHandlerHelloPrivate *priv = HEV_SCGI_HANDLER_HELLO_GET_PRIVATE(self);

	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	if(!priv->alias)
	  priv->alias = g_key_file_get_string(priv->config,
				  "Module", "Alias", NULL);

	return priv->alias;
}

static const gchar * hev_scgi_handler_hello_get_name(HevSCGIHandler *handler)
{
	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	return HEV_SCGI_HANDLER_HELLO_NAME;
}

static const gchar * hev_scgi_handler_hello_get_version(HevSCGIHandler *handler)
{
	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	return HEV_SCGI_HANDLER_HELLO_VERSION;
}

static const gchar * hev_scgi_handler_hello_get_pattern(HevSCGIHandler *handler)
{
	HevSCGIHandlerHello *self = HEV_SCGI_HANDLER_HELLO(handler);
	HevSCGIHandlerHelloPrivate *priv = HEV_SCGI_HANDLER_HELLO_GET_PRIVATE(self);

	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	if(!priv->pattern)
	  priv->pattern = g_key_file_get_string(priv->config,
				  "Module", "Pattern", NULL);

	return priv->pattern;
}

static void hev_scgi_handler_hello_handle(HevSCGIHandler *self, GObject *scgi_task)
{
	GObject *scgi_response = NULL;
	GOutputStream *output_stream = NULL;
	GHashTable *res_hash_table = NULL;

	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	g_object_ref(scgi_task);

	scgi_response = hev_scgi_task_get_response(HEV_SCGI_TASK(scgi_task));
	output_stream = hev_scgi_response_get_output_stream(HEV_SCGI_RESPONSE(scgi_response));
	res_hash_table = hev_scgi_response_get_header_hash_table(HEV_SCGI_RESPONSE(scgi_response));

	g_hash_table_insert(res_hash_table, g_strdup("Status"), g_strdup("200 OK"));
	g_hash_table_insert(res_hash_table, g_strdup("Content-Type"), g_strdup("text/html"));
	hev_scgi_response_write_header_async(HEV_SCGI_RESPONSE(scgi_response), NULL,
				hev_scgi_response_write_header_async_handler, scgi_task);
}

static void hev_scgi_response_write_header_async_handler(GObject *source_object,
			GAsyncResult *res, gpointer user_data)
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

	if(!hev_scgi_response_write_header_finish(HEV_SCGI_RESPONSE(source_object),
					res, NULL))
	{
		g_string_free(str, TRUE);
		g_object_unref(scgi_task);

		return;
	}

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
				hev_scgi_handler_hello_output_stream_write_async_handler,
				scgi_task);
}

static void hev_scgi_handler_hello_output_stream_write_async_handler(GObject *source_object,
			GAsyncResult *res, gpointer user_data)
{
	g_debug("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

	g_output_stream_write_finish(G_OUTPUT_STREAM(source_object),
				res, NULL);

	g_string_free(g_object_get_data(user_data, "str"), TRUE);
	g_object_unref(user_data);
}

