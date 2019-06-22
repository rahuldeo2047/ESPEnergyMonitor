<?php
header('Content-Type: application/json');

$conn = mysqli_connect("localhost:3306","root","RDIPYRiKKY1giVp3","mysql");

$sqlQuery = "SELECT * FROM `air_conditioner_sense` ORDER BY `data_id` DESC LIMIT 1" ; //`time` DESC LIMIT 1";
 
$result = mysqli_query($conn,$sqlQuery);

$data = array();
foreach ($result as $row) {
	$data[] = $row;
}

mysqli_close($conn);

echo json_encode($data);
?>
