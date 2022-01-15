const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>


<head>
    <title>ESP8266 Web Console</title>

    <script src="https://cdn.jsdelivr.net/npm/mdui@1.0.2/dist/js/mdui.min.js"></script>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/mdui@1.0.2/dist/css/mdui.min.css" />

</head>

<body class="mdui-theme-layout-dark">

    <div class="mdui-appbar">
        <div class="mdui-toolbar mdui-color-theme">
            <a href="javascript:;" class="mdui-btn mdui-btn-icon">
                <i class="mdui-icon material-icons">menu</i>
            </a>
            <a href="javascript:;" class="mdui-typo-headline">NodeMCU Web Console</a>
            <div class="mdui-toolbar-spacer"></div>
            <a href="javascript:;" class="mdui-btn mdui-btn-icon">
                <i class="mdui-icon material-icons">refresh</i>
            </a>

        </div>
    </div>

    <div class="mdui-container-fluid">

        <div class="mdui-card mdui-m-t-1">
            <div class="mdui-card-primary">
                <div class="mdui-card-primary-title">Varaible Values:</div>
            </div>
            <div class="mdui-card-content">
                <ul class="mdui-list">
                    <li class="mdui-list-item">
                        <i class="mdui-list-item-icon mdui-icon material-icons">adjust</i>
                        <div class="mdui-list-item-content">ENCODER_A:</div>
                        <span id="enc_a" style="font-weight:500;font-size: x-large;">0</span>
                    </li>
                    <li class="mdui-list-item">
                        <i class="mdui-list-item-icon mdui-icon material-icons">adjust</i>
                        <div class="mdui-list-item-content">ENCODER_C:</div>
                        <span id="enc_c" style="font-weight:500;font-size: x-large;">0</span>
                    </li>
                    <li class="mdui-list-item">
                        <i class="mdui-list-item-icon mdui-icon material-icons">av_timer</i>
                        <div class="mdui-list-item-content">ODOMETER:</div>
                        <span id="odometer" style="font-weight:500;font-size: x-large;">0</span>
                    </li>
                </ul>

                <div class="mdui-divider"></div>

                According to current Javascript file, every 500 ms the values above will be updated.
            </div>

            <div class="mdui-card-actions">
                <button class="mdui-btn mdui-ripple mdui-color-blue-800" onclick="send(0)">Start Sequence</button>
                <button class="mdui-btn mdui-ripple mdui-color-grey-400" onclick="send(1)">Clear Encoders and
                    odometer</button>

                <button class="mdui-btn mdui-ripple mdui-color-red-500" onclick="send(2)">Restart</button>
            </div>
        </div>

        <div class="mdui-card mdui-m-t-1 mdui-m-b-1">
            <div class="mdui-card-primary">
                <div class="mdui-card-primary-title">Vehicle status:</div>
            </div>
            <div class="mdui-card-content">
                <ul>
                    <li class="mdui-list-item">
                        <i class="mdui-list-item-icon mdui-icon material-icons">slow_motion_video</i>
                        <div class="mdui-list-item-content">LEFT_SPEED: (cm/s)</div>
                        <span id="left_spd" style="font-weight:500;font-size: x-large;">0</span>
                    </li>
                    <li class="mdui-list-item">
                        <i class="mdui-list-item-icon mdui-icon material-icons">slow_motion_video</i>
                        <div class="mdui-list-item-content">RIGHT_SPEED: (cm/s)</div>
                        <span id="right_spd" style="font-weight:500;font-size: x-large;">0</span>
                    </li>
                </ul>
            </div>
        </div>

    </div>
</body>


</body>

<script>
    function send(para) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {

            }
        };
        xhttp.open("GET", "act?selector=" + para, true);
        xhttp.send();
    }

    setInterval(function () {
        getData();
    }, 500);

    function send(para) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {

            }
        };
        xhttp.open("GET", "act?selector=" + para, true);
        xhttp.send();
    }

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
