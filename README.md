# php-bonjour
Demo of PHP module with shared code for Windows and Linux, thanks to `phpize` wth Linux or `phpize.bat` with Windows.

# start

It is a demo about the usage of [ext_skel.php](https://github.com/php/php-src/blob/master/ext/ext_skel.php).
This tool was signifcantly revamped and updates since PHP 7.3. It should be used to start a skeleton of PHP
extension that can run on both Linux (bonjour.so) and windows (php-bonjour.dll).

# step 1

```console
vj@p:~/d$ php ${PHPSRC}/ext/ext_skel.php --ext bonjour --author "Vincent JARDIN" --std --dir .
```

Tricks: you can get `ext_skel.php` and still phpize your code with PHP 7.2 assmuing you apply the following patch,

```c
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE()  \
        ZEND_PARSE_PARAMETERS_START(0, 0) \
        ZEND_PARSE_PARAMETERS_END()
#endif /* ZEND_PARSE_PARAMETERS_NONE */
```

This repository was built using this _step 1_.

# step 2 - Linux

```console
vj@p:~/php-bonjour$ apt-cache show php7.2 | grep Filename
Filename: pool/main/p/php7.2/php7.2_7.2.10-0ubuntu0.18.04.1_all.deb

vj@p:~/php-bonjour$ php -v
php -v
PHP 7.2.10-0ubuntu0.18.04.1 (cli) (built: Sep 13 2018 13:45:02) ( NTS )
Copyright (c) 1997-2018 The PHP Group

vj@p:~/php-bonjour$ which phpize
/usr/bin/phpize
vj@p:~/php-bonjour$ ls -la /usr/bin/phpize
lrwxrwxrwx 1 root root 24 Aug  1 00:55 /usr/bin/phpize -> /etc/alternatives/phpize
vj@p:~/php-bonjour$ ls -la /etc/alternatives/phpize
lrwxrwxrwx 1 root root 18 Aug  1 00:55 /etc/alternatives/phpize -> /usr/bin/phpize7.2

vj@p:~/php-bonjour$ phpize
Configuring for:
PHP Api Version:         20170718
Zend Module Api No:      20170718
Zend Extension Api No:   320170718
vj@p:~/php-bonjour$ ./configure
 ...
checking for PHP includes... -I/usr/include/php/20170718 -I/usr/include/php/20170718/main -I/usr/include/php/20170718/TSRM -I/usr/include/php/20170718/Zend -I/usr/include/php/20170718/ext -I/usr/include/php/20170718/ext/date/lib
checking for PHP extension directory... /usr/lib/php/20170718
checking for PHP installed headers prefix... /usr/include/php/20170718
checking if debug is enabled... no
 ...
vj@p:~/php-bonjour$ make
vj@p:~/php-bonjour$ make test
 ...
=====================================================================
TIME START 2019-01-01 21:09:50
=====================================================================
PASS Check if bonjour is loaded [tests/001.phpt] 
PASS bonjour_test1() Basic test [tests/002.phpt] 
PASS bonjour_test2() Basic test [tests/003.phpt] 
=====================================================================
TIME END 2019-01-01 21:09:51

=====================================================================
TEST RESULT SUMMARY
---------------------------------------------------------------------
Exts skipped    :    0
Exts tested     :   15
---------------------------------------------------------------------

Number of tests :    3                 3
Tests skipped   :    0 (  0.0%) --------
Tests warned    :    0 (  0.0%) (  0.0%)
Tests failed    :    0 (  0.0%) (  0.0%)
Expected fail   :    0 (  0.0%) (  0.0%)
Tests passed    :    3 (100.0%) (100.0%)
---------------------------------------------------------------------
Time taken      :    1 seconds
=====================================================================
 ...
```

GREAT ! You have built your first PHP extension,

```console
vj@p:~/php-bonjour$ ls -la modules/bonjour.so
-rwxrwxrwx 1 vjardin vjardin 53296 Jan  1 22:09 modules/bonjour.so
```

Let's use it:

```console
vj@p:~/php-bonjour$ php -d extension=modules/bonjour.so -m | head
[PHP Modules]
bonjour
calendar
Core
 ...
```

This `bonjour.so` extension includes 2 PHP functions:

```console
vj@p:~/php-bonjour$ grep '{{{' bonjour.c | grep test
/* {{{ void bonjour_test1()
/* {{{ string bonjour_test2( [ string $var ] )

vj@p:~/php-bonjour$ grep PHP_FUNCTION bonjour.c
PHP_FUNCTION(bonjour_test1)
PHP_FUNCTION(bonjour_test2)
```

it means,

```php
void bonjour_test1();
string bonjour_test2( [ string $var ];
```

Let's call them from the PHP's CLI,

```console
vj@p:~/php-bonjour$ php -d extension=modules/bonjour.so -a
Interactive mode enabled

php > bonj[tab]
php > bonjour_test[tab][tab]
bonjour_test1 bonjour_test2
php > bonjour_test1();
The extension bonjour is loaded and working!
php > echo bonjour_test2("la Terre");
Hello la Terre
php > quit
```

Like any shell, the PHP's CLI has a support for completion using tabulation `[TAB]`.

You can clean your folder using:

```console
vj@p:~/php-bonjour$ make clean
vj@p:~/php-bonjour$ phpize --clean
```

Then, you are ready to re-compile everything.
