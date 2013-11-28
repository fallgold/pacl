dnl $Id$
dnl config.m4 for extension pacl

PHP_ARG_ENABLE(pacl, whether to enable pacl support,
Make sure that the comment is aligned:
[  --enable-pacl           Enable pacl support])

if test "$PHP_PACL" != "no"; then
  PHP_SUBST(PACL_SHARED_LIBADD)

  AC_MSG_CHECKING([PHP version])

  tmp_version=$PHP_VERSION
  if test -z "$tmp_version"; then
    if test -z "$PHP_CONFIG"; then
      AC_MSG_ERROR([php-config not found])
    fi
    php_version=`$PHP_CONFIG --version 2>/dev/null|head -n 1|sed -e 's#\([0-9]\.[0-9]*\.[0-9]*\)\(.*\)#\1#'`
  else
    php_version=`echo "$tmp_version"|sed -e 's#\([0-9]\.[0-9]*\.[0-9]*\)\(.*\)#\1#'`
  fi

  if test -z "$php_version"; then
    AC_MSG_ERROR([failed to detect PHP version, please report])
  fi

  ac_IFS=$IFS
  IFS="."
  set $php_version
  IFS=$ac_IFS
  yaf_php_version=`expr [$]1 \* 1000000 + [$]2 \* 1000 + [$]3`

  if test "$yaf_php_version" -lt "5003000"; then
    AC_MSG_ERROR([You need at least PHP 5.3.0 to be able to use this version of Pacl. PHP $php_version found])
  else
    AC_MSG_RESULT([$php_version, ok])
  fi
  if test "$yaf_php_version" -lt "5004000"; then
	  tmp=1
  elif test "$yaf_php_version" -lt "5005000"; then
	  tmp=1
  else
	AC_MSG_RESULT([zend engine is too new, it may not work])
  fi
  PHP_ADD_INCLUDE("$zend")
  PHP_ADD_INCLUDE("phalcon")

	phalcon_sources="phalcon/phalcon.c \
phalcon/kernel/main.c \
phalcon/kernel/memory.c \
phalcon/kernel/object.c \
phalcon/kernel/fcall.c \
phalcon/kernel/hash.c \
phalcon/kernel/array.c \
phalcon/kernel/string.c \
phalcon/kernel/operators.c \
phalcon/kernel/concat.c \
phalcon/kernel/exception.c \
phalcon/kernel/alternative/fcall.c \
phalcon/acl/resource.c \
phalcon/acl/resourceinterface.c \
phalcon/acl/adapter/memory.c \
phalcon/acl/exception.c \
phalcon/acl/role.c \
phalcon/acl/adapterinterface.c \
phalcon/acl/adapter.c \
phalcon/acl/roleinterface.c"

  PHP_NEW_EXTENSION(pacl, pacl.c acl.c $phalcon_sources, $ext_shared)
fi
