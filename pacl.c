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
#define STUPY_VERSION "1.0.0"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/file.h"

#include "pacl.h"
#include "phalcon/php_phalcon.h"

/* add module header here */
#include "acl.h"
#include "yaf/php_yaf.h"
#include "yaf/yaf.c"

ZEND_DECLARE_MODULE_GLOBALS(stupy);
ZEND_DECLARE_MODULE_GLOBALS(phalcon);

const zend_function_entry stupy_functions[] = {
	/* add module function here */
	PHP_FE_END
};

zend_module_entry stupy_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"pacl",
	stupy_functions,
	PHP_MINIT(stupy),
	PHP_MSHUTDOWN(stupy),
	PHP_RINIT(stupy),	
	PHP_RSHUTDOWN(stupy),
	PHP_MINFO(stupy),
#if ZEND_MODULE_API_NO >= 20010901
	STUPY_VERSION,
#endif
	PHP_MODULE_GLOBALS(stupy),
	PHP_GINIT(stupy),
	PHP_GSHUTDOWN(stupy),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};


#ifdef COMPILE_DL_PACL
BEGIN_EXTERN_C()
ZEND_GET_MODULE(stupy)
END_EXTERN_C()
#endif

PHP_MINIT_FUNCTION(stupy)
{
	/* add module startup here */
	STU_MINIT(acl);

	// yaf config init
	STU_MINIT(config);
	STU_MINIT(config_ini);

	// phalcon acl init
	PHALCON_INIT(Phalcon_Acl_ResourceInterface);
	PHALCON_INIT(Phalcon_Acl_RoleInterface);
	PHALCON_INIT(Phalcon_Acl_AdapterInterface);
	PHALCON_INIT(Phalcon_Acl_Adapter);
	PHALCON_INIT(Phalcon_Acl_Role);
	PHALCON_INIT(Phalcon_Acl_Resource);
	PHALCON_INIT(Phalcon_Acl_Adapter_Memory);
	PHALCON_INIT(Phalcon_Acl_Exception);

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(stupy)
{
	/* add module shutdown here */
	STU_MSHUTDOWN(acl);

	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}

PHP_RINIT_FUNCTION(stupy)
{
	php_phalcon_init_globals(PHALCON_VGLOBAL TSRMLS_CC);

	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(stupy)
{
	if (PHALCON_GLOBAL(start_memory) != NULL) {
		phalcon_clean_restore_stack_shutdown(TSRMLS_C);
	}

	if (PHALCON_GLOBAL(function_cache) != NULL) {
		zend_hash_destroy(PHALCON_GLOBAL(function_cache));
		FREE_HASHTABLE(PHALCON_GLOBAL(function_cache));
		PHALCON_GLOBAL(function_cache) = NULL;
	}

	return SUCCESS;
}

PHP_MINFO_FUNCTION(stupy)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "Pacl support", "enabled");
    php_info_print_table_row(2, "Version", STUPY_VERSION);
	php_info_print_table_end();
}

PHP_GINIT_FUNCTION(stupy)
{
	zend_phalcon_globals *phalcon_cc = PHALCON_VGLOBAL;

	phalcon_memory_entry *start;

	php_phalcon_init_globals(phalcon_cc TSRMLS_CC);

	start = (phalcon_memory_entry *) pecalloc(1, sizeof(phalcon_memory_entry), 1);
/* pecalloc() will take care of these members
	start->pointer      = 0;
	start->hash_pointer = 0;
	start->prev = NULL;
	start->next = NULL;
*/
	start->addresses       = pecalloc(24, sizeof(zval*), 1);
	start->capacity        = 24;
	start->hash_addresses  = pecalloc(8, sizeof(zval*), 1);
	start->hash_capacity   = 8;

	phalcon_cc->start_memory = start;
}

PHP_GSHUTDOWN_FUNCTION(stupy)
{
	zend_phalcon_globals *phalcon_cc = PHALCON_VGLOBAL;

	assert(phalcon_cc->start_memory != NULL);

	pefree(phalcon_cc->start_memory->hash_addresses, 1);
	pefree(phalcon_cc->start_memory->addresses, 1);
	pefree(phalcon_cc->start_memory, 1);
	phalcon_cc->start_memory = NULL;
}


