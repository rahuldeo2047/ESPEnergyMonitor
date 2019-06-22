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

        /* Create three equal columns that floats next to each other */
        .column {
            float: left;
            width: 50%;
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
    </style>
    <script type="text/javascript" src="js/jquery.min.js"></script>
    <script type="text/javascript" src="js/Chart.min.js"></script>
    <style>
        canvas {
            -moz-user-select: none;
            -webkit-user-select: none;
            -ms-user-select: none;
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
</head>

<body>
    <div class="row">
        <div class="column">
            <canvas id="graphCanvas_temp" style="border:1px solid #000000;"></canvas>
        </div>
        <div class="column">
            <canvas id="graphCanvas_curr" style="border:1px solid #000000;"></canvas>
        </div>
    </div>
    <div class="row">
        <div class="column">
            <canvas id="graphCanvas_acc" style="border:1px solid #000000;"></canvas>
        </div>
        <div class="column">
            <canvas id="graphCanvas_temp_rad" style="border:1px solid #000000;"></canvas>
        </div>
        <progress id="animationProgress" max="1" value="0.5" style="width: 100%"></progress>
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
        $(document).ready(function () {
            showGraph();
            setInterval(updateChart, 1000);
        });

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
                            data: chartdata_curr
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
                            data: chartdata_acc
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

                        lineGraph_temp_rad.options.title.text = (parseFloat(data[0].temp_filter).toFixed(2)).toString() + "*C | " + (parseFloat(data[0].curr_filter).toFixed(2)) + "A | " + (parseFloat(cell7.innerHTML).toFixed(2)) + "G";

                        //"rgb(54, 235, 162 )",
                        //"rgb(255, 99, 132)",
                        //"rgb(255, 205, 86)"

                        if (data[0].temp_filter <= 0) {
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[0] = "rgb(150, 150, 255)";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[1] = "rgb(132, 99, 255)";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                        if (data[0].temp_filter > 0 && data[0].temp_filter <= 25.0) {
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

                            var valr = Math.floor(150 + data[0].temp_filter * 3.0).toString(); //150 to 255
                            var valg = Math.floor(185 + data[0].temp_filter * 2.5).toString(); //185 to 255
                            var valb = Math.floor(150 - data[0].temp_filter * 1.0).toString(); //150 to 100

                            lineGraph_temp_rad.data.datasets[0].backgroundColor[0] = "rgb(" + valr + ", " + valg + ", " + valb + ")";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[1] = "rgb(220, 200, 200)";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                        if (data[0].temp_filter > 40.0 && data[0].temp_filter <= 55.0) {
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[0] = "rgb(205, 10, 10)";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[1] = "rgb(220, 150, 150)";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                        if (data[0].temp_filter > 55.0) {
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[0] = "rgb(10, 0, 0)";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[1] = "rgb(220, 200, 200)";
                            lineGraph_temp_rad.data.datasets[0].backgroundColor[2] = "rgb(255, 205, 86)";
                        }


                        if (data[0].curr_filter <= 1) {
                            lineGraph_temp_rad.data.datasets[1].backgroundColor[0] = "rgb(54, 235, 162)";
                            lineGraph_temp_rad.data.datasets[1].backgroundColor[1] = "rgb(245, 255, 245)";
                            lineGraph_temp_rad.data.datasets[1].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                        if (data[0].curr_filter > 1.0 && data[0].curr_filter <= 6.0) {
                            lineGraph_temp_rad.data.datasets[1].backgroundColor[0] = "rgb(235, 235, 162)";
                            lineGraph_temp_rad.data.datasets[1].backgroundColor[1] = "rgb(255, 200, 200)";
                            lineGraph_temp_rad.data.datasets[1].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                        if (data[0].curr_filter > 6.0) {
                            lineGraph_temp_rad.data.datasets[1].backgroundColor[0] = "rgb(235, 54, 54)";
                            lineGraph_temp_rad.data.datasets[1].backgroundColor[1] = "rgb(255, 240, 240)";
                            lineGraph_temp_rad.data.datasets[1].backgroundColor[2] = "rgb(255, 205, 86)";
                        }


                        // data[0].acc_filter === cell7.innerHTML =>1
                        if (cell7.innerHTML <= 1.05) {
                            lineGraph_temp_rad.data.datasets[2].backgroundColor[0] = "rgb(54, 235, 162)";
                            lineGraph_temp_rad.data.datasets[2].backgroundColor[1] = "rgb(54, 235, 162)";
                            lineGraph_temp_rad.data.datasets[2].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                        if (cell7.innerHTML > 1.05 && cell7.innerHTML <= 2.0) {
                            lineGraph_temp_rad.data.datasets[2].backgroundColor[0] = "rgb(235, 235, 162)";
                            lineGraph_temp_rad.data.datasets[2].backgroundColor[1] = "rgb(255, 20, 20)";
                            lineGraph_temp_rad.data.datasets[2].backgroundColor[2] = "rgb(255, 205, 86)";
                        }

                        if (cell7.innerHTML > 3.0) {
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
