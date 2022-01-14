const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>


<head>
    <title>ESP8266 Web Console</title>
    <!-- Font Awesome -->
    <link href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.1/css/all.min.css" rel="stylesheet" />
    <!-- Google Fonts -->
    <link href="https://fonts.googleapis.com/css?family=Roboto:300,400,500,700&display=swap" rel="stylesheet" />
    <!-- MDB -->
    <link href="https://cdnjs.cloudflare.com/ajax/libs/mdb-ui-kit/3.10.1/mdb.min.css" rel="stylesheet" />

    <!-- MDB -->
    <script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/mdb-ui-kit/3.10.1/mdb.min.js"></script>
</head>

<body>

    <div class="container-fluid">
        <p>
            ENCODER_A:
        </p>

        <!-- <div class="progress">
            <div id="" class="progress-bar progress-bar-animated" role="progressbar" style="width: 75%;" aria-valuemin="0"
                aria-valuemax="255" aria-valuenow="50" >
            </div>
        </div> -->

        <span id="enc_a">0</span>

        <p>
            ENCODER_C
        </p>
        <span id="enc_c">0</span>

        <hr />

        <p>
            Odometer is:
        </p>
        <span id="odometer">0</span>
    </div>
</body>


</body>

<script>
    setInterval(function () {
        getData();
    }, 500);
    function getData() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("enc_a").innerHTML =
                    this.responseText;
            }
        };
        xhttp.open("GET", "update_ena", true);
        xhttp.send();

        var yhttp = new XMLHttpRequest();
        yhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("enc_c").innerHTML =
                    this.responseText;
            }
        };
        yhttp.open("GET", "update_enc", true);
        yhttp.send();

        var zhttp = new XMLHttpRequest();
        zhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("odometer").innerHTML =
                    this.responseText;
            }
        };
        zhttp.open("GET", "update_odometer", true);
        zhttp.send();
    }
</script>

</html>
)=====";
