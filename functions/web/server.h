//
// Created by user on 25.10.21.
//

#ifndef FILEIO_SERVER_H
#define FILEIO_SERVER_H

typedef struct {
    php_stream *current_stream;
    int current_fd;
    uint64_t id;
} client_type;

typedef struct server_type {
    php_stream *server_stream;
    int server_fd;
    client_type *client_stream;
    uint64_t clients_count;
    uv_cb_type on_data;
    uv_cb_type on_connect;
    uv_cb_type on_disconnect;
    uv_cb_type on_error;
    uv_buf_t write_buf;
} server_type;

typedef struct event_handle_item {
    zend_long cur_id;
    zend_object *this;
    void *handle_data;
} event_handle_item;

#define GET_SERV_ID()     zval * rv; \
rv = zend_read_property(FILE_IO_GLOBAL(server_class), Z_OBJ_P(ZEND_THIS),"#",sizeof("#")-1, 0, NULL); \
zend_long cur_id = Z_LVAL_P(rv);

#define GET_SERV_ID_FROM_EVENT_HANDLE()  zend_long cur_id;\
cur_id = ((event_handle_item *)handle->data)->cur_id;
#define PROP(string)  string, sizeof(string) - 1
#endif //FILEIO_SERVER_H
