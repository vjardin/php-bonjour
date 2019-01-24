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

## step 3.-1 - recompile PHP - SKIP THIS SECTION

This section describes how to recompile PHP and then we'll leverage the environment to build the extension.
It is not the proper way of building an extension. For intance, for the previous case of Linux, you did
not recompile PHP before building your `php-bonjour` extesion. I just keep this section as a reminder.
I advise that you jump to the next section `3.0`.

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
Other folders are important too: `lib` provides the library to link with, `include` provides the C header files
that our extension (dll) will use during its compilation.

## step 3.0 - Install the php-dev framework for Windows

There are 2 ways to install the PHP development environment:
 * using Chcolatey, and assuming that only *one* PHP is installed on your system.
 * using an handy setup with curl, then having multiple PHP interpreter on your system.

### step 3.0.1 - with Chocolatey

The official PHP releases for Windows and their respective development packages are available at:
[https://windows.php.net/downloads/releases/](https://windows.php.net/downloads/releases/).
A develpment package provides the header files (`.h`) and the tools to build your php extensions (`phpize.bat`, etc.)

First you need to install a [PHP release](https://windows.php.net/downloads/releases/):
 * either [php-7.3.1-Win32-VC15-x64.zip](https://windows.php.net/downloads/releases/php-7.3.1-Win32-VC15-x64.zip)
 * or [php-7.3.1-nts-Win32-VC15-x64.zip](https://windows.php.net/downloads/releases/php-7.3.1-nts-Win32-VC15-x64.zip)

Myself, I do prefer to get it from [Chocolatey](https://chocolatey.org/packages/php) PHP packages:

```dos
C:\php$ choco install php --params '"/ThreadSafe ""/InstallDir:C:\php"""'
C:\php$ choco upgrade php --params '"/ThreadSafe ""/InstallDir:C:\php"""'
```

then its respective develoment package:
[php-devel-pack-7.3.1-Win32-VC15-x64.zip](https://windows.php.net/downloads/releases/php-devel-pack-7.3.1-Win32-VC15-x64.zip)
[php-devel-pack-7.3.1-nts-Win32-VC15-x64.zip](https://windows.php.net/downloads/releases/php-devel-pack-7.3.1-nts-Win32-VC15-x64.zip)

You should unzip the proper `php-devel` into `c:\php\SDK\` and you should add a path for `phpize.bat`:

```dos
set PATH=%PATH%;c:\php\SDK
```

### step 3.0.1 - with curl.exe

The official PHP releases for Windows and their respective development packages are available at:
[https://windows.php.net/downloads/releases/](https://windows.php.net/downloads/releases/).
A develpment package provides the header files (`.h`) and the tools to build your php extensions (`phpize.bat`, etc.)

First you need to install a [PHP release](https://windows.php.net/downloads/releases/):
 * either [php-7.3.1-Win32-VC15-x64.zip](https://windows.php.net/downloads/releases/php-7.3.1-Win32-VC15-x64.zip)
 * or [php-7.3.1-nts-Win32-VC15-x64.zip](https://windows.php.net/downloads/releases/php-7.3.1-nts-Win32-VC15-x64.zip)

The following can be scripted using `curl.exe`. Sicne I need to check for both NTS and TS releases, I do prefer to have
both installed.

```dos
$ mkdir c:\dev
$ mkdir c:\dev\php
$ mkdir c:\dev\php\ts
$ mkdir c:\dev\php\nts
$ cd \dev\php\nts
c:\dev\php\nts $ curl https://windows.php.net/downloads/releases/php-7.3.1-nts-Win32-VC15-x64.zip -o php-7.3.1-nts-Win32-VC15-x64.zip
c:\dev\php\nts $ unzip php-7.3.1-nts-Win32-VC15-x64.zip
c:\dev\php\nts $ curl https://windows.php.net/downloads/releases/php-devel-pack-7.3.1-nts-Win32-VC15-x64.zip -o php-devel-pack-7.3.1-nts-Win32-VC15-x64.zip
c:\dev\php\nts $ unzip php-devel-pack-7.3.1-nts-Win32-VC15-x64.zip
Archive:  php-devel-pack-7.3.1-nts-Win32-VC15-x64.zip
    creating: php-7.3.1-devel-VC15-x64/
    creating: php-7.3.1-devel-VC15-x64/build/
       ...
$ cd c:\dev\php\ts
c:\dev\php\ts $ curl https://windows.php.net/downloads/releases/php-7.3.1-Win32-VC15-x64.zip -o php-7.3.1-Win32-VC15-x64.zip
c:\dev\php\ts $ unzip php-7.3.1-Win32-VC15-x64.zip
c:\dev\php\ts $ curl https://windows.php.net/downloads/releases/php-devel-pack-7.3.1-Win32-VC15-x64.zip -o php-devel-pack-7.3.1-Win32-VC15-x64.zip
c:\dev\php\ts $ unzip php-devel-pack-7.3.1-Win32-VC15-x64.zip
```

## step 3.1 - Build `php-bonjour` on Windows

```dos
C:\dev$ mkdir projects
C:\dev$ cd projects
C:\dev\projects$ git clone https://github.com/vjardin/php-bonjour.git
C:\dev\projects$ cd php-bonjour
```

Then, you are ready to get it compiled. First, you need to setup the build environment for `Microsoft Visual C`:

```dos
c:\dev\projects\bonjour $ "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
```

then, we have 3 options to setup the path depending on your deployed PHP environment:
 * `c:\php\php.exe` and `c:\php\SDK\phpize.bat`
 * `c:\dev\php\nts\php.exe` and `c:\dev\php\nts\php-7.3.1-devel-VC15-x64\phpize.bat`
 * `c:\dev\php\ts\php.exe`  and  `c:\dev\php\ts\php-7.3.1-devel-VC15-x64\phpize.bat`

For instance, using the `ts` option, you should setup you PATH and PHPRC:

```dos
$ set PHPRC=c:\dev\php\ts
$ set PATH=c:\dev\php\ts;c:\dev\php\ts\php-7.3.1-devel-VC15-x64;%PATH%
```

I advise that you set it up within your `ConEmu`'s tab once for ever.

You can check you `php.exe` and `phpize.bat` to be used with:

```dos
$ where phpize
c:\dev\php\ts\php-7.3.1-devel-VC15-x64\phpize.bat

$ where php
c:\dev\php\ts\php.exe
c:\tools\php72\php.exe
```

Build the Makefile for your PHP module using `phpize`:

```dos
C:\dev\projects\php-bonjour$ phpize
```

Then, you are ready to compile using the command
`configure --with-php=%PHPRC% --enable-bonjour`
but configure will need tools like `bison`, `sed` or `re2c` which are not
required.
Either you install them (thanks to `chocolatey` for instance), or you disable
them using the hard way from your `configure.js` script:

C:\dev\projects\php-bonjour$ vim configure.js
```javascript
 ...
toolset_setup_arch();

toolset_setup_linker();
//toolset_setup_project_tools(); TODO comment it, no need for bison, neither re2c, etc.

// stick objects somewhere outside of the source tree
/* ARG_ENABLE('object-out-dir', 'Alternate location for binary objects during build', ''); */
 ...
```

Then, you are ready to compile:

```dos
php-bonjour$ configure --with-php=%PHPRC% --enable-bonjour
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
php-bonjour$ nmake
php-bonjour$ nmake test
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
```

Then, you can run some manual tests using both PHP functions that are provided by the `php_bonjour.dll` extension:

```dos
php-bonjour$ nmake run

Microsoft (R) Program Maintenance Utility Version 14.16.27025.1
Copyright (C) Microsoft Corporation. Tous droits reserves.

          "C:\dev\ts\php\php.exe" -n -d extension=C:\dev\projects\php-bonjour\x64\Release_TS\\php_bonjour.dll
<?php
bonjour_test1();
^Z
The extension bonjour is loaded and working!
```

If you face any issues, you can check that all system depencies are properly installed:
```dos
php-bonjour$ deplister x64\Release_TS\php_bonjour.dll
php7ts.dll,OK
VCRUNTIME140.dll,OK
api-ms-win-crt-runtime-l1-1-0.dll,OK
KERNEL32.dll,OK
```

This new `php_bonjour.dll` can be released with your `.msi` or any other means. Currently,
we'll only release it for the local system:

```dos
php-bonjour$ nmake install
```

# Usage, special notes for Windows

On any production or development system, you can have fun with you `php_bonjour.dll` using:

```dos
c:\$ php -n -c %PHPRC%\php.ini-development -d extension_dir=%PHPRC%\ext -d extension=bonjour -m | findstr bonjour
c:\$ php -n -c %PHPRC%\php.ini-development -d extension_dir=%PHPRC%\ext -d extension=bonjour -a

Interactive shell
php > bonjour_test1();
The extension bonjour is loaded and working!
php > bonjo[tab]
```

or

```dos
c:\$ php -n -c %PHPRC%\php.ini-development -d extension_dir=%PHPRC%\ext -d extension=bonjour
<?php
echo bonjour_test2("la Terre");
^Z[enter]
Hello la Terre
C:\php-sdk\php-bonjour
```

# Tests

The PHP build framework for extensions uses the script `run-tests.php` in order to execute all the set of tests.
We saw within the previous section that it can be called using:

```console
vj@p:~/php-bonjour$ make test TESTS="tests/002.phpt tests/003.phpt"
```

However all the tests are run at once. You can run only one or a set of specific test using the `TESTS=` argument:

```console
vj@p:~/php-bonjour$ make test TESTS="tests/002.phpt"
vj@p:~/php-bonjour$ make test TESTS="tests/002.phpt tests/003.phpt"
```

it will run only the test `002.phpt` or both the tests `002.phpt` and `003.phpt`.

The same test framework and arguments are available for both Linux and Windows. Or course, with Windows,
you need to use `nmake` instead of `make` and replace the '/' with a '\'.

## Analyse a failing test

If you want to understand how to analyse a failing test, let's change the test `003.phpt` so it will
keep failing:

```console
vj@p:~/php-bonjour$ git diff
diff --git a/tests/003.phpt b/tests/003.phpt
index c025eba..4decf39 100644
--- a/tests/003.phpt
+++ b/tests/003.phpt
@@ -14,3 +14,4 @@ var_dump(bonjour_test2('PHP'));
 --EXPECT--
 string(11) "Hello World"
 string(9) "Hello PHP"
+XXX
```

We add the `XXX` string at the and of the expected output of the test.

Then, once you'll run it, it will fail:

```console
vj@p:~/php-bonjour$ make test TESTS=tests/003.phpt
 ...
=====================================================================
Running selected tests.
FAIL bonjour_test2() Basic test [tests/003.phpt]
=====================================================================
Number of tests :    1                 1
Tests skipped   :    0 (  0.0%) --------
Tests warned    :    0 (  0.0%) (  0.0%)
Tests failed    :    1 (100.0%) (100.0%)
Expected fail   :    0 (  0.0%) (  0.0%)
Tests passed    :    0 (  0.0%) (  0.0%)
---------------------------------------------------------------------
Time taken      :    0 seconds
=====================================================================

=====================================================================
FAILED TEST SUMMARY
---------------------------------------------------------------------
bonjour_test2() Basic test [tests/003.phpt]
=====================================================================
Makefile:133: recipe for target 'test' failed
make: *** [test] Error 1
```

and you will have the following files that you can analyse:

```console
vj@p:~/php-bonjour$ ls tests/003.*
tests/003.diff  tests/003.exp  tests/003.log  tests/003.out  tests/003.php  tests/003.phpt  tests/003.sh
```

You should open each of these files and review them.

```console
vj@p:~/php-bonjour$ cat $(ls tests/003.*)
 ...
```

You can notice that you can re-run a specific test wihtout the test framework using the `.sh` file generated
by `run-tests.php`.

```console
vj@p:~/php-bonjour$ tests/003.sh
string(11) "Hello World"
string(9) "Hello PHP"
```

of course, on Windows, you have to adapt it a bit:

```dos
C:\$ php.exe -n -d extension=x64\Release_TS\php_bonjour.dll -f "tests\003.php"
```
