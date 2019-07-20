<?php

include "./variables.php";

header('Content-Type: application/json');

if(!empty($_SERVER['QUERY_STRING']))
{
        parse_str($_SERVER['QUERY_STRING'], $output);
} 


if(!empty($output['device_code_type']))
{
	$device_code_type = $output['device_code_type'];	
}

if(!empty($output['device_id']))
{
	$device_id = $output['device_id'];	
}

if(!empty($output['congif_id']))
{
	$congif_id = $output['congif_id'];	
}

if(!empty($output['config_type']))
{
	$config_type = $output['config_type']; // 's'-short or 'l'-long or 'd'-direct using config_id
}

if(!empty($output['device_code_version']))
{
   // $device_code_version = $output['device_code_version'];
   // $input_str_array = explode ("-", $device_code_version); // 0.0.0-3 => 0.0.0, 3
    //$input_code_version_arr = $input_str_array[0];
    //$input_code_version_commit_num = $input_str_array[1];
}

$conn = mysqli_connect("localhost","id10062120_devices_logging","jUv2SjiYGhB8pkA","id10062120_devices");

if(!empty($output['Device_mac_id_str']))
{
	$Device_mac_id_str = $output['Device_mac_id_str'];	
 
        //if($config_type!='s')
        //{ 
            // Check for device info
            $sqlQuery = "SELECT * FROM `device_info` WHERE Device_mac_id_str='".$Device_mac_id_str."'";
            //$result = mysqli_query($conn, $sqlQuery);
  
            if ($result = mysqli_query($conn, $sqlQuery)) 
            {
                $number_of_rows=mysqli_num_rows($result);
                // echo($sqlQuery);
                // echo nl2br("\n number_of_rows =". $number_of_rows);
                // print_r($result);
                
                if($number_of_rows == 1) // existing entry and valid device 
                {
                    $row = mysqli_fetch_array($result);
                    //print_r($row);
                    $device_id=$row['Device_id'];
                    $config_id=$row['Device_config_id'];

                    //echo nl2br("\n device_id=$device_id, config_id=$config_id");
                    // $Device_type too can be used
                }

                mysqli_free_result($result);
            }
            
            
            //$sqlQuery = "INSERT INTO `device_info`(`device_id`, `Device_config_id`, `Device_mac_id`, `Device_mac_id_str`, `Device_code_type`, `Device_code_version`, `Device_location_gps`, `Device_code_update_datetime`, `This_info_entry_datetimestamp`) VALUES ( )";
        //}
}

// Get config

$data = array();

if($device_id > 0)
{ 
    //$sqlQuery = "SELECT * FROM `device_config` WHERE `device_id`=".$device_id;
    //device_code_type='".$device_code_type."' AND config_id=".$congif_id." LIMIT 1" ; //`time` DESC LIMIT 1";
    $sqlQuery = "";

    //echo nl2br("\n device_id=$device_id, config_id=$config_id");               

    if($config_type==='s')
    {
        $sqlQuery = "SELECT `config_id` ,`whether_update_available` ,`device_code_to_update_to` ,`server_host_address_config` ,`server_host_port_config` ,`host_config_server_query_path` FROM `device_config` WHERE device_code_type='".$device_code_type."' AND config_id=".$config_id." LIMIT 1" ; //`time` DESC LIMIT 1"
    } 
    else if($config_type==='l') 
    {
        $sqlQuery = "SELECT * FROM `device_config` WHERE device_code_type='".$device_code_type."' AND config_id=".$config_id." LIMIT 1" ; //`time` DESC LIMIT 1"
    } 

    if ($result = mysqli_query($conn,$sqlQuery)) 
    {
        $number_of_rows=mysqli_num_rows($result);
        
        if($config_type==='s')
        { 
            echo($sqlQuery);
            echo nl2br("\n number_of_rows =". $number_of_rows);
            print_r($result);
        }

        if($number_of_rows==1)
        { 

            foreach ($result as $row) 
            {
                $data[] = $row; 
            } 

            mysqli_free_result($result);
        }
        else
        {
            $data[] += ['err msg'=> 'no result'];
        }
    }
    else
    {
        $data[] += ['err msg'=> 'multiple configs'];
    }
}
else
{
    $data[] = ['err msg'=> 'wrong device id '.$device_id];
}

// Following data is not available in the db.table `device_config'
$data[0] += ['device_id'=> $device_id]; 
// appending data at the last of the db data
   
mysqli_close($conn);

echo json_encode($data);
?>
