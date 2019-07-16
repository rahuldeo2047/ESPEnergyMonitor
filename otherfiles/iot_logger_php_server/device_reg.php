<html>
   <head>
    
   </head>
   <body>
   <div align="center">
    <h2>Register devices</h2>
    <form action="" method="POST" enctype="multipart/form-data">
         
   <div style="display: inline-block; text-align: left;">
   <label for="MAC_ID">MAC ID</label>
   <input id="MAC_ID" name="MAC_ID" type="text" maxlength="12" value="000000000000" autocomplete="off"> 
   </br><label for="GPS">GPS   </label>
   <input id="GPS" name="GPS" type="text" maxlength="17" value="000.0000,000.0000" autocomplete="off"> 
   <label id="GPS_Err"> </label>
   </br><label for="device_type">Device type</label>
       <div style="border: thin solid black">
         <!-- the radio values are critical as the name -->
         <input type="radio" name="device_type" value="DEVT" checked> Development (Experimental)<br>
        <input type="radio" name="device_type" value="OBSN"> Observational (In case of any issue needs observation)<br>
        <input type="radio" name="device_type" value="DEPL"> Mass Deployment (warning: this will update all the device marked as 'depl'.) <br>
        <input type="radio" name="device_type" value="PROD"> Production (release for third party)<br>
       </div>
         
      </form><input type="submit"/>
      </div></div>

      <script>
var x = document.getElementById("GPS");
function getLocation() {
  if (navigator.geolocation) {
    navigator.geolocation.getCurrentPosition(showPosition, errfn
    ,{maximumAge:600000, timeout:5000, enableHighAccuracy: true});
 
  } else {
    //x.innerHTML = "Geolocation is not supported by this browser.";
  }
}  

function errfn(error) {
  var y = document.getElementById("GPS_Err");
  y.value = error.message;
}
 
function showPosition(position) {
  x.value =  position.coords.latitude + 
  "," + position.coords.longitude; 
}

//$(document).ready(getLocation);
window.onload = getLocation;

</script>

   </body>
</html>
<?php
   
      $radio_type="";
      $mac_id="";
      $gps="";

    if(isset($_POST['device_type']))
    {
        $radio_type = $_POST['device_type'];
        echo nl2br("\n\nYou have selected :".$radio_type."\n");  //  Displaying Selected Value
      

      if(isset($_POST['MAC_ID']))
      {
         $mac_id = $_POST['MAC_ID'];
         echo nl2br("\nMAC ID :".$mac_id."\n");  
      }  
   
      if(isset($_POST['GPS']))
      {
         $gps = $_POST['GPS'];
         echo nl2br("\nGPS :".$gps."\n");  
      }   

      $conn = mysqli_connect("localhost","id10062120_devices_logging","jUv2SjiYGhB8pkA","id10062120_devices");

      $sqlQuery = "INSERT INTO `device_info`(`Device_mac_id_str`, `Device_code_type`,`Device_location_gps`) VALUES (".$mac_id.",'".$radio_type."','".$gps."' )";
                // INSERT INTO `device_info`(`Device_mac_id_str`, `Device_code_type`,`Device_location_gps`) VALUES (0,'DEVT',0.0)
      $result = mysqli_query($conn,$sqlQuery);

      if ($result) { 
         echo nl2br("\nDevice registered having ID ".mysqli_insert_id($conn)."\n"); 
         mysqli_close($conn);
      }else {
        echo nl2br("\nError: " . "<br>" .  mysqli_error($conn));
        mysqli_close($conn);
    }
    
  }

      //echo "Device registered\n"; 
    
?>

