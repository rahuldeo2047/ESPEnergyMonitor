<?php
header('Content-Type: application/json');

$conn = mysqli_connect("localhost:3306","root","RDIPYRiKKY1giVp3","mysql");

$sqlQuery = "SELECT * FROM air_conditioner_sense ORDER BY time DESC LIMIT 20";

$sql = "INSERT INTO `air_conditioner_sense` (`sr`, `dt`, `time`, `uptm`, `temp_filter`, `temp_raw`, `curr_filter`, `curr_raw`, `accel_filter`, `accel_raw`) VALUES (\'1\', \'675,986\', \'2019-06-21 11:00:00\', \'80366\', \'35.9\', \'36.0\', \'0.07\', \'0.07\', \'1.0\', \'1.02\')";

$result = mysqli_query($conn,$sqlQuery);

$data = array();
foreach ($result as $row) {
	$data[] = $row;
}


//while ($row = mysql_fetch_array($result)) {
//        $data[] = $row;

//}

mysqli_close($conn);

//echo json_encode($data, JSON_NUMERIC_CHECK);

//array_push($json, $data);
//echo json_encode($json);


echo json_encode($data);
?>