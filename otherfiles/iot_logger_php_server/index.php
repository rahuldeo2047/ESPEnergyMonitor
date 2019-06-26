<!DOCTYPE html>
<html>

<head>
    <title>Creating Dynamic Data Graph using PHP and Chart.js</title>
    <style type="text/css">
        BODY {
            width: 95%;
        }

        #chart-container {
            width: 100%;
            height: auto;
        }

        * {
            box-sizing: border-box;
        }

div.scroll {
  background-color: #f2f2f2;
  width: 95%;
  height: 80px;
  overflow: auto;
  display: table;
} 

img.log_img {
  vertical-align: middle;
  display: table-cell;
}
span {
  vertical-align: middle;
  display: table-cell;
}

        /* Create three equal columns that floats next to each other */
        .column {
            float: left;
            width: 33%;
            padding: 1px;
            height: auto;
            /* Should be removed. Only for demonstration */
        }

        /* Clear floats after the columns */
        .row:after {
            content: "";
            display: table;
            clear: both;
        }

	/* Data Table Styling */
        #dataTable {
            font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
            border-collapse: collapse;
            width: 100%;
        }

        #dataTable td,
        #dataTable th {
            border: 1px solid #ddd;
            padding: 8px;
        }

        #dataTable tr:nth-child(even) {
            background-color: #f2f2f2;
        }

        #dataTable tr:hover {
            background-color: #ddd;
        }

        #dataTable th {
            padding-top: 12px;
            padding-bottom: 12px;
            text-align: left;
            background-color: #4CAF50;
            color: white;
        }
      
	  
    </style>
    <script type="text/javascript" src="js/moment.js"></script>
    <script type="text/javascript" src="js/jquery.min.js"></script>
    <script type="text/javascript" src="js/Chart.min.js"></script>


    
</head>

<body>

 

    <div class="row">
        <div class="column">
            <canvas id="graphCanvas_temp" style="border:1px solid #000000;"></canvas>
        </div>
        <div class="column">
            <canvas id="graphCanvas_curr" style="border:1px solid #000000;"></canvas>
        </div>
        <div class="column">
            <canvas id="graphCanvas_acc" style="border:1px solid #000000;"></canvas>
        </div>
    </div>
    <div class="row">
        <div id="outside" class="column">
             <h2>City 
		<select id='selectCity' onchange='showWeatherOutside()'>
		  <!--option value="" disabled selected style="display:none;">City</option-->
		</select>
		</h2>

        </div>
        <div class="column">
            <canvas id="graphCanvas_temp_rad" style="border:1px solid #000000;"></canvas>
        </div>
	<div id="predictions" class="column">
             
	</div>
        <progress id="animationProgress" max="1" value="0.5" style="width: 100%"></progress>
        <div id"tempC_loc" class="row">Loading...<div> 
         
    </div>
    <div id="text_display">
    </div>
    <div id="output_display">
    </div>
    <div>
        <table id="dataTable">
            <tr>
                <th>Sr.</th>
                <th>Time</th>
                <th>dt</th>
                <th>temp Value filtered</th>
                <th>temp Value raw</th>
                <th>curr Value filtered</th>
                <th>curr Value raw</th>
                <th>acc Value filtered</th>
                <th>accc Value raw</th>
            </tr>
        </table>
    </div>
    <script>
        function getXMLHTTP() {
            var x = false;
            try {
                x = new XMLHttpRequest();
            } catch (e) {
                try {
                    x = new ActiveXObject("Microsoft.XMLHTTP");
                } catch (ex) {
                    try {
                        req = new ActiveXObject("Msxml2.XMLHTTP");
                    }
                    catch (e1) {
                        x = false;
                    }
                }
            }
            return x;
        }
    </script>
    <script>
        const SIMULATE = false;

	const severities = 
	{
            INVALID: 'invalid',
	    NONE: 'none',
	    INFO: 'info',
	    ALERT: 'alert',
	    WARNING: 'warning',
	    CRITICAL: 'critical'
	}

	const senses =
	{
		TEMPERATURE: 'temperature',
		CURRENT: 'current',
		VIBRATION: 'vibration'
	}


	let severity_states = {
	  temperature_severity: severities.INVALID,
	  current_info: severities.INVALID,
	  vibration_info: severities.INVALID
	};
 


var listed_cities = [
 {
    "id": 1278314,
    "name": "Asansol",
    "country": "IN",
    "coord": {
      "lon": 86.98333,
      "lat": 23.683331
    }
},


{
    "id": 1270642,
    "name": "Gurgaon",
    "country": "IN",
    "coord": {
      "lon": 77.033333,
      "lat": 28.466667
    }
},

 {
    "id": 1275097,
    "name": "Bundu",
    "country": "IN",
    "coord": {
      "lon": 85.583328,
      "lat": 23.183331
    }
},
 {
    "id": 1258526,
    "name": "Ranchi",
    "country": "IN",
    "coord": {
      "lon": 85.333328,
      "lat": 23.35
    }
},
  {
    "id": 1262321,
    "name": "Mysore",
    "country": "IN",
    "coord": {
      "lon": 76.649719,
      "lat": 12.30722
    }
  }];



var select;


        $(document).ready(function () {
		select = document.getElementById("selectCity");

		for(var i = 0; i < listed_cities.length; i++) {
		var opt = listed_cities[i];
		var el = document.createElement("option");
		el.textContent = opt.name;
		el.value = opt.id;
		select.appendChild(el);}

            showGraph();
            setInterval(updateChart, 1000);

        });



function getForcast(id)
{
//var key = "YOUR KEY";
//var city = "YOUR CITY"; // My test case was "London"
var url = "https://api.openweathermap.org/data/2.5/forecast";

$.ajax({
  url: url, //API Call
  dataType: "json",
  type: "GET",
  data: {
    id: id,
    appid: "05025ab29e4fec699aaede714bbd823d",
    units: "metric",
    cnt: "9"
  },
  success: function(data) {
    console.log('Received data:', data) // For testing
    var wf = "" ; 
    $.each(data.list, function(index, val) {
        var d1 = new Date(val.dt_txt);
	var d2 = new Date(Date.now());
	var same_three_hrs = d1.getTime() - d2.getTime(); 
        same_three_hrs = same_three_hrs / (1000.0*60.0*60.0);

    if(same_three_hrs<3 && same_three_hrs>0){
      wf += "<font color='09F909'>" // Opening paragraph tag
      wf += "<b>Forecast for " + val.dt_txt + "</b><br> " // Day
      wf += "<img src='https://openweathermap.org/img/w/" + val.weather[0].icon + ".png'><b>" // Icon
      wf += val.main.temp + "&degC </b>| Humidity " // Temperature
      wf += val.main.humidity + "% " // Temperature
      wf += "| " + val.weather[0].description + "<br>"; // Description
      wf += "  wind speed " + val.wind.speed + " | wind angle " + val.wind.deg;
      wf += "<hr></font><div class='scroll'><span><img id='log_img' src='imgs/waiting.png' width='50' height='50'></img></span> <span id='logs'>waiting for events...<br> msgs will be here</span></div>" // Closing paragraph tag
    }});
    $("#predictions").html(wf);
  }
});
}

function showWeatherOutside() 
{
  
//id=1270642
//lon="+position.coords.latitude.toFixed(3)+"&lat="+position.coords.longitude.toFixed(3)
let gps = listed_cities[select.selectedIndex];

	$.getJSON("https://api.openweathermap.org/data/2.5/weather?id="+gps.id+"&units=imperial&appid=05025ab29e4fec699aaede714bbd823d", function(data) 
 {
 
	console.log(data);

	var icon = "http://openweathermap.org/img/w/" + data.weather[0].icon + ".png";

	var temp = Math.floor(data.main.temp);
	  
	temp =  ((temp - 32)*5/9).toFixed(1);

	var weather = data.weather[0].main;

	//data.name // city name

	//x.innerHTML = "City " + data.name +"<br>Latitude: " + position.coords.latitude.toFixed(2) + 
 // "<br>Longitude: " + position.coords.longitude.toFixed(2); 

          console.log('Received data:', data); // For testing

	wf =  "<font color='09F909'><div class='row'>";
	wf += " <div class='column'>";
	wf += "  <h3> Current Weather </h3>";
	wf += " </div>";
	wf += " <div align='center' class='column'>";
	wf += "  <img src='https://openweathermap.org/img/w/" + data.weather[0].icon + ".png'>"; //data.name  ;
	wf += "	</div>";
	wf += " <div class='column'>";
	wf += "Date: <br>"+getDateString(data.dt);
	wf += "	</div>";
	wf += "</div>";
	wf += " ";
	wf += "<div class='row'>";
	wf += " <div class='column'>";
	wf += data.weather[0].description + "<h4> Humidity "+ data.main.humidity + "%</h4>";
	wf += " </div>";
	wf += " <div class='column'>";
	wf += "  <h1>"+temp+ " &#8451;</h1>";
	wf += " </div>";
	wf += " <div class='column'>";
	wf += "  visibility " + data.visibility + " | wind speed " + data.wind.speed + " | wind angle " + data.wind.deg;
	wf += "	</div>";
	wf += "</div>";
	wf += " "; 
	wf += "<div>"
	wf += "  Sunrise " + getDateString(data.sys.sunrise);
	wf += "   | Sunset " + getDateString(data.sys.sunset);

let hrs = (Math.abs(data.sys.sunset - data.sys.sunrise) / 36e2);
let hrs_hh = Math.trunc(hrs);
let hrs_mm = (hrs-hrs_hh)*60;
let hrs_ss = (hrs_mm - Math.trunc(hrs_mm))*60;
hrs_mm = Math.trunc(hrs_mm);


	wf += "  <br>Day hours " + hrs_hh +":"+ hrs_mm +":"+ hrs_ss.toFixed(0) +" hrs";
	wf += " </div></font>"; 

	$("#outside").html(wf);

	getForcast(gps.id)

 });

}

	function updateWeatherStatusAndForcast()
	{
		showWeatherOutside() 
	}

        setInterval(updateWeatherStatusAndForcast, 30*60*1000);

        //var chartdata;
        var lineGraph_temp;
        var lineGraph_curr;
        var lineGraph_acc;
        var lastdata;
        var datalen = 0;
        var lasttimelocal = new Date();
        var lasttimectrl = 0;
        var table;
        var progress;
  
        function showGraph() {

            progress = document.getElementById("animationProgress");

            {
                $.post("data.php",
                    function (data) {
                        if (data.length > 0) {
                            //console.log(data);
                            var time = [0];
                            var temp = [0.0];
                            var temp_raw = [0.0];
                            var curr = [0.0];
                            var curr_raw = [0.0];
                            var accel = [0.0];
                            var acc_raw = [0.0];
                            var timectrl = 0;
                            lasttimelocal = new Date(data[0].time);

                            datalen = data.length;

                            for (var i in data) {
                                time.push(data[i].time);


                                table = document.getElementById("dataTable");
                                var row = table.insertRow(1);	//Add after headings
                                var cell0 = row.insertCell(0);
                                var cell1 = row.insertCell(1);
                                var cell2 = row.insertCell(2);
                                var cell3 = row.insertCell(3);
                                var cell4 = row.insertCell(4);
                                var cell5 = row.insertCell(5);
                                var cell6 = row.insertCell(6);
                                var cell7 = row.insertCell(7);
                                var cell8 = row.insertCell(8);


                                cell0.innerHTML = data[i].sr;
                                cell1.innerHTML = data[i].time;
                                cell2.innerHTML = [new Date().getTime() - lasttimelocal.getTime(), parseInt(timectrl) - parseInt(lasttimectrl)].toString();
                                cell3.innerHTML = data[i].temp_filter;
                                cell4.innerHTML = data[i].temp_raw;
                                cell5.innerHTML = data[i].curr_filter;
                                cell6.innerHTML = data[i].curr_raw;
                                cell7.innerHTML = data[i].accel_filter; // It is not nan here ???
                                cell8.innerHTML = data[i].accel_raw;

                                temp.push(data[i].temp_filter);
                                temp_raw.push(data[i].temp_raw);
                                curr.push(data[i].curr_filter);
                                curr_raw.push(data[i].curr_raw);
                                var st = isNaN(data[i].acc_filter); // unknown why it is Nan here
                                accel.push(cell7.innerHTML); // doing this solves it ???
                                acc_raw.push(cell8.innerHTML);

                                timectrl = data[i].uptm;


                                lasttimelocal = new Date(data[i].time);
                                lasttimectrl = data[i].uptm;



                            }

                            lastdata = data[datalen - 1];
                        }
                        else {
                            // If no data then put up some dummy
                            updateInsertStr();
                            datalen = 1; // otherwise it will keep on going on loop in updateChart()
                        }

                        var chartdata_temp = {
                            labels: time,
                            datasets: [
                                {
                                    label: 'Temperature',
                                    backgroundColor: '#49e2ff',
                                    borderColor: '#46d5f1',
                                    hoverBackgroundColor: '#CCCCCC',
                                    hoverBorderColor: '#666666',
                                    data: temp
                                },
                                {
                                    label: 'Temperature raw',
                                    backgroundColor: '#e2e2ff',
                                    borderColor: '#d5d5f1',
                                    hoverBackgroundColor: '#CCCCCC',
                                    hoverBorderColor: '#666666',
                                    data: temp_raw
                                },
                            ]

                        };

                        var graphTarget_temp = $("#graphCanvas_temp");

                        lineGraph_temp = new Chart(graphTarget_temp, {
                            type: 'line',
                            data: chartdata_temp,
                            options: {
				scales: {
				    yAxes: [{
					ticks: {
					    suggestedMin: 35,
					    suggestedMax: 40
					}
				    }]/*,
				    xAxes: [{
			   		type: 'time',
					distribution: 'series'
					    }]*/
				},
                                animation: {
                                    duration: 200,
                                    onProgress: function (animation) {
                                        progress.value = animation.animationObject.currentStep / animation.animationObject.numSteps;
                                    },
                                    onComplete: function () {
                                        setTimeout(function () {
                                            //progress.value = 0;
                                        }, 2000);
                                    }
                                }

                            }
                        });

                        var chartdata_curr = {
                            labels: time,
                            datasets: [
                                {
                                    label: 'Current',
                                    backgroundColor: '#49e2ff',
                                    borderColor: '#46d5f1',
                                    hoverBackgroundColor: '#CCCCCC',
                                    hoverBorderColor: '#666666',
                                    data: curr
                                },
                                {
                                    label: 'Current raw',
                                    backgroundColor: '#e2e2ff',
                                    borderColor: '#d5d5f1',
                                    hoverBackgroundColor: '#CCCCCC',
                                    hoverBorderColor: '#666666',
                                    data: curr_raw
                                }
                            ]
                        };

                        var graphTarget_curr = $("#graphCanvas_curr");

                        lineGraph_curr = new Chart(graphTarget_curr, {
                            type: 'line',
                            data: chartdata_curr,
			    options: {
				scales: {
				    yAxes: [{
					ticks: {
					    suggestedMin: 1,
					    suggestedMax: 8
					}
				    }]/*,
				    xAxes: [{
			   		type: 'time',
					distribution: 'series'
					    }]*/
				}
			    }
                        });

                        var chartdata_acc = {
                            labels: time,
                            datasets: [
                                {
                                    label: 'Acc',
                                    backgroundColor: '#49e2ff',
                                    borderColor: '#46d5f1',
                                    hoverBackgroundColor: '#CCCCCC',
                                    hoverBorderColor: '#666666',
                                    data: accel
                                },
                                {
                                    label: 'Acc raw',
                                    backgroundColor: '#e2e2ff',
                                    borderColor: '#d5d5f1',
                                    hoverBackgroundColor: '#CCCCCC',
                                    hoverBorderColor: '#666666',
                                    data: acc_raw
                                }
                            ]
                        };

                        var graphTarget_acc = $("#graphCanvas_acc");

                        lineGraph_acc = new Chart(graphTarget_acc, {
                            type: 'line',
                            data: chartdata_acc,
			    options: {
				scales: {
				    yAxes: [{
					ticks: {
					    suggestedMin: 1,
					    suggestedMax: 8
					}
				    }]/*,
				    xAxes: [{
			   		type: 'time',
					distribution: 'series'
					    }]*/
				}
			    }
                        });



                        var chartdata_temp_rad = {
                            labels: ["Red", "green"],
                            datasets: [{
                                label: "temp",
                                data: [10, 190],
                                borderWidth: 10,
                                backgroundColor: [
                                    "rgb(54, 235, 162 )",
                                    "rgb(255, 99, 132)",
                                    "rgb(255, 205, 86)"
                                ]
                            }, {
                                label: "curr",
                                data: [190, 10],
                                borderWidth: 5,
                                backgroundColor: [
                                    "rgb(54, 235, 162 )",
                                    "rgb(255, 99, 132)",
                                    "rgb(255, 205, 86)"
                                ]
                            }, {
                                label: "acc",
                                data: [10, 10],
                                borderWidth: 0,
                                backgroundColor: [
                                    "rgb(54, 235, 162 )",
                                    "rgb(255, 99, 132)",
                                    "rgb(255, 205, 86)"
                                ]
                            }]
                        };

                        var graphTarget_temp_rad = $("#graphCanvas_temp_rad");

                        lineGraph_temp_rad = new Chart(graphTarget_temp_rad, {
                            type: 'doughnut',
                            data: chartdata_temp_rad,
                            options: {
                                circumference: 2 * Math.PI - 2 * Math.PI / 4,
                                rotation: 3 * Math.PI / 4,
                                cutoutPercentage: 20, // precent
                                title: {
                                    display: true,
                                    text: 4,
                                    position: 'bottom',
                                    fontSize: 20
                                },
                                plugins: {
                                    datalabels: {
                                        backgroundColor: 'rgba(0, 0, 0, 0.7)',
                                        borderColor: '#ffffff',
                                        color: function (context) {
                                            return context.dataset.backgroundColor;
                                        },
                                        font: function (context) {
                                            var w = context.chart.width;
                                            return {
                                                size: w < 512 ? 18 : 20
                                            }
                                        },
                                        align: 'start',
                                        anchor: 'start',
                                        offset: 50,
                                        borderRadius: 4,
                                        borderWidth: 1,
                                        formatter: function (value, context) {
                                            var i = context.dataIndex;
                                            var len = context.dataset.data.length - 1;
                                            if (i == len) {
                                                return null;
                                            }
                                            return value + ' dC';
                                        }
                                    }
                                },
                                legend: {
                                    display: false
                                },
                                tooltips: {
                                    enabled: false
                                }
                            }


                        });
                    });
            }

        }

        function updateChart() {
            if (datalen == 0) {
                showGraph();
            }
            else {
                $.post("get_last_row.php", function (data) {

                    var id1 = parseInt(data[0].data_id);
                    var id2 = parseInt(lastdata.data_id);
                    if (id1 != id2) {
                        //console.log(data);

                        timectrl = data[0].uptm;

                        var row = table.insertRow(1);	//Add after headings
                        var cell0 = row.insertCell(0);
                        var cell1 = row.insertCell(1);
                        var cell2 = row.insertCell(2);
                        var cell3 = row.insertCell(3);
                        var cell4 = row.insertCell(4);
                        var cell5 = row.insertCell(5);
                        var cell6 = row.insertCell(6);
                        var cell7 = row.insertCell(7);
                        var cell8 = row.insertCell(8);


                        cell0.innerHTML = data[0].sr;
                        cell1.innerHTML = data[0].time;
                        cell2.innerHTML = [new Date().getTime() - lasttimelocal.getTime(), parseInt(timectrl) - parseInt(lasttimectrl)].toString();
                        cell3.innerHTML = data[0].temp_filter;
                        cell4.innerHTML = data[0].temp_raw;
                        cell5.innerHTML = data[0].curr_filter;
                        cell6.innerHTML = data[0].curr_raw;
                        cell7.innerHTML = parseFloat(data[0].accel_filter);
                        cell8.innerHTML = data[0].accel_raw;




                        lasttimelocal = new Date(data[0].time);
                        lasttimectrl = data[0].uptm;



                        lineGraph_temp.data.datasets[0].data.push(data[0].temp_filter);
                        lineGraph_temp.data.datasets[1].data.push(data[0].temp_raw);
                        lineGraph_temp.data.labels.push(data[0].time);

                        lineGraph_curr.data.datasets[0].data.push(data[0].curr_filter);
                        lineGraph_curr.data.datasets[1].data.push(data[0].curr_raw);
                        lineGraph_curr.data.labels.push(data[0].time);

                        lineGraph_acc.data.datasets[0].data.push(cell7.innerHTML);
                        lineGraph_acc.data.datasets[1].data.push(cell8.innerHTML);
                        lineGraph_acc.data.labels.push(data[0].time);

                        lineGraph_temp_rad.data.datasets[0].data[0] = data[0].temp_filter;//-10;
                        lineGraph_temp_rad.data.datasets[0].data[1] = 20;//data[0].temp_filter+65;        // range -10 to 65        

                        lineGraph_temp_rad.data.datasets[1].data[0] = data[0].curr_filter;
                        lineGraph_temp_rad.data.datasets[1].data[1] = 20;//data[0].curr_filter+65;        // range -10 to 65        

                        lineGraph_temp_rad.data.datasets[2].data[0] = cell7.innerHTML;
                        lineGraph_temp_rad.data.datasets[2].data[1] = 20;//data[0].acc_filter+65;        // range -10 to 65   

                        lineGraph_temp_rad.options.title.text = (parseFloat(data[0].temp_filter).toFixed(2)).toString() + "°C | " + (parseFloat(data[0].curr_filter).toFixed(2)) + "A | " + (parseFloat(cell7.innerHTML).toFixed(2)) + "dB";

                        //"rgb(54, 235, 162 )",
                        //"rgb(255, 99, 132)",
                        //"rgb(255, 205, 86)"

                        if (data[0].temp_filter <= 0) {

   			    playAudio(severities.WARNING, senses.TEMPERATURE);

                            lineGraph_temp_rad.data.datasets[0].backgroundColor[0] = "rgb(150, 150, 255)";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[1] = "rgb(132, 99, 255)";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                        if (data[0].temp_filter > 0 && data[0].temp_filter <= 16.0) {

			    playAudio(severities.ALERT, senses.TEMPERATURE);

                            var valg = Math.floor(150 + data[0].temp_filter * 4.0).toString(); //150 to 250
                            var valb = Math.floor(250 - data[0].temp_filter * 4.0).toString(); //255 to 150

                            lineGraph_temp_rad.data.datasets[0].backgroundColor[0] = "rgb(150, " + valg + ", " + valb + ")";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[1] = "rgb(220, 200, 200)";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                        if (data[0].temp_filter > 16 && data[0].temp_filter <= 25.0) {
                            var valg = Math.floor(150 + data[0].temp_filter * 4.0).toString(); //150 to 250
                            var valb = Math.floor(250 - data[0].temp_filter * 4.0).toString(); //255 to 150

                            lineGraph_temp_rad.data.datasets[0].backgroundColor[0] = "rgb(150, " + valg + ", " + valb + ")";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[1] = "rgb(220, 200, 200)";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                        if (data[0].temp_filter > 25.0 && data[0].temp_filter <= 35.0) {
                            var valr = Math.floor(150 + data[0].temp_filter * 1.0).toString(); //150 to 185
                            var valg = Math.floor(250 + data[0].temp_filter * 0.5).toString(); //250 to 255
                            var valb = 150;//Math.floor(150-data[0].temp_filter*4.0).toString(); //150 to 150

                            lineGraph_temp_rad.data.datasets[0].backgroundColor[0] = "rgb(" + valr + ", " + valg + ", " + valb + ")";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[1] = "rgb(220, 200, 200)";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                        if (data[0].temp_filter > 35.0 && data[0].temp_filter <= 40.0) {

                            playAudio(severities.INFO, senses.TEMPERATURE);

			    var valr = Math.floor(150 + data[0].temp_filter * 3.0).toString(); //150 to 255
                            var valg = Math.floor(185 + data[0].temp_filter * 2.5).toString(); //185 to 255
                            var valb = Math.floor(150 - data[0].temp_filter * 1.0).toString(); //150 to 100

                            lineGraph_temp_rad.data.datasets[0].backgroundColor[0] = "rgb(" + valr + ", " + valg + ", " + valb + ")";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[1] = "rgb(220, 200, 200)";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                        if (data[0].temp_filter > 40.0 && data[0].temp_filter <= 55.0) {
				
			    playAudio(severities.ALERT, senses.TEMPERATURE);

                            lineGraph_temp_rad.data.datasets[0].backgroundColor[0] = "rgb(205, 10, 10)";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[1] = "rgb(220, 150, 150)";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                        if (data[0].temp_filter > 55.0) {
				
			    playAudio(severities.WARNING, senses.TEMPERATURE);

                            lineGraph_temp_rad.data.datasets[0].backgroundColor[0] = "rgb(10, 0, 0)";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[1] = "rgb(220, 200, 200)";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[2] = "rgb(255, 205, 86)";
                        }


                        if (data[0].curr_filter <= 1) {

			    playAudio(severities.NONE, senses.CURRENT);

                            lineGraph_temp_rad.data.datasets[1].backgroundColor[0] = "rgb(54, 235, 162)";
                            lineGraph_temp_rad.data.datasets[1].backgroundColor[1] = "rgb(245, 255, 245)";
                            lineGraph_temp_rad.data.datasets[1].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                        if (data[0].curr_filter > 1.0 && data[0].curr_filter <= 8.0) {
			    
	 
			    playAudio(severities.INFO, senses.CURRENT);

			    lineGraph_temp_rad.data.datasets[1].backgroundColor[0] = "rgb(235, 235, 162)";
                            lineGraph_temp_rad.data.datasets[1].backgroundColor[1] = "rgb(255, 200, 200)";
                            lineGraph_temp_rad.data.datasets[1].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                         if (data[0].curr_filter > 8.0 && data[0].curr_filter <= 16.0) {
			    
	 
			    playAudio(severities.ALERT, senses.CURRENT);

			    lineGraph_temp_rad.data.datasets[1].backgroundColor[0] = "rgb(235, 235, 162)";
                            lineGraph_temp_rad.data.datasets[1].backgroundColor[1] = "rgb(255, 200, 200)";
                            lineGraph_temp_rad.data.datasets[1].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

			if (data[0].curr_filter > 16.0) {
		 
			    playAudio(severities.WARNING, senses.CURRENT);

			    lineGraph_temp_rad.data.datasets[1].backgroundColor[0] = "rgb(235, 54, 54)";
                            lineGraph_temp_rad.data.datasets[1].backgroundColor[1] = "rgb(255, 240, 240)";
                            lineGraph_temp_rad.data.datasets[1].backgroundColor[2] = "rgb(255, 205, 86)";
                        }


                        // data[0].acc_filter === cell7.innerHTML =>1
                        if (cell7.innerHTML <= 0.1) {
				
			    playAudio(severities.INFO, senses.VIBRATION);

                            lineGraph_temp_rad.data.datasets[2].backgroundColor[0] = "rgb(54, 235, 162)";
                            lineGraph_temp_rad.data.datasets[2].backgroundColor[1] = "rgb(54, 235, 162)";
                            lineGraph_temp_rad.data.datasets[2].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                        if (cell7.innerHTML > 0.1 && cell7.innerHTML <= 1.0) {
			    	
			    playAudio(severities.ALERT, senses.VIBRATION);

                            lineGraph_temp_rad.data.datasets[2].backgroundColor[0] = "rgb(162, 205, 54)";
                            lineGraph_temp_rad.data.datasets[2].backgroundColor[1] = "rgb(255, 240, 200)";
                            lineGraph_temp_rad.data.datasets[2].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                        if (cell7.innerHTML > 1.0 && cell7.innerHTML <= 3.0) {

			    playAudio(severities.WARNING, senses.VIBRATION);

                            lineGraph_temp_rad.data.datasets[2].backgroundColor[0] = "rgb(235, 235, 162)";
                            lineGraph_temp_rad.data.datasets[2].backgroundColor[1] = "rgb(255, 20, 20)";
                            lineGraph_temp_rad.data.datasets[2].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                        if (cell7.innerHTML > 3.0) {
			
			    playAudio(severities.CRITICAL, senses.VIBRATION);

                            lineGraph_temp_rad.data.datasets[2].backgroundColor[0] = "rgb(235, 54, 54)";
                            lineGraph_temp_rad.data.datasets[2].backgroundColor[1] = "rgb(255, 40, 40)";
                            lineGraph_temp_rad.data.datasets[2].backgroundColor[2] = "rgb(255, 205, 86)";
                        }


                        if (table.rows.length > 20) {
                            lineGraph_temp.data.datasets[0].data.shift();
                            lineGraph_temp.data.datasets[1].data.shift();
                            lineGraph_temp.data.labels.shift();

                            lineGraph_curr.data.datasets[0].data.shift();
                            lineGraph_curr.data.datasets[1].data.shift();
                            lineGraph_curr.data.labels.shift();

                            lineGraph_acc.data.datasets[0].data.shift();
                            lineGraph_acc.data.datasets[1].data.shift();
                            lineGraph_acc.data.labels.shift();

                            table.deleteRow(table.rows.length - 1);

                        }

                        lineGraph_temp.update(500);
                        lineGraph_curr.update(500);
                        lineGraph_acc.update(500);
                        lineGraph_temp_rad.update(500);
                        lastdata = data[0];
                    }

                    if (SIMULATE == true) {
                        updateInsertStr();
                    }
                });
            }
        }





        var inc_sr = 0;
        var rnd_uptm = 0;
        var temp_inc = -2.0;
        function getDateString() {
            const date = new Date();
            const year = date.getFullYear();
            const month = `${date.getMonth() + 1}`.padStart(2, '0');
            const day = `${date.getDate()}`.padStart(2, '0');
            const hour = `${date.getHours()}`.padStart(2, '0');
            const min = `${date.getMinutes()}`.padStart(2, '0');
            const sec = `${date.getSeconds()}`.padStart(2, '0');

            return `${year}-${month}-${day} ${hour}:${min}:${sec}`;
        }

function getDateString(val) {
            const date = new Date(0);
	    date.setUTCSeconds(val);
            const year = date.getFullYear();
            const month = `${date.getMonth() + 1}`.padStart(2, '0');
            const day = `${date.getDate()}`.padStart(2, '0');
            const hour = `${date.getHours()}`.padStart(2, '0');
            const min = `${date.getMinutes()}`.padStart(2, '0');
            const sec = `${date.getSeconds()}`.padStart(2, '0');

            return `${year}-${month}-${day} ${hour}:${min}:${sec}`;
        }
 
var log_timer;

function revertMsg()
{

if(!log_timer)
{
	clearTimeout(log_timer);
}
	var icn = ""; var log_img = document.getElementById("log_img");var div_log = document.getElementById("logs");
	if(!log_img) 
	{
	}
	else
	{
	log_img.src = "imgs/waiting.png";
	}
	if(!div_log) 
	{
	}
	else
	{
	div_log.innerHTML = "'<b>Waiting for events ... </b>'<br>msgs will be here";
	}
	
}
            function showNotifaication(sense, title, msg) {

log_timer = setTimeout(revertMsg, 65000); 
                var icn = ""; var log_img = document.getElementById("log_img");var div_log = document.getElementById("logs");


 				switch(sense)
				    {
					case senses.TEMPERATURE:
				    	{
						icn = "imgs/hightemp.png";
		 		    	}
					break;

				        case senses.CURRENT:
				    	{
						icn = "imgs/highcurr.svg";
		 		    	}

				        case senses.VIBRATION:
				    	{
					icn = "imgs/earthquake.png";
		 		    	}
				    }
if(!log_img) 
{
}
else
{
log_img.src = icn;
}
if(!div_log) 
{
}
else
{
div_log.innerHTML = "'<b>"+title+"</b>'<br>"+msg;
}




                if (!("Notification" in window)) {
                    alert("Desktop notifications is not supported by this browser. Try another.");
                    return;
                } else if (Notification.permission === "granted") {
			
	            //var icn = "";
		     switch(sense)
				    {
					case senses.TEMPERATURE:
				    	{
						icn = "imgs/hightemp.png";
		 		    	}
					break;

				        case senses.CURRENT:
				    	{
						icn = "imgs/highcurr.svg";
		 		    	}

				        case senses.VIBRATION:
				    	{
						icn = "imgs/earthquake.png";
		 		    	}
				    }

 
                    var myNotification = new Notification(title, {
                        icon: icn,
                        body: msg
                    });
                    myNotification.onclick = function () {
                        //window.open("https://www.websparrow.org/java/how-to-parse-nested-json-object-in-java");
                    };

                } else if (Notification.permission === 'denied') {
                    Notification.requestPermission(function (userPermission) {
                        if (userPermission === "granted") {

				    switch(sense)
				    {
					case senses.TEMPERATURE:
				    	{
						icn = "imgs/hightemp.png";
		 		    	}
					break;

				        case senses.CURRENT:
				    	{
						icn = "imgs/highcurr.svg";
		 		    	}

				        case senses.VIBRATION:
				    	{
						icn = "imgs/earthquake.png";
		 		    	}
				    }

                            var myNotification = new Notification(title, {
                                 icon: icn,
                                body: msg
                            });
                            myNotification.onclick = function () {
                                //window.open("https://www.websparrow.org/spring/");
                            };
                            // setTimeout(myNotification.close.bind(myNotification), 5000);
                        }
                    });
                }
            } 
/*
	const severities = 
	{
	    NONE: 'none',
	    INFO: 'info',
	    ALERT: 'alert',
	    WARNING: 'warning',
	    CRITICAL: 'critical'
	}

	const senses =
	{
		TEMPERATURE: 'temperature',
		CURRENT: 'current',
		VIBRATION: 'vibration'
	}


	let severity_status = {
	  ['temperature_severity', severities.NONE],
	  ['current_info', severities.NONE],
	  ['vibration_info', severities.NONE]
	};
*/
 
	function playAudio(severity, sense)
	{
		if(!severity)
		{
    			throw new Error('Severity is not defined')
		}

		if(!sense)
		{
    			throw new Error('sense is not defined')
		}

//			var played_sound_once_per_event_per_
		
		var audio = new Audio('audios/Info_S-Beeping.mp3');

		switch(severity) {
	          case severities.NONE:
			
			switch(sense)
			{
				case senses.TEMPERATURE:
					if(severity_states.temperature_severity!=severity)
					{			
						//var audio = new Audio('audios/Bleep.mp3');
						//audio.play();
						severity_states.temperature_severity = severity;
					}
				break;

				case senses.CURRENT:
					if(severity_states.current_severity!=severity)
					{			
						//var audio = new Audio('audios/Bleep.mp3');
						//audio.play();
						severity_states.current_severity=severity;
					}
				break;	

				case senses.VIBRATION:
					if(severity_states.vibration_severity!=severity)
					{			
						//var audio = new Audio('audios/Bleep.mp3');
						//audio.play();
						severity_states.vibration_severity=severity;
					}
				break;
		  
				default:
					{
				    		// code block
				        }
			}				
			 
			
			

		  break;
		  case severities.INFO:

			audio.src = 'audios/Info_S-Beeping.mp3';

			switch(sense)
			{ 
				case senses.TEMPERATURE:
					if(severity_states.temperature_severity!=severity)
					{	
						audio.play();
						severity_states.temperature_severity = severity;
					}
				break;

				case senses.CURRENT:
					if(severity_states.current_severity!=severity)
					{	 
						audio.play();
						severity_states.current_severity=severity;
					}
				break;	

				case senses.VIBRATION:
					if(severity_states.vibration_severity!=severity)
					{			
						audio.play();
						severity_states.vibration_severity=severity;
					}
				break;
		  
				default:
					{
				    		// code block
				        }
			}				
			 
			
			
		    // code block
		    break;
		  case severities.ALERT:
			
			audio.src = 'audios/Alert_Bleep.mp3';

			switch(sense)
			{
				
				case senses.TEMPERATURE:
					if(severity_states.temperature_severity!=severity)
					{			
						audio.play();
						showNotifaication(sense, "Temperature Alert", "Good time to switch on the AC. <br>Check predictions for temperature drop if there are.")
						severity_states.temperature_severity = severity;
					}
				break;

				case senses.CURRENT:
					if(severity_states.current_severity!=severity)
					{			
						audio.play();
						severity_states.current_severity=severity;
					}
				break;	

				case senses.VIBRATION:
					if(severity_states.vibration_severity!=severity)
					{			
						audio.play();
						updateSeverityStatusColors(); // To test it is here, after correction it will be before switch.switch
						showNotifaication(sense, "Earthquake Alert", "Seems ground is shaking. <br>Ready to perform actions for safety.")
						severity_states.vibration_severity=severity;
					}
				break;
		  
				default:
					{
				    		// code block
				        }
			}			
		    break;
		  case severities.WARNING:

			audio.src = 'audios/Alert_Bleep.mp3';

			switch(sense)
			{
						
				case senses.TEMPERATURE:
					if(severity_states.temperature_severity!=severity)
					{			
						audio.play();
						severity_states.temperature_severity = severity;
					}
				break;

				case senses.CURRENT:
					if(severity_states.current_severity!=severity)
					{			
						audio.play();
						showNotifaication(sense, "Device Warning", "Seems AC is switched on. <br>Check in case the wheather is good the AC can be switched off.")
						severity_states.current_severity=severity;
					}
				break;	

				case senses.VIBRATION:
					if(severity_states.vibration_severity!=severity)
					{			
						audio.play();
						showNotifaication(sense, "Earthquake Warning", "Seems ground is shaking hard. <br>Safety is primary goal.")
						severity_states.vibration_severity=severity;
					}
				break;
		  
				default:
					{
				    		// code block
				        }
				setTimeout(audio.play,100);
			}				
			 
			
			
		    break;
		  case severities.CRITICAL:

			audio.src = 'audios/Critical_code-3-horn.mp3';
				
			switch(sense)
			{
						
				case senses.TEMPERATURE:
					if(severity_states.temperature_severity!=severity)
					{			
						audio.play();
						showNotifaication(sense, "Temperature Critical", "Seems it is too hot here. <br>Switch on the AC must.")
						severity_states.temperature_severity = severity;
					}
				break;

				case senses.CURRENT:
					if(severity_states.current_severity!=severity)
					{			
						audio.play();
						showNotifaication(sense, "Current Critical", "Seems device is consuming too much of current. <br>Switch off the device.")
						severity_states.current_severity=severity;
					}
				break;	

				case senses.VIBRATION:
					if(severity_states.vibration_severity!=severity)
					{			
						audio.play();
						showNotifaication(sense, "Earthquake Critical", "Heavy earthquake. <br>Run for safety.")
						severity_states.vibration_severity=severity;
					}
				break;
		  
				default:
					{
				    		// code block
				        }
			}				
			
		    break;
		  default:
		    // code block
		}
	}



        let rad_chart_color = 
	{
		vibration: {
			info: ["rgb(54, 235, 162)", "rgb(54, 235, 162)", "rgb(255, 205, 86)"],
			alert: ["rgb(162, 205, 54)", "rgb(255, 240, 200)", "rgb(255, 205, 86)"],
			warn: ["rgb(235, 235, 162)", "rgb(255, 20, 20)", "rgb(255, 205, 86)"],
			critical: ["rgb(235, 54, 54)", "rgb(255, 40, 40)", "rgb(255, 205, 86)"]
                }
		// ,
                // current ...


	};

 
        let toggle_blink = 0;
	function blinkForSeverity()
	{
		if(0 == toggle_blink)
		{
			lineGraph_temp_rad.data.datasets[2].backgroundColor[0] = "rgb(100, 100, 100)";
			lineGraph_temp_rad.data.datasets[2].backgroundColor[1] = "rgb(100, 100, 100)";
			lineGraph_temp_rad.data.datasets[2].backgroundColor[2] = "rgb(255, 205, 86)";
			toggle_blink = 1;
		}

		else if(1 == toggle_blink)
		{
			lineGraph_temp_rad.data.datasets[2].backgroundColor[0] = "rgb(162, 205, 54)";
			lineGraph_temp_rad.data.datasets[2].backgroundColor[1] = "rgb(255, 240, 200)";
			lineGraph_temp_rad.data.datasets[2].backgroundColor[2] = "rgb(255, 205, 86)";
			toggle_blink = 0;
		}

                lineGraph_temp_rad.update(250);

	}

        function updateSeverityStatusColors()
        {

		let timer_handle;

		/*if(severity_states.vibration_severity == severities.WARNING)
		{

			setInterval(blinkForSeverity, 250);
			// info
			lineGraph_temp_rad.data.datasets[2].backgroundColor[0] = "rgb(54, 235, 162)";
			lineGraph_temp_rad.data.datasets[2].backgroundColor[1] = "rgb(54, 235, 162)";
			lineGraph_temp_rad.data.datasets[2].backgroundColor[2] = "rgb(255, 205, 86)";


			// alert
			lineGraph_temp_rad.data.datasets[2].backgroundColor[0] = "rgb(162, 205, 54)";
			lineGraph_temp_rad.data.datasets[2].backgroundColor[1] = "rgb(255, 240, 200)";
			lineGraph_temp_rad.data.datasets[2].backgroundColor[2] = "rgb(255, 205, 86)";


			// warn
			lineGraph_temp_rad.data.datasets[2].backgroundColor[0] = "rgb(235, 235, 162)";
                        lineGraph_temp_rad.data.datasets[2].backgroundColor[1] = "rgb(255, 20, 20)";
                        lineGraph_temp_rad.data.datasets[2].backgroundColor[2] = "rgb(255, 205, 86)";

			
			// critical
			lineGraph_temp_rad.data.datasets[2].backgroundColor[0] = "rgb(235, 54, 54)";
			lineGraph_temp_rad.data.datasets[2].backgroundColor[1] = "rgb(255, 40, 40)";
			lineGraph_temp_rad.data.datasets[2].backgroundColor[2] = "rgb(255, 205, 86)";
			
		}*/

		if(severity_states.vibration_severity == severities.ALERT)
		{

			timer_handle = setInterval(blinkForSeverity, 250);
                        setTimeout(clearInterval, 5000, timer_handle );
			/*// info
			lineGraph_temp_rad.data.datasets[2].backgroundColor[0] = "rgb(54, 235, 162)";
			lineGraph_temp_rad.data.datasets[2].backgroundColor[1] = "rgb(54, 235, 162)";
			lineGraph_temp_rad.data.datasets[2].backgroundColor[2] = "rgb(255, 205, 86)";


			// alert
			lineGraph_temp_rad.data.datasets[2].backgroundColor[0] = "rgb(162, 205, 54)";
			lineGraph_temp_rad.data.datasets[2].backgroundColor[1] = "rgb(255, 240, 200)";
			lineGraph_temp_rad.data.datasets[2].backgroundColor[2] = "rgb(255, 205, 86)";


			// warn
			lineGraph_temp_rad.data.datasets[2].backgroundColor[0] = "rgb(235, 235, 162)";
                        lineGraph_temp_rad.data.datasets[2].backgroundColor[1] = "rgb(255, 20, 20)";
                        lineGraph_temp_rad.data.datasets[2].backgroundColor[2] = "rgb(255, 205, 86)";

			
			// critical
			lineGraph_temp_rad.data.datasets[2].backgroundColor[0] = "rgb(235, 54, 54)";
			lineGraph_temp_rad.data.datasets[2].backgroundColor[1] = "rgb(255, 40, 40)";
			lineGraph_temp_rad.data.datasets[2].backgroundColor[2] = "rgb(255, 205, 86)";*/
			
		}

	}
	


        function updateInsertStr() {

            var min_t = -20.0;
            var max_t = 20.0;
            temp_inc = temp_inc + 4;
            temp_inc = temp_inc > 70 ? -2 : temp_inc;

            var random_t = 35.0 + Math.random() * (+max_t - +min_t) + +min_t;

            var min_c = 0.0;
            var max_c = 10.0;
            var random_c = Math.random() * (+max_c - +min_c) + +min_c;

            var min_a = -1.0;
            var max_a = 4.0;
            var random_a = 1.0 + Math.random() * (+max_a - +min_a) + +min_a;

            inc_sr = inc_sr + 1;
            rnd_uptm = rnd_uptm + Math.floor(Math.random() * 1000);

            var query_str =
                "sr=" + inc_sr.toString() + "&dt='" + [Math.floor(Math.random() * 1000), Math.floor(Math.random() * 3000)].toString() + "'&time='" + getDateString() + "'&uptm=" + rnd_uptm.toString() + "&temp_filter=" + random_t.toString() + "&temp_raw=" + (random_t + Math.random()).toString() + "&curr_filter=" + random_c.toString() + "&curr_raw=" + (random_c + Math.random()).toString() + "&accel_filter=" + random_a.toString() + "&accel_raw=" + (random_a + Math.random()).toString();

            document.getElementById("text_display").innerHTML = query_str;

            var strURL = "http://localhost:8000/insert_new_data.php?" + query_str;

            var req = getXMLHTTP();

            if (req) {
                req.onreadystatechange = function () {
                    if (req.readyState == 4) {
                        // only if "OK"
                        if (req.status == 200) {
                            //console.log(req.responseText);
                            if (0 == (req.responseText == 'New record created successfully.')) {
                                // open next page here itself
                                ///$.post("insert_new_data.php", function (data)
                                {
                                    // it never came here
                                    //console.log(data);
                                    document.getElementById("output_display").innerHTML = "Data inserted";
                                }
                                //);
                            }
                            else {
                                console.error('Can\'t open page');
                                document.getElementById("output_display").innerHTML = "Some error: " + req.responseText;
                            }
                        }
                        else {
                            console.error("There was a problem while using XMLHTTP:\n" + req.statusText);
                        }
                    }
                };
            }
            req.open("GET", strURL, true);
            req.setRequestHeader("If-Modified-Since", "Sat, 1 Jan 2000 00:00:00 GMT");
            req.send(null);
        }



    </script>

</body>

</html>
