<html>
   <body>
   <div align="center">
    <h2>Update devices</h2>
    <form action="" method="POST" enctype="multipart/form-data">
         
   <div style="display: inline-block; text-align: left;">
      
      
       <input type="file" name="image" /><br>
       <div style="border: thin solid black">
         <!-- the radio values are critical as the name -->
         <input type="radio" name="image_type" value="devt" checked> Development (Experimental)<br>
        <input type="radio" name="image_type" value="obsn"> Observational (In case of any issue needs observation)<br>
        <input type="radio" name="image_type" value="depl"> Mass Deployment (warning: this will update all the device marked as 'depl'.) <br>
        <input type="radio" name="image_type" value="prod"> Production (release for third party)<br>
       </div>
       <div>0
       <label for="Config_id">Select Config ids to update</label>

                <!--div style="display: inline-block; text-align: left; height:200px;width:100%; overflow:auto; overflow-x:scroll; overflow-y:scroll;"-->
                <div style="width:  640px; height: 170px; overflow-y: scroll; overflow-x: scroll;">
                    
                    <table  id="dataTable">

                        <thead>
                            <tr>
                                <th>Selection</th>
                                <?php 
                                $conn = mysqli_connect("localhost","id10062120_devices_logging","jUv2SjiYGhB8pkA","id10062120_devices");
                                $sqlQuery = "SELECT * FROM `device_config` WHERE 1";
                                // INSERT INTO `device_info`(`Device_mac_id_str`, `Device_code_type`,`Device_location_gps`) VALUES (0,'DEVT',0.0)
                                $result = mysqli_query($conn,$sqlQuery);  
                                 

                                // foreach ($data[0] as $colm=>$val) {
                                //     echo "<th>".$colm."</th>";
                                // }

                                while ($field_info = mysqli_fetch_field($result))
                                {
                                    echo "<th>{$field_info->name}</th>";
                                }

                                // for($i = 0; $i < mysqli_num_fields($result); $i++) {
                                //         $field_info = mysqli_fetch_field($result, $i);
                                //         echo "<th>{$field_info->name}</th>";
                                //     }                                  
                                    ?>
                            </tr>
                        </thead>
                        <tbody>
                       
                        
                                <?php 

                                // foreach ($data as $val) {
                                //     echo "<tr>";
                                //     echo "<td>input type radio</td>";
                                //     // foreach ($val as $element) {
                                //     //       echo "<td>".$element."</td>";
                                //     // } 
                                //    echo "</tr>";
                                // } 

                                $cntr = 0;

                                while ($row=mysqli_fetch_row($result))
                                {
                                    
                                    echo "<tr><td><input type='checkbox' name='config_id' value=".$cntr."></td>";
                                    $cntr++;
                                    foreach ($row as $cell) {
                                        echo "<td >{$cell}</td>";
                                    }
                                    echo "</tr>";
                                    //echo "<th>{$field_info->name}</th>"; 
                                }
                        // while ($row = mysqli_fetch_array($query))
                        // {
                            
                        //     echo "<td>{field_data}</td>";
                        //     for($i = 0; $i < mysqli_num_fields($result); $i++) {
                        //         $field_data = $row[$i];
                        //         echo "<td>{field_data}</td>";
                        //     }  
                        // }
                        ?>
                            
                        </tbody>
                        <tfoot>
                            <tr>
                                <th colspan="4">TOTAL</th>
                                <th>
                                  <?php mysqli_num_fields($result);
                                  mysqli_free_result($result); ?>
                                </th>
                            </tr>
                        </tfoot>
                    </table>

       </div>
       <label for="code_version">Code version</label>
       <input id="code_version" name="code_version" type="text" maxlength="32" value="v0.0.0-4-g1abcdef" autocomplete="off">
         
      </form><input type="submit"/>
      </div></div>
   </body>
</html>
<?php
   if(isset($_FILES['image'])){
      $errors= array();
      $file_name = $_FILES['image']['name'];
      $file_size =$_FILES['image']['size'];
      $file_tmp =$_FILES['image']['tmp_name'];
      $file_type=$_FILES['image']['type'];
      $file_ext=strtolower(end(explode('.',$_FILES['image']['name'])));
      $radio_type="";
      $code_version="";
      $config_ids = array();
      $debug=0;

      if(isset($_POST['debug']))
      {
         $debug = $_POST['debug'];
         //echo "You have selected :".$radio_type."\n";  //  Displaying Selected Value
      }

      if(isset($_POST['image_type']))
        {
            $radio_type = $_POST['image_type'];
            echo "You have selected :".$radio_type."\n";  //  Displaying Selected Value
        }

        if(isset($_POST['code_version']))
        {
            $code_version = $_POST['code_version'];
            echo "version number given :".$code_version."\n";  //  Displaying Selected Value
        }

        if(isset($_POST['config_id']))
        {
           foreach($config_ids as $selected)
           {
             if($debug==1)
             {
                echo $selected;
                print_r($config_ids);
             }

            $config_ids += $selected;

           }

           print_r($config_ids);
        }
    
      $extensions= array("bin");
      
      if(in_array($file_ext,$extensions)=== false){
         $errors[]="extension not allowed, please choose a bin file.";
      }
      
      if($file_size > 2097152){
         $errors[]='File size must be excately 2 MB';
      }
      
      if(empty($errors)==true){
         $folder_name = strtoupper($radio_type);
         move_uploaded_file($file_tmp,"uploads/".$folder_name."/".$file_name);

         /*
         $conn = mysqli_connect("localhost","id10062120_devices_logging","jUv2SjiYGhB8pkA","id10062120_devices");
         //UPDATE `device_info` SET `Device_code_version`='v0.0.2-51-g35d2244' WHERE `Device_id`=1; 

         $sqlQuery = "UPDATE `device_info` SET `Device_code_version`='".$device_code_version."' WHERE `Device_id`='".$device_id."' AND  `Device_code_type`='".$device_code_type."'"; 
         $sqlQuery = "UPDATE `device_code_to_update_to` FROM `device_config` WHERE `config_id` IN "
         $data = array();

         if(mysqli_query($conn, $sqlQuery))
         {  
            $data[] = ['whether_updated' => '1'];
         }
         else
         {
            $data[] = ['whether_updated' => '0'];
         }

         mysqli_close($conn);

         echo json_encode($data);

         echo "Success";
      }else{
         print_r($errors);
      }
      */
   }
}
?>
