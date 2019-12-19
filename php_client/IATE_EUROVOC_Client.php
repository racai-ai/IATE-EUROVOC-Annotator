<?php

function IATE_EUROVOC_Annotate($conllu){
   global $IATE_EUROVOC_Server_URL;

   $ch = curl_init();

   set_time_limit(0);
   ini_set("default_socket_timeout", 600);

   curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 0);
   curl_setopt($ch, CURLOPT_TIMEOUT_MS, 600 * 1000); //timeout in seconds
   curl_setopt($ch, CURLOPT_NOSIGNAL, 1);
   curl_setopt($ch, CURLOPT_URL,$IATE_EUROVOC_Server_URL);
   curl_setopt($ch, CURLOPT_POST, 1);
   curl_setopt($ch, CURLOPT_HTTPHEADER, array(
       'Content-Type: text/text',
       'Content-Length: ' . strlen($conllu))
   );
   curl_setopt($ch, CURLOPT_POSTFIELDS, $conllu);
   curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
   $server_output = curl_exec($ch);
   curl_close ($ch);
   
   return $server_output;

}
