--TEST--
bonjour_array1() Basic test
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

var_dump(bonjour_array1($a1, $a2));

?>
--EXPECTF--
Array size arg1=3
zif_bonjour_array1:84 key[1]=NULL value=Bonjour
zif_bonjour_array1:84 key[2]=NULL value=Salut
zif_bonjour_array1:84 key[3]=NULL value=Au revoir
zif_bonjour_array1:96 key[1]=NULL value=Bonjour
zif_bonjour_array1:96 key[2]=NULL value=Salut
zif_bonjour_array1:93 key[3]=NULL value=Invalid String
string(34) "BonjourSalutAu revoirBonjourSalut "
