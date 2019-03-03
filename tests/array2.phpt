--TEST--
bonjour_array2() Basic test
--SKIPIF--
<?php
if (!extension_loaded('bonjour')) {
	echo 'skip';
}
?>
--FILE--
<?php
declare(strict_types=1);

$a1 = array(
  1 => "Bonjour",
  2 => "Salut",
  3 => "Au revoir",
);

$a2 = array(
  1 => "Bonjour",
  2 => "Salut",
  3 => $a1,
);

var_dump(bonjour_array2($a1, 'index-testA', false));
var_dump(bonjour_array2($a2, 'index-testB'));
var_dump(bonjour_array2($a1, 'index-testC', true));

?>
--EXPECT--
array(5) {
  [0]=>
  array(3) {
    [0]=>
    string(9) "Bonjour-0"
    [1]=>
    string(7) "Salut-1"
    [2]=>
    string(11) "Au revoir-2"
  }
  [1]=>
  array(3) {
    [0]=>
    string(9) "Bonjour-0"
    [1]=>
    string(7) "Salut-1"
    [2]=>
    string(11) "Au revoir-2"
  }
  [2]=>
  array(3) {
    [0]=>
    string(9) "Bonjour-0"
    [1]=>
    string(7) "Salut-1"
    [2]=>
    string(11) "Au revoir-2"
  }
  [3]=>
  array(3) {
    [0]=>
    string(9) "Bonjour-0"
    [1]=>
    string(7) "Salut-1"
    [2]=>
    string(11) "Au revoir-2"
  }
  [4]=>
  array(3) {
    [0]=>
    string(9) "Bonjour-0"
    [1]=>
    string(7) "Salut-1"
    [2]=>
    string(11) "Au revoir-2"
  }
}
array(5) {
  [0]=>
  array(2) {
    [0]=>
    string(9) "Bonjour-0"
    [1]=>
    string(7) "Salut-1"
  }
  [1]=>
  array(2) {
    [0]=>
    string(9) "Bonjour-0"
    [1]=>
    string(7) "Salut-1"
  }
  [2]=>
  array(2) {
    [0]=>
    string(9) "Bonjour-0"
    [1]=>
    string(7) "Salut-1"
  }
  [3]=>
  array(2) {
    [0]=>
    string(9) "Bonjour-0"
    [1]=>
    string(7) "Salut-1"
  }
  [4]=>
  array(2) {
    [0]=>
    string(9) "Bonjour-0"
    [1]=>
    string(7) "Salut-1"
  }
}
array(5) {
  [0]=>
  array(3) {
    ["index-testC-0"]=>
    string(9) "Bonjour-0"
    ["index-testC-1"]=>
    string(7) "Salut-1"
    ["index-testC-2"]=>
    string(11) "Au revoir-2"
  }
  [1]=>
  array(3) {
    ["index-testC-0"]=>
    string(9) "Bonjour-0"
    ["index-testC-1"]=>
    string(7) "Salut-1"
    ["index-testC-2"]=>
    string(11) "Au revoir-2"
  }
  [2]=>
  array(3) {
    ["index-testC-0"]=>
    string(9) "Bonjour-0"
    ["index-testC-1"]=>
    string(7) "Salut-1"
    ["index-testC-2"]=>
    string(11) "Au revoir-2"
  }
  [3]=>
  array(3) {
    ["index-testC-0"]=>
    string(9) "Bonjour-0"
    ["index-testC-1"]=>
    string(7) "Salut-1"
    ["index-testC-2"]=>
    string(11) "Au revoir-2"
  }
  [4]=>
  array(3) {
    ["index-testC-0"]=>
    string(9) "Bonjour-0"
    ["index-testC-1"]=>
    string(7) "Salut-1"
    ["index-testC-2"]=>
    string(11) "Au revoir-2"
  }
}