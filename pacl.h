/*
  +----------------------------------------------------------------------+
  | Stupy                                                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: weils <fallgold@gmail.com>                                   |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_STUPY_H
#define PHP_STUPY_H

extern zend_module_entry stupy_module_entry;
#define phpext_stupy_ptr &stupy_module_entry

#ifdef PHP_WIN32
#	define PHP_STUPY_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_STUPY_API __attribute__ ((visibility("default")))
#else
#	define PHP_STUPY_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define STU_MINIT_FUNCTION(module)   	ZEND_MINIT_FUNCTION(stu_##module)
#define STU_MSHUTDOWN_FUNCTION(module) 	ZEND_MSHUTDOWN_FUNCTION(stu_##module)
#define STU_MINIT(module)				ZEND_MINIT(stu_##module)(INIT_FUNC_ARGS_PASSTHRU)
#define STU_MSHUTDOWN(module)			ZEND_MSHUTDOWN(stu_##module)(SHUTDOWN_FUNC_ARGS_PASSTHRU)

ZEND_BEGIN_MODULE_GLOBALS(stupy)
	char *tmp; // 必须定义一个变量，否则初始化不调用GINIT
ZEND_END_MODULE_GLOBALS(stupy)

PHP_MINIT_FUNCTION(stupy);
PHP_MSHUTDOWN_FUNCTION(stupy);
PHP_RINIT_FUNCTION(stupy);
PHP_RSHUTDOWN_FUNCTION(stupy);
PHP_MINFO_FUNCTION(stupy);
PHP_GINIT_FUNCTION(stupy);
PHP_GSHUTDOWN_FUNCTION(stupy);

extern ZEND_DECLARE_MODULE_GLOBALS(stupy);

#ifdef ZTS
#define STUPY_G(v) TSRMG(stupy_globals_id, zend_stupy_globals *, v)
#else
#define STUPY_G(v) (stupy_globals.v)
#endif

#define STU_G(v) STUPY_G(v)

#endif


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
