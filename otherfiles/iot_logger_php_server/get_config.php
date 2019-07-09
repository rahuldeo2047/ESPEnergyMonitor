<?php
header('Content-Type: application/json');

if(!empty($_SERVER['QUERY_STRING']))
{
        parse_str($_SERVER['QUERY_STRING'], $output);
} 

$device_code_type="";
$congif_id=0;

// To work on this part
// Right now selection process will be done locally
$device_code_version="0.0.3-3";  // 0.0.0-3 => 0.0.0, 3
$input_code_version = ""; // 0.0.0
$input_code_version_commit_num = 0; // 3
$input_str_array = array();
$db_code_version_min = "";
$db_code_version_max = "";

if(!empty($output['device_code_type']))
{
	$device_code_type = $output['device_code_type'];	
}

if(!empty($output['congif_id']))
{
	$congif_id = $output['congif_id'];	
}

if(!empty($output['device_code_version']))
{
    $device_code_version = $output['device_code_version'];
    $input_str_array = explode ("-", $code_version); // 0.0.0-3 => 0.0.0, 3
    $input_code_version_arr = $input_str_array[0];
    $input_code_version_commit_num = $input_str_array[1];
}

$conn = mysqli_connect("localhost:3306","root","RDIPYRiKKY1giVp3","mysql");

$sqlQuery = "SELECT * FROM `device_config` WHERE device_code_type='".$device_code_type."' AND config_id=".$congif_id." LIMIT 1" ; //`time` DESC LIMIT 1";
 
$result = mysqli_query($conn,$sqlQuery);

if (!$result) {
    echo 'Could not run query: ' . mysql_error();
    exit;
}

$data = array();
foreach ($result as $row) {
    // check with code_version
    // https://stackoverflow.com/questions/14067736/compare-two-version-strings-in-php

    // $fieldinfo=mysqli_fetch_field($result); 
    // mysqli_fetch_field is not correct way of getting the name based data
    
    // foreach($fieldinfo as $field)
    // {
    //     if($field->name == 'device_min_code_version')
    //     {

    //     }

    //     if($field->name == 'device_max_code_version')
    //     {

    //     }
    // }
	$data[] = $row;
}

mysqli_free_result($result);

mysqli_close($conn);

echo json_encode($data);
?>
