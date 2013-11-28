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

/* $Id: tpl.c 321634 2012-01-01 13:15:04Z felipe $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/php_var.h"

#include "pacl.h"
#include "acl.h"
#include "yaf/php_yaf.h"
#include "yaf/yaf_config.c"

#include "phalcon/php_phalcon.h"
#include "phalcon/acl/adapter/memory.h"
#include "phalcon/acl/role.h"
#include "phalcon/acl/resource.h"

zend_class_entry *php_acl_ce;

/** {{{ ARG_INFO */
ZEND_BEGIN_ARG_INFO_EX(php_acl_construct_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, config_file)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(php_acl_check_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, role)
	ZEND_ARG_INFO(0, res_name)
	ZEND_ARG_INFO(0, action)
ZEND_END_ARG_INFO();
/* }}} */


static void set_default_action(zval *acl_obj) {
	zval *vRole;
	MAKE_STD_ZVAL(vRole);
	ZVAL_LONG(vRole, 0);

	zval *function;
	MAKE_STD_ZVAL(function);
	ZVAL_STRING(function, "setDefaultAction", 1);
	zval **call_params[1];
	zval *ret = 0;
	call_params[0] = &vRole;
    if (SUCCESS != call_user_function_ex(CG(function_table), &acl_obj, function, &ret, 1, call_params, 0, NULL TSRMLS_CC)) {
		//
	}
	if (ret) {
		zval_ptr_dtor(&ret);
	}
	zval_ptr_dtor(&function);
	zval_ptr_dtor(&vRole);
}

static void add_role(zval *acl_obj, const char *role_name) {
	zval *vRole;
	MAKE_STD_ZVAL(vRole);
	ZVAL_STRING(vRole, role_name, 1);

	// addRole
	zval *function;
	MAKE_STD_ZVAL(function);
	ZVAL_STRING(function, "addRole", 1);
	zval **call_params[1];
	zval *ret = 0;
	call_params[0] = &vRole;
    if (SUCCESS != call_user_function_ex(CG(function_table), &acl_obj, function, &ret, 1, call_params, 0, NULL TSRMLS_CC)) {
		//
	}
	if (ret) {
		zval_ptr_dtor(&ret);
	}
	zval_ptr_dtor(&function);
	zval_ptr_dtor(&vRole);
}

static void add_resource(zval *acl_obj, const char *res_name, const char *action) {
	zval *vRes;
	MAKE_STD_ZVAL(vRes);
	ZVAL_STRING(vRes, res_name, 1);
	zval *vAction;
	MAKE_STD_ZVAL(vAction);
	ZVAL_STRING(vAction, action, 1);

	// addResource
	zval *function;
	MAKE_STD_ZVAL(function);
	ZVAL_STRING(function, "addResource", 1);
	zval **call_params[2];
	zval *ret = 0;
	call_params[0] = &vRes;
	call_params[1] = &vAction;
    if (SUCCESS != call_user_function_ex(CG(function_table), &acl_obj, function, &ret, 2, call_params, 0, NULL TSRMLS_CC)) {
		//
	}
	if (ret) {
		zval_ptr_dtor(&ret);
	}
	zval_ptr_dtor(&function);
	zval_ptr_dtor(&vRes);
	zval_ptr_dtor(&vAction);
}

static void add_auth(zval *acl_obj, int isAllow, const char *role_name, const char *res_name, const char *action) {
	zval *vRole;
	MAKE_STD_ZVAL(vRole);
	ZVAL_STRING(vRole, role_name, 1);
	zval *vRes;
	MAKE_STD_ZVAL(vRes);
	ZVAL_STRING(vRes, res_name, 1);
	zval *vAction;
	MAKE_STD_ZVAL(vAction);
	ZVAL_STRING(vAction, action, 1);

	// addResource
	zval *function;
	MAKE_STD_ZVAL(function);
	if (isAllow)
		ZVAL_STRING(function, "allow", 1);
	else
		ZVAL_STRING(function, "deny", 1);
	zval **call_params[3];
	zval *ret = 0;
	call_params[0] = &vRole;
	call_params[1] = &vRes;
	call_params[2] = &vAction;
    if (SUCCESS != call_user_function_ex(CG(function_table), &acl_obj, function, &ret, 3, call_params, 0, NULL TSRMLS_CC)) {
		//
	}
	if (ret) {
		zval_ptr_dtor(&ret);
	}
	zval_ptr_dtor(&function);
	zval_ptr_dtor(&vRes);
	zval_ptr_dtor(&vRole);
	zval_ptr_dtor(&vAction);
}

static zval* is_allow(zval *acl_obj, zval *vRole, zval *vRes, zval *vAction) {
	zval *function;
	MAKE_STD_ZVAL(function);
	ZVAL_STRING(function, "isAllowed", 1);
	zval **call_params[3];
	zval *ret = 0;
	call_params[0] = &vRole;
	call_params[1] = &vRes;
	call_params[2] = &vAction;
    if (SUCCESS != call_user_function_ex(CG(function_table), &acl_obj, function, &ret, 3, call_params, 0, NULL TSRMLS_CC)) {
		//
	}
	zval_ptr_dtor(&function);
	return ret;
}

static zval* get_roles(zval *acl_obj) {
	zval *function;
	MAKE_STD_ZVAL(function);
	ZVAL_STRING(function, "getRoles", 1);
	zval *ret = 0;
    if (SUCCESS != call_user_function_ex(CG(function_table), &acl_obj, function, &ret, 0, NULL, 0, NULL TSRMLS_CC)) {
		//
	}
	zval_ptr_dtor(&function);
	return ret;
}

static void ini_init(zval *acl_obj, zval *cfile) {
	zval *zconfig;

	zconfig = yaf_config_instance(NULL, cfile, NULL TSRMLS_CC);
	if  (zconfig == NULL || Z_TYPE_P(zconfig) != IS_OBJECT) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "parse error");
		return;
	}

	HashTable 	*conf = HASH_OF(zend_read_property(yaf_config_ce, zconfig, ZEND_STRL(YAF_CONFIG_PROPERT_NAME), 1 TSRMLS_CC));
	HashPosition pos;
	zval **entry;
	char *var_name;
	ulong num_key;
	uint var_name_len;
	for(zend_hash_internal_pointer_reset_ex(conf, &pos);
			zend_hash_get_current_data_ex(conf, (void **)&entry, &pos) == SUCCESS;
			zend_hash_move_forward_ex(conf, &pos)) {
		if (zend_hash_get_current_key_ex(conf, &var_name, &var_name_len, &num_key, 0, &pos) != HASH_KEY_IS_STRING) {
			continue;
		}
		if (IS_ARRAY != Z_TYPE_PP(entry)) {
			continue;
		}

		add_role(acl_obj, var_name);

		zval **ppzval;
		HashPosition pos2;
		zval **entry2;
		char *var_name2;
		ulong num_key2;
		uint var_name_len2;
		HashTable *conf2;
		if (zend_hash_find(Z_ARRVAL_PP(entry), ZEND_STRS("allow"), (void **)&ppzval) == SUCCESS
				&& Z_TYPE_PP(ppzval) == IS_ARRAY) {
			conf2 = Z_ARRVAL_PP(ppzval);
			for(zend_hash_internal_pointer_reset_ex(conf2, &pos2);
					zend_hash_get_current_data_ex(conf2, (void **)&entry2, &pos2) == SUCCESS;
					zend_hash_move_forward_ex(conf2, &pos2)) {
				if (Z_TYPE_PP(entry2) == IS_STRING) {
					if (Z_STRLEN_PP(entry2) == 1 && Z_STRVAL_PP(entry2)[0] == '*') { // '*' <=> '*.*'
						add_resource(acl_obj, "*", "*");
						add_auth(acl_obj, 1, var_name, "*", "*");
					} else {
						char *s = estrndup(Z_STRVAL_PP(entry2), Z_STRLEN_PP(entry2));
						char *p = strchr(s, '.');
						if (NULL != p) {
							*p = '\0';
							/*if (!(strlen(s) == 1 && *s == '*'))*/
								add_resource(acl_obj, s, ++p);
							add_auth(acl_obj, 1, var_name, s, p);
						}
						efree(s);
					}
				}
			}
		}
		if (zend_hash_find(Z_ARRVAL_PP(entry), ZEND_STRS("deny"), (void **)&ppzval) == SUCCESS
				&& Z_TYPE_PP(ppzval) == IS_ARRAY) {
			conf2 = Z_ARRVAL_PP(ppzval);
			for(zend_hash_internal_pointer_reset_ex(conf2, &pos2);
					zend_hash_get_current_data_ex(conf2, (void **)&entry2, &pos2) == SUCCESS;
					zend_hash_move_forward_ex(conf2, &pos2)) {
				if (Z_TYPE_PP(entry2) == IS_STRING) {
					if (Z_STRLEN_PP(entry2) == 1 && Z_STRVAL_PP(entry2)[0] == '*') { // '*' <=> '*.*'
						add_resource(acl_obj, "*", "*");
						add_auth(acl_obj, 1, var_name, "*", "*");
					} else {
						char *s = estrndup(Z_STRVAL_PP(entry2), Z_STRLEN_PP(entry2));
						char *p = strchr(s, '.');
						if (NULL != p) {
							*p = '\0';
							/*if (!(strlen(s) == 1 && *s == '*'))*/
								add_resource(acl_obj, s, ++p);
							add_auth(acl_obj, 0, var_name, s, p);
						}
						efree(s);
					}
				}
			}
		}
	}

	zval_ptr_dtor(&zconfig);
}

/** {{{ 
*/
PHP_METHOD(php_acl, __construct) {
	zval *cfile;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &cfile) == FAILURE) {
		WRONG_PARAM_COUNT;
		return;
	}

	zval *acl_obj;
	MAKE_STD_ZVAL(acl_obj);
	object_init_ex(acl_obj, phalcon_acl_adapter_memory_ce);

	set_default_action(acl_obj);

	if (Z_TYPE_P(cfile) == IS_STRING && (strncasecmp(Z_STRVAL_P(cfile) + Z_STRLEN_P(cfile) - 3, "ini", 3) == 0)) {
		ini_init(acl_obj, cfile);
	}

	/*php_var_dump(&acl_obj, 0);*/
	zend_update_property(php_acl_ce, getThis(), ZEND_STRL("_acl_obj"), acl_obj TSRMLS_CC);
	zval_ptr_dtor(&acl_obj);
}
/* }}} */

/** {{{ 
*/
PHP_METHOD(php_acl, check) {
	zval *acl_obj, *vRole, *vRes, *vAction;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &vRole, &vRes, &vAction) == FAILURE) {
		WRONG_PARAM_COUNT;
		return;
	}

	acl_obj = zend_read_property(php_acl_ce, getThis(), ZEND_STRL("_acl_obj"), 0 TSRMLS_CC);

	zval *z = is_allow(acl_obj, vRole, vRes, vAction);
	RETURN_ZVAL(z, 0, 1);
}
/* }}} */

/** {{{ 
*/
PHP_METHOD(php_acl, getRoles) {
	zval *acl_obj;

	acl_obj = zend_read_property(php_acl_ce, getThis(), ZEND_STRL("_acl_obj"), 0 TSRMLS_CC);

	zval* z = get_roles(acl_obj);
	RETURN_ZVAL(z, 0, 1);
}
/* }}} */

/** {{{ php_acl_methods
*/
zend_function_entry php_acl_methods[] = {
	PHP_ME(php_acl, __construct, php_acl_construct_arginfo, ZEND_ACC_CTOR|ZEND_ACC_FINAL|ZEND_ACC_PUBLIC)
	PHP_ME(php_acl, check, php_acl_check_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(php_acl, getRoles, NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
STU_MINIT_FUNCTION(acl)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "Pacl", php_acl_methods);
	php_acl_ce = zend_register_internal_class_ex(&ce, NULL, NULL TSRMLS_CC);

	zend_declare_property_null(php_acl_ce, ZEND_STRL("_acl_obj"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
STU_MSHUTDOWN_FUNCTION(acl)
{
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
