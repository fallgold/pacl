--TEST--
base acl test
--SKIPIF--
<?php if (!extension_loaded("pacl")) print "skip"; ?>
--FILE--
<?php 
$acl = new Pacl(dirname(__FILE__).'/acl.rule.ini');
var_dump($acl->check("user", "album", "show"));
var_dump($acl->check("user", "server", "show"));
var_dump($acl->check("tech", "server", "show"));
var_dump($acl->check("tech", "server", "del"));
var_dump($acl->check("admin", "server", "del"));
--EXPECTF--
bool(true)
bool(false)
bool(true)
bool(false)
bool(true)
