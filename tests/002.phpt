--TEST--
bonjour_test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('bonjour')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = bonjour_test1();

var_dump($ret);
?>
--EXPECT--
The extension bonjour is loaded and working!
NULL
