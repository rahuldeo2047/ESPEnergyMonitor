
<?php  

try {

$url = "https://script.google.com/macros/s/AKfycbz1paSaXa-JnXOJGBJSVCFCQmS0cw7XPtJnfbi6tnWsWlSrxRg/exec?" .$_SERVER['QUERY_STRING'];

// Initialize session and set URL.
$ch = curl_init();
curl_setopt($ch, CURLOPT_URL, $url);

// Set so curl_exec returns the result instead of outputting it.
//curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
    
// Get the response and close the channel.
$response = curl_exec($ch);
//echo $response;
if ($response == "1") {
    echo "\nSheet updated";
}
else
{
   echo "\nerror";
}
curl_close($ch);
}
catch(Exception $e) {
  echo 'error Message: ' .$e->getMessage();
}


?>  
