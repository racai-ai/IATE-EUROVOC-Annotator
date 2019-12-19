<?php

require "IATE_EUROVOC_Client.php";

$IATE_EUROVOC_Server_URL="http://127.0.0.1:9001/annotate";

$test_conllu=file_get_contents("test.conllu");

var_dump(IATE_EUROVOC_Annotate($test_conllu));
