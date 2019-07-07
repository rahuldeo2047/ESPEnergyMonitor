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

      if(isset($_POST['image_type']))
        {
            $radio_type = $_POST['image_type'];
            echo "You have selected :".$radio_type."\n";  //  Displaying Selected Value
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
         echo "Success";
      }else{
         print_r($errors);
      }
   }
?>
