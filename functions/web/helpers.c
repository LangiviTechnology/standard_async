//
// Created by admin on 29.10.2021.
//

#include <string.h>
#include <zend_API.h>
#include <php.h>
#include <uv.h>
#include "../../constants.h"
#include "helpers.h"
#include "http_server.h"

#define LOG_TAG "TCP SERVER"

char *create_host(const char *host, size_t host_len, zend_long port, size_t *str_len) {
    if (host == NULL) {
        host = "0.0.0.0";
        host_len = strlen(host);
    }
    char port_str[10]={0};
    size_t final_len=0;
    sprintf(port_str, "%ld", port);
    final_len = host_len + strlen(port_str) + 2;
    char * host_ = emalloc(sizeof(char) * final_len);
    memset(host_, 0, final_len);
    strncpy(host_, host, host_len);
    strncat(host_, ":", 1);
    strncat(host_, port_str, strlen(port_str));
    * str_len = final_len;
    return host_;
}

void parse_fci_error(long error, const char *func_name) {
    LOG("%s - ", func_name);
    switch (error) {
        case 0:
            LOG("Function run\n");
            break;
        case -1:
            LOG("Error in function execution\n");
            break;
        case -2:
            LOG("Function not initialized\n");
            break;
    }
    puts("");
    puts("");
}

void parse_uv_event(int event, int status) {
    switch (event) {
        case UV_DISCONNECT:
            LOG("UV_DISCONNECT event, status -  %d \n", status);
            break;
//        case UV_WRITABLE:
//            LOG("UV_WRITABLE event, status -  %d \n", status);
//            break;
        case UV_READABLE:
            LOG("UV_READABLE event, status -  %d \n", status);
            break;
//        default:
//            LOG("%d event, status -  %d \n", event, status);
//            break;
    }
}

void get_meta_data(php_stream *stream) {
    zval *return_value;
    return_value = emalloc(sizeof(zval));
    array_init(return_value);

    if (!php_stream_populate_meta_data(stream, return_value)) {
        add_assoc_bool(return_value, "timed_out", 0);
        add_assoc_bool(return_value, "blocked", 1);
        add_assoc_bool(return_value, "eof", php_stream_eof(stream));
    }

    if (!Z_ISUNDEF(stream->wrapperdata)) {
        Z_ADDREF_P(&stream->wrapperdata);
        add_assoc_zval(return_value, "wrapper_data", &stream->wrapperdata);
    }
    if (stream->wrapper) {
        add_assoc_string(return_value, "wrapper_type", (char *) stream->wrapper->wops->label);
    }
    add_assoc_string(return_value, "stream_type", (char *) stream->ops->label);

    add_assoc_string(return_value, "mode", stream->mode);

#if 0    /* TODO: needs updating for new filter API */
    if (stream->filterhead) {
        php_stream_filter *filter;

        MAKE_STD_ZVAL(newval);
        array_init(newval);

        for (filter = stream->filterhead; filter != NULL; filter = filter->next) {
            add_next_index_string(newval, (char *)filter->fops->label);
        }

        add_assoc_zval(return_value, "filters", newval);
    }
#endif

    add_assoc_long(return_value, "unread_bytes", stream->writepos - stream->readpos);
    add_assoc_long(return_value, "position", stream->position);

    add_assoc_bool(return_value, "seekable", (stream->ops->seek) && (stream->flags & PHP_STREAM_FLAG_NO_SEEK) == 0);
    if (stream->orig_path) {
        add_assoc_string(return_value, "uri", stream->orig_path);
    }
    zend_print_zval_r(return_value, 0);
    efree(return_value);
}

int cast_to_fd(php_stream *stream, zend_result *result) {
    int fd = -1;
    (*result) = _php_stream_cast(stream, PHP_STREAM_AS_FD_FOR_SELECT | PHP_STREAM_CAST_INTERNAL, (void *) &fd, 1);
    if (*result == FAILURE) {
        php_error_docref(NULL, E_ERROR, "Could not get FD of stream");
    }
    return fd;
}

int set_non_blocking(php_stream *stream) {
    return stream->ops->set_option(stream, PHP_STREAM_OPTION_BLOCKING, 0, NULL);
}

bool fill_super_global(const unsigned char name, zval *value) {
    zend_string *numb_var;
    switch (name) {
        case TRACK_VARS_GET:
            numb_var = zend_string_init_interned("_GET", sizeof("_GET") - 1, 1);
            break;
        case TRACK_VARS_POST:
            numb_var = zend_string_init_interned("_POST", sizeof("_POST") - 1, 1);
            break;
        case TRACK_VARS_COOKIE:
            numb_var = zend_string_init_interned("_COOKIE", sizeof("_COOKIE") - 1, 1);
            break;
        default:
            return false;
    }

    ZVAL_COPY_VALUE(&PG(http_globals)[name], value);
    zend_hash_update(&EG(symbol_table), numb_var, &PG(http_globals)[name]);
    Z_ADDREF(PG(http_globals)[name]);
    return true;
}


void alloc_handles(uv_poll_t *cli_handle, http_client_type *que_cli_handle ) {
    cli_handle = emalloc(sizeof(uv_poll_t));
    que_cli_handle = emalloc(sizeof(http_client_type));
    memset(que_cli_handle, 0, sizeof(http_client_type));

};