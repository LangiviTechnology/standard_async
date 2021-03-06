//
// Created by user on 25.10.21.
//

#ifndef FILEIO_HTTP_SERVER_H
#define FILEIO_HTTP_SERVER_H
#include "../common/struct.h"
#include "http_client_type.h"
#include "pointer_to_free.h"


ADD_STRUCT(http_client_stream, http_client_type);

typedef struct {
    php_stream *server_stream;
    int server_fd;
    ADD_HANDLE_TO_STRUCT(http_client_stream)
    uint64_t clients_count;
    uv_poll_t *server_handle;
    uv_cb_type on_data;
    uv_cb_type on_connect;
    uv_cb_type on_disconnect;
    uv_cb_type on_error;
    uv_buf_t write_buf;
    unsigned int active_handles;
} http_server_type;

typedef struct {
    unsigned long long id;
    bool is_read;
    bool is_written;
} request_info;

typedef struct ht_event_handle_item {
    zend_long cur_id;
    zend_object *this;
    void *handle_data;
    request_info req_info;
} ht_event_handle_item;




#define SERVER_ID "#"
#define CLOSABLE "##"

#define GET_HTTP_SERV_ID()     zval * rv; \
rv = zend_read_property(MODULE_GL(http_server_class), Z_OBJ_P(ZEND_THIS), PROP("#"), 0, NULL); \
zend_long cur_id = Z_LVAL_P(rv);


#define GET_HTTP_SERV_ID_FROM_RES(RESPONSE_OBJECT)     zval * rv; \
RESPONSE_OBJECT = responseObj_from_zend_obj(Z_OBJ_P(ZEND_THIS)); \
rv = zend_read_property((RESPONSE_OBJECT)->server->ce, (RESPONSE_OBJECT)->server, PROP("#"), 0, NULL); \
zend_long cur_id = Z_LVAL_P(rv);

#define GET_HTTP_SERV_ID_FROM_EVENT_HANDLE()  zend_long cur_id = ((ht_event_handle_item *)handle->data)->cur_id;

#define PROP(string)  string, sizeof(string) - 1

static void on_listen_server_for_clients(uv_poll_t *handle, int status, int events);

static void on_ready_to_write(uv_poll_t *handle, http_client_stream_id_item_t *client, int status, int events);

static void on_ready_to_read(uv_poll_t *handle, http_client_stream_id_item_t *client, int status, int events);

static void on_listen_client_event(uv_poll_t *handle, int status, int events);

static bool on_ready_to_disconnect(uv_poll_t *handle, http_client_stream_id_item_t *client, int status, int events);

CREATE_HANDLE_LIST_HEADERS(http_client_stream, http_client_type);
void add_pntr(pntrs_to_free *pointers_store, void *pointer);
extern http_server_type http_php_servers[10];

#endif //FILEIO_SERVER_H
