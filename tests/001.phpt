--TEST--
Check if bonjour is loaded
--SKIPIF--
<?php
if (!extension_loaded('bonjour')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "bonjour" is available';
?>
--EXPECT--
The extension "bonjour" is available
