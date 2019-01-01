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

vj@p:~/php-bonjour$ phpize
vj@p:~/php-bonjour$ ./configure
vj@p:~/php-bonjour$ make
vj@p:~/php-bonjour$ make test
```
