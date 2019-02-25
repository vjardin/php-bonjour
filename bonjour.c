/*
   +----------------------------------------------------------------------+
   | PHP Version 7                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2019 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Vincent JARDIN                                               |
   +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_bonjour.h"

#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE()  \
        ZEND_PARSE_PARAMETERS_START(0, 0) \
        ZEND_PARSE_PARAMETERS_END()
#endif /* ZEND_PARSE_PARAMETERS_NONE */

/* {{{ void bonjour_test1()
 */
PHP_FUNCTION(bonjour_test1)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_printf("The extension %s is loaded and working!\r\n", "bonjour");
}
/* }}} */

/* {{{ string bonjour_test2( [ string $var ] )
 */
PHP_FUNCTION(bonjour_test2)
{
	char *var = "World";
	size_t var_len = sizeof("World") - 1;
	zend_string *retval;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(var, var_len)
	ZEND_PARSE_PARAMETERS_END();

	retval = strpprintf(0, "Hello %s", var);

	RETURN_STR(retval);
}
/* }}}*/

/* {{{ string bonjour_array1( array $arg1, array $arg2 )
 */
PHP_FUNCTION(bonjour_array1)
{
	zval *arg1, *arg2;
	zend_string *key;
	zval *value;
	zend_long h;
	char str[BUFSIZ] = "";

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(arg1)
		Z_PARAM_ARRAY(arg2)
	ZEND_PARSE_PARAMETERS_END();

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(arg1), h, key, value) {
		if (Z_TYPE_P(value) != IS_STRING) {
			php_printf("%s:%d key[%d]=%s value=Invalid String\n", __func__, __LINE__, (int)h, key ? ZSTR_VAL(key): "NULL");
			continue;
		}
		php_printf("%s:%d key[%d]=%s value=%s\n", __func__, __LINE__, (int)h, key ? ZSTR_VAL(key): "NULL", Z_STRVAL_P(value));
		ZVAL_DEREF(value);
		strcat(str, Z_STRVAL_P(value)); /* TODO use strlcat() */
	} ZEND_HASH_FOREACH_END();

	ZVAL_NEW_STR(return_value, zend_string_init(str, strlen(str)+1, 0));

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(arg2), h, key, value) {
		if (Z_TYPE_P(value) != IS_STRING) {
			php_printf("%s:%d key[%d]=%s value=Invalid String\n", __func__, __LINE__, (int)h, key ? ZSTR_VAL(key): "NULL");
			continue;
		}
		php_printf("%s:%d key[%d]=%s value=%s\n", __func__, __LINE__, (int)h, key ? ZSTR_VAL(key): "NULL", Z_STRVAL_P(value));
		ZVAL_DEREF(value);
		strcat(str, Z_STRVAL_P(value)); /* TODO use strlcat() */
	} ZEND_HASH_FOREACH_END();

	if (strlen(str) <= 1) {
		RETURN_EMPTY_STRING();
		return;
	}

	ZVAL_NEW_STR(return_value, zend_string_init(str, strlen(str)+1, 0));
	return;
}
/* }}}*/

/* {{{ array bonjour_array2( array $arg1, string $arg2[, bool $associative] )
 */
PHP_FUNCTION(bonjour_array2)
{
	zval *arg1;
	zend_string *arg2;
	zend_bool associative = 0;

	zend_string *key;
	zval *value;
	zend_long h;

	char str_value[BUFSIZ];

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_ARRAY(arg1)
		Z_PARAM_STR(arg2)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(associative)
	ZEND_PARSE_PARAMETERS_END();

	array_init(return_value); /* zval *return_value */

	if (associative) {
		char str_key[BUFSIZ];
		int i = 0;
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(arg1), h, key, value) {
			if (Z_TYPE_P(value) != IS_STRING)
				continue;
			snprintf(str_key, sizeof(str_key), "%s-%d", ZSTR_VAL(arg2), i);
			snprintf(str_value, sizeof(str_value), "%s-%d", Z_STRVAL_P(value), i);
			ZVAL_DEREF(value);
			/* return_value[str_key] = str_value; */
			add_assoc_stringl(return_value, str_key, str_value, strlen(str_value));
			i++;
		} ZEND_HASH_FOREACH_END();
	} else {
		int i = 0;
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(arg1), h, key, value) {
			if (Z_TYPE_P(value) != IS_STRING)
				continue;
			snprintf(str_value, sizeof(str_value), "%s-%d", Z_STRVAL_P(value), i);
			ZVAL_DEREF(value);
			/* return_value[i] = str_value; */
			add_index_stringl(return_value, i, str_value, strlen(str_value));
			i++;
		} ZEND_HASH_FOREACH_END();
	}

	return;
}
/* }}}*/

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(bonjour)
{
#if defined(ZTS) && defined(COMPILE_DL_BONJOUR)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(bonjour)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "bonjour support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(arginfo_bonjour_test1, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_bonjour_test2, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_bonjour_array1_return, IS_STRING, 0)
	ZEND_ARG_ARRAY_INFO(0, arg1, 0)
	ZEND_ARG_ARRAY_INFO(0, arg2, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_bonjour_array2_return, IS_ARRAY, 0)
	ZEND_ARG_ARRAY_INFO(0, arg1, 0)
	ZEND_ARG_TYPE_INFO(0, arg2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, associative, _IS_BOOL, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ bonjour_functions[]
 */
static const zend_function_entry bonjour_functions[] = {
	PHP_FE(bonjour_test1,		arginfo_bonjour_test1)
	PHP_FE(bonjour_test2,		arginfo_bonjour_test2)
	PHP_FE(bonjour_array1,		arginfo_bonjour_array1_return)
	PHP_FE(bonjour_array2,		arginfo_bonjour_array2_return)
	PHP_FE_END
};
/* }}} */

/* {{{ bonjour_module_entry
 */
zend_module_entry bonjour_module_entry = {
	STANDARD_MODULE_HEADER,
	"bonjour",					/* Extension name */
	bonjour_functions,			/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(bonjour),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(bonjour),			/* PHP_MINFO - Module info */
	PHP_BONJOUR_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_BONJOUR
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(bonjour)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
