# php-bonjour
Demo of PHP module with shared code for Windows and Linux, thanks to `phpize` wth Linux or `phpize.bat` with Windows.

# start

It is a demo about the usage of [ext_skel.php](https://github.com/php/php-src/blob/master/ext/ext_skel.php).
This tool was signifcantly revamped and updated since PHP 7.3. It should be used to start a skeleton of PHP
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

# step 3 - Windows

The following steps are run within a [ConEmu](https://chocolatey.org/packages/ConEmu) console that is provided by [Chocolatey.org](https://chocolatey.org/) packages for Windows.
I will not use any of the Visual Studio IDE, but only the command lines so you can script the following steps easily if needed.
Regarding [Visual Studio](https://visualstudio.microsoft.com/free-developer-offers/), only the C and system environment are needed in order to compile some PHP DLLs.
You need to get `nmake` and the C compiler `cl.exe` including the other tools from Microsoft.
Once Visual Studio is available, you can open a `ConEmu` tab with `SDK::VS 15.0 x64 tools prompt`.

The following steps are extracted from [Microsoft's PHP]( https://github.com/Microsoft/php-sdk-binary-tools/blob/master/README.md)'s README.
It will compile a fresh PHP (mine is PHP 7.4) from the main PHP git repository. Then, it will install a minimalist PHP interpretor into
the folder `c:\php`. Be careful, it make overwrite your existing PHP environment.

```dos
cmd$ cd C:\
C:\$ git clone https://github.com/Microsoft/php-sdk-binary-tools.git c:\php-sdk
C:\$ cd c:\php-sdk
C:\php-sdk$ phpsdk-vc15-x64.bat # Visual Studio 2017 / Windows 64 bits
C:\php-sdk$ phpsdk_buildtree phpbuild
C:\php-sdk\phpbuild\vc15\x64$ git clone https://github.com/php/php-src.git
C:\php-sdk\phpbuild\vc15\x64$ cd php-src
C:\php-sdk\phpbuild\vc15\x64\php-src$ phpsdk_deps --update --branch master
C:\php-sdk\phpbuild\vc15\x64\php-src$ buildconf
C:\php-sdk\phpbuild\vc15\x64\php-src$ configure --disable-all --enable-cli --enable-cgi
C:\php-sdk\phpbuild\vc15\x64\php-src$ nmake
C:\php-sdk\phpbuild\vc15\x64\php-src$ nmake install
```

Then, you can check your fresh PHP,

```dos
cmd$ cd c:\php
C:\php$ php -v
PHP 7.4.0-dev (cli) (built: Jan  1 2019 22:41:35) ( ZTS MSVC15 (Visual C++ 2017) x64 )
Copyright (c) 1997-2018 The PHP Group
Zend Engine v3.4.0-dev, Copyright (c) 1998-2018 Zend Technologies
```

Let's start with the usual Hello World example:

```dos
C:\php$ php -a
Interactive shell

<?php
echo "bonjour";
^Z[enter]
bonjour
C:\php$
```

Then, we need to check that we have the Windows `phpize.bat` tool that is mandatory
to build the PHP extension on Windows. We will not re-build a bonjour skeleton, but
we'll reuse the same code that the on that was build for Linux. It is all about code
sharing between both Linux and Windows.

Our PHP environment should be:
```dos
C:\php$ dir /W
[.]             [..]            [ext]           php-cgi.exe     php.exe         php7ts.dll      [SDK]

C:\php$ dir SDK /W
[.]          [..]         [build]      [include]    [lib]        phpize.bat   [script]
```

So, we have our `C:\php\SDK\phpize.bat` tool that can be used to generate the Makefile for Windows Visual Studio environment.
Other folders are important tool. `lib` provides the library to link with. `include` provides the C header files
that our extension (dll) will use during its compilation.

```dos
C:\php$ cd c:\php-sdk
C:\php$ git clone https://github.com/vjardin/php-bonjour.git
C:\php$ php-bonjour
C:\php\php-bonjour$ c:\php\SDK\phpize
C:\php\php-bonjour$ configure --enable-bonjour
 ...
Enabled extensions:
----------------------
| Extension | Mode   |
----------------------
| bonjour   | shared |
----------------------

------------------------------------------------
|                   |                          |
------------------------------------------------
| Build type        | Release                  |
| Thread Safety     | Yes                      |
| Compiler          | MSVC15 (Visual C++ 2017) |
| Architecture      | x64                      |
| Optimization      | PGO disabled             |
| Native intrinsics | SSE2                     |
| Static analyzer   | disabled                 |
------------------------------------------------
 ...
C:\php\php-bonjour$ nmake
C:\php\php-bonjour$ nmake test
=====================================================================
TIME START 2019-01-01 22:29:40
=====================================================================
PASS Check if bonjour is loaded [C:\php-sdk\php-bonjour\tests\001.phpt]
PASS bonjour_test1() Basic test [C:\php-sdk\php-bonjour\tests\002.phpt]
PASS bonjour_test2() Basic test [C:\php-sdk\php-bonjour\tests\003.phpt]
=====================================================================
TIME END 2019-01-01 22:29:40

=====================================================================
TEST RESULT SUMMARY
---------------------------------------------------------------------
Exts skipped    :    0
Exts tested     :    8
---------------------------------------------------------------------

Number of tests :    3                 3
Tests skipped   :    0 (  0.0%) --------
Tests warned    :    0 (  0.0%) (  0.0%)
Tests failed    :    0 (  0.0%) (  0.0%)
Expected fail   :    0 (  0.0%) (  0.0%)
Tests passed    :    3 (100.0%) (100.0%)
---------------------------------------------------------------------
Time taken      :    0 seconds
=====================================================================
C:\php\php-bonjour$ nmake install
```

Then, you can run some manual tests using both PHP functions that are provided by the `php_bonjour.dll` extension:

```dos
C:\php-sdk\php-bonjour$ nmake run

Microsoft (R) Program Maintenance Utility Version 14.16.27025.1
Copyright (C) Microsoft Corporation. Tous droits réservés.

          "C:\php\php.exe" -n -d extension=C:\php-sdk\php-bonjour\x64\Release_TS\\php_bonjour.dll
<?php
bonjour_test1();
^Z
The extension bonjour is loaded and working!
```

or assuming you are using your production environment,

```dos
C:\$ c:\php\php.exe -n -d extension=c:\php\ext\php_bonjour.dll
<?php
echo bonjour_test2("la Terre");
^Z[enter]
Hello la Terre
C:\php-sdk\php-bonjour
```
