dnl config.m4 for extension bonjour

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(bonjour, for bonjour support,
dnl Make sure that the comment is aligned:
dnl [  --with-bonjour             Include bonjour support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(bonjour, whether to enable bonjour support,
dnl Make sure that the comment is aligned:
[  --enable-bonjour          Enable bonjour support], no)

if test "$PHP_BONJOUR" != "no"; then
  dnl Write more examples of tests here...

  dnl # get library FOO build options from pkg-config output
  dnl AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  dnl AC_MSG_CHECKING(for libfoo)
  dnl if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists foo; then
  dnl   if $PKG_CONFIG foo --atleast-version 1.2.3; then
  dnl     LIBFOO_CFLAGS=\`$PKG_CONFIG foo --cflags\`
  dnl     LIBFOO_LIBDIR=\`$PKG_CONFIG foo --libs\`
  dnl     LIBFOO_VERSON=\`$PKG_CONFIG foo --modversion\`
  dnl     AC_MSG_RESULT(from pkgconfig: version $LIBFOO_VERSON)
  dnl   else
  dnl     AC_MSG_ERROR(system libfoo is too old: version 1.2.3 required)
  dnl   fi
  dnl else
  dnl   AC_MSG_ERROR(pkg-config not found)
  dnl fi
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBDIR, BONJOUR_SHARED_LIBADD)
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)

  dnl # --with-bonjour -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/bonjour.h"  # you most likely want to change this
  dnl if test -r $PHP_BONJOUR/$SEARCH_FOR; then # path given as parameter
  dnl   BONJOUR_DIR=$PHP_BONJOUR
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for bonjour files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       BONJOUR_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$BONJOUR_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the bonjour distribution])
  dnl fi

  dnl # --with-bonjour -> add include path
  dnl PHP_ADD_INCLUDE($BONJOUR_DIR/include)

  dnl # --with-bonjour -> check for lib and symbol presence
  dnl LIBNAME=BONJOUR # you may want to change this
  dnl LIBSYMBOL=BONJOUR # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $BONJOUR_DIR/$PHP_LIBDIR, BONJOUR_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_BONJOURLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong bonjour lib version or lib not found])
  dnl ],[
  dnl   -L$BONJOUR_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(BONJOUR_SHARED_LIBADD)

  dnl # In case of no dependencies
  AC_DEFINE(HAVE_BONJOUR, 1, [ Have bonjour support ])

  PHP_NEW_EXTENSION(bonjour, bonjour.c, $ext_shared)
fi
