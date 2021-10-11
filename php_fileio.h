/* file io extension for PHP */

#ifndef PHP_FILEIO_H
# define PHP_FILEIO_H

#include <uv.h>
#include <zend_modules.h>
#ifdef ZTS
#include "TSRM.h"
#endif

//typedef struct _zend_fileio_globals {
ZEND_BEGIN_MODULE_GLOBALS(fileio)
    uv_loop_t *loop;
    zend_class_entry *promise_class;
    zend_class_entry *promise__status_enum;
//} zend_fileio_globals;
ZEND_END_MODULE_GLOBALS(fileio)

//zend_fileio_globals fileio_globals = {NULL};
//extern zend_fileio_globals v8js_globals;

ZEND_EXTERN_MODULE_GLOBALS(fileio)

extern zend_module_entry fileio_module_entry;
#define phpext_fileio_ptr &fileio_module_entry

# define PHP_FILEIO_VERSION "0.1.0-beta"

#ifdef ZTS
#define FILE_IO_GLOBAL(v) TSRMG(fileio_globals_id, zend_fileio_globals *, v)
#else
#define FILE_IO_GLOBAL(v) fileio_globals.v
#endif




# if defined(ZTS) && defined(COMPILE_DL_FILEIO)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif    /* PHP_FILEIO_H */
