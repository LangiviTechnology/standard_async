//
// Created by admin on 30.10.2021.
//

#ifndef FILEIO_HELPERS_H
#define FILEIO_HELPERS_H
char *create_host(const char *host, size_t host_len, zend_long port, size_t *str_len);
void get_meta_data(php_stream *stream);
#endif //FILEIO_HELPERS_H