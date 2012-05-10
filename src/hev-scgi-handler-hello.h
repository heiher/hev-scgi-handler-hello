/*
 ============================================================================
 Name        : hev-scgi-handler-hello.h
 Author      : Heiher <admin@heiher.info>
 Version     : 0.0.2
 Copyright   : Copyright (C) 2012 everyone.
 Description : 
 ============================================================================
 */

#ifndef __HEV_SCGI_HANDLER_HELLO_H__
#define __HEV_SCGI_HANDLER_HELLO_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define HEV_REGISTER_TYPE_SCGI_HANDLER_HELLO(module) (hev_scgi_handler_hello_reg_type(module))
#define HEV_TYPE_SCGI_HANDLER_HELLO	(hev_scgi_handler_hello_get_type())
#define HEV_SCGI_HANDLER_HELLO(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj), HEV_TYPE_SCGI_HANDLER_HELLO, HevSCGIHandlerHello))
#define HEV_IS_SCGI_HANDLER_HELLO(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj), HEV_TYPE_SCGI_HANDLER_HELLO))
#define HEV_SCGI_HANDLER_HELLO_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass), HEV_TYPE_SCGI_HANDLER_HELLO, HevSCGIHandlerHelloClass))
#define HEV_IS_SCGI_HANDLER_HELLO_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), HEV_TYPE_SCGI_HANDLER_HELLO))
#define HEV_SCGI_HANDLER_HELLO_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), HEV_TYPE_SCGI_HANDLER_HELLO, HevSCGIHandlerHelloClass))

typedef struct _HevSCGIHandlerHello HevSCGIHandlerHello;
typedef struct _HevSCGIHandlerHelloClass HevSCGIHandlerHelloClass;

struct _HevSCGIHandlerHello
{
	GObject parent_instance;
};

struct _HevSCGIHandlerHelloClass
{
	GObjectClass parent_class;
};

void hev_scgi_handler_hello_reg_type(GTypeModule *module);
GType hev_scgi_handler_hello_get_type(void);

G_END_DECLS

#endif /* __HEV_SCGI_HANDLER_HELLO_H__ */

