const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<head>
    <title>ESP8266 Web Console</title>

    <script src="https://cdn.jsdelivr.net/npm/mdui@1.0.2/dist/js/mdui.min.js"></script>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/mdui@1.0.2/dist/css/mdui.min.css" />
    <link href="https://cdnjs.cloudflare.com/ajax/libs/mdb-ui-kit/3.10.1/mdb.min.css" rel="stylesheet" />

    <script src="https://cdn.bootcdn.net/ajax/libs/jquery/3.6.0/jquery.min.js"></script>
    <!-- used for chart -->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/3.7.0/chart.min.js"
        integrity="sha512-TW5s0IT/IppJtu76UbysrBH9Hy/5X41OTAbQuffZFU6lQ1rdcLHzpU5BzVvr/YFykoiMYZVWlr/PX1mDcfM9Qg=="
        crossorigin="anonymous" referrerpolicy="no-referrer"></script>
    <!-- used for chart -->
</head>

<body class="mdui-theme-layout-dark">

    <div class="mdui-appbar">
        <div class="mdui-toolbar mdui-color-theme">
            <a href="javascript:;" class="mdui-btn mdui-btn-icon">
                <i class="mdui-icon material-icons">menu</i>
            </a>
            <p class="mdui-typo-headline">NodeMCU Web Console</p>
            <div class="mdui-toolbar-spacer"></div>
            <a href="" class="mdui-btn mdui-btn-icon">
                <i class="mdui-icon material-icons">refresh</i>
            </a>

        </div>
    </div>

    <div class="mdui-container-fluid">

        <div class="mdui-row">

            <div class="mdui-p-a-1 mdui-col-md-6 mdui-col-xs-12 mdui-hidden-sm-down">
                <div class="mdui-card mdui-m-t-1">
                    <div class="mdui-p-a-2" style="height: 52vh;">
                        <canvas id="velocity-chart"></canvas>
                    </div>
                    <!-- <button class="mdui-btn" onclick="addData(chart, 'null', 23, 25)"> test</button> -->

                    <script>
                        const ctx = document.getElementById('velocity-chart').getContext('2d');
                        var chart = new Chart(ctx, {

                            type: 'line',
                            data: {
                                labels: [],
                                datasets: [
                                    {
                                        label: 'Left',
                                        data: [],
                                        backgroundColor: 'rgb(54, 162, 235)',
                                        borderColor: 'rgb(54, 162, 235)',
                                        tension: 0.5
                                    },
                                    {
                                        label: 'Right',
                                        data: [],
                                        backgroundColor: 'rgb(255, 99, 132)',
                                        borderColor: 'rgb(255, 99, 132)',
                                        tension: 0.5

                                    }
                                ],
                                options: {
                                    responsive: true,
                                    maintainAspectRatio: false
                                }
                            }
                        });

                        function addData(chart, label, data1, data2) {

                            // if (chart.data.datasets[0].data.length >= 16) {
                            //     chart.data.datasets[0].data.shift();
                            //     chart.data.datasets[1].data.shift();
                            //     chart.data.labels.shift();
                            // }
                            
                            // 用于删除之前数据

                            chart.data.datasets[0].data.push(data1);
                            chart.data.datasets[1].data.push(data2);
                            chart.data.labels.push(label);

                            chart.update();
                        }
                    </script>

                </div>
            </div>

            <div class="mdui-p-a-1 mdui-col-md-6 mdui-col-xs-12">
                <div class="mdui-card mdui-m-t-1">
                    <div class="mdui-card-primary">
                        <div class="mdui-card-primary-title">Varaible Values:</div>
                    </div>
                    <div class="mdui-card-content">
                        <ul class="mdui-list">
                            <li class="mdui-list-item">
                                <i class="mdui-list-item-icon mdui-icon material-icons">adjust</i>
                                <div class="mdui-list-item-content">ENCODER_L:</div>
                                <span id="enc_c" style="font-weight:500;font-size: x-large;">0</span>
                            </li>
                            <li class="mdui-list-item">
                                <i class="mdui-list-item-icon mdui-icon material-icons">adjust</i>
                                <div class="mdui-list-item-content">ENCODER_R:</div>
                                <span id="enc_a" style="font-weight:500;font-size: x-large;">0</span>
                            </li>
                            <!-- 本编码器显示顺序为白线接D5,黑线接D6 编码器输入不变情况 -->
                            <li class="mdui-list-item">
                                <i class="mdui-list-item-icon mdui-icon material-icons">av_timer</i>
                                <div class="mdui-list-item-content">ODOMETER:</div>
                                <span id="odometer" style="font-weight:500;font-size: x-large;">0</span>
                            </li>
                        </ul>

                        <div class="mdui-divider"></div>

                        According to current Javascript file, every 500 ms the values above will be updated.
                    </div>

                    <div class="mdui-card-actions ">

                        <button class="mdui-btn mdui-ripple mdui-color-blue-800 " onclick="send(0)">
                            Start Sequence
                        </button>

                        <button class="mdui-btn mdui-ripple mdui-color-orange-400" onclick="send(2)">
                            Stop Chart Update
                        </button>

                        <button class="mdui-btn mdui-ripple mdui-color-grey-400" onclick="send(1)">
                            Clear Encoders / Odometer
                        </button>

                        <!-- <button class="mdui-btn mdui-ripple mdui-color-red-500" onclick="send(2)">Restart</button> -->
                    </div>
                </div>
            </div>
        </div>

        <!-- 第二级 -->

        <div class="mdui-row">

            <div class="mdui-p-a-1 mdui-col-md-3 mdui-col-xs-12">
                <div class="mdui-card mdui-m-y-1 mdui-m-r">
                    <div class="mdui-card-primary">
                        <div class="mdui-card-primary-title">Vehicle speed:</div>
                        <div class="mdui-card-primary-subtitle">ticks/100ms</div>
                    </div>
                    <div class="mdui-card-content">
                        <ul>
                            <li class="mdui-list-item">
                                <i class="mdui-list-item-icon mdui-icon material-icons">slow_motion_video</i>
                                <div class="mdui-list-item-content">LEFT: </div>
                                <span id="left_spd" style="font-weight:500;font-size: x-large;">N/A</span>
                            </li>
                            <li class="mdui-list-item">
                                <i class="mdui-list-item-icon mdui-icon material-icons">slow_motion_video</i>
                                <div class="mdui-list-item-content">RIGHT: </div>
                                <span id="right_spd" style="font-weight:500;font-size: x-large;">N/A</span>
                            </li>
                        </ul>
                    </div>
                </div>
            </div>

            <div class="mdui-p-a-1  mdui-col-md-9 mdui-col-xs-12">
                <div class="mdui-card mdui-m-y-1">
                    <div class="mdui-card-primary">
                        <div class="mdui-card-primary-title">PWM Modify:</div>
                        <div class="mdui-card-primary-subtitle">Modify their value instantly!</div>
                    </div>

                    <div class="mdui-card-content">
                        <div class="mdui-container-fluid">
                            <div class="mdui-row">
                                <div class="mdui-col-xs-3">
                                    <p style="margin-top: 5px;">
                                        PWM[1]
                                        <span id="pwm1" class="badge bg-primary mdui-m-l-1">
                                            -1
                                        </span>
                                    </p>
                                    <p class="mdui-m-t-0">
                                        PWM[2]
                                        <span id="pwm2" class="badge bg-primary mdui-m-l-1">
                                            -1
                                        </span>
                                    </p>
                                </div>

                                <div class="mdui-col-7">
                                    <div style="padding-left: 7rem;">
                                        <label class="mdui-slider mdui-slider-discrete">
                                            <input id="pwm_slider_a" type="range" step="1" min="0" max="255" />
                                        </label>
                                    </div>

                                    <div style="padding-left: 7rem; margin-top: 4px;">
                                        <label class="mdui-slider mdui-slider-discrete">
                                            <input id="pwm_slider_b" type="range" step="1" min="0" max="255" />
                                        </label>
                                    </div>
                                </div>

                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>

    </div>

    <!-- <button class="mdui-fab mdui-fab-fixed mdui-ripple mdui-color-blue-700">
        <i class="mdui-icon material-icons">add</i>
    </button> -->

    <div class="mdui-fab-wrapper" mdui-fab="{trigger: 'hover'}">
        <button class="mdui-fab mdui-ripple mdui-color-blue-700">
            <!-- 默认显示的图标 -->
            <i class="mdui-icon material-icons">add</i>

            <!-- 在拨号菜单开始打开时，平滑切换到该图标，若不需要切换图标，则可以省略该元素 -->
            <i class="mdui-icon mdui-fab-opened material-icons">person</i>
        </button>
        <div class="mdui-fab-dial">
            <button class="mdui-fab mdui-fab-mini mdui-ripple mdui-color-pink">
                <i class="mdui-icon material-icons">more</i>
            </button>
            <button class="mdui-fab mdui-fab-mini mdui-ripple mdui-color-red">
                <i class="mdui-icon material-icons">bookmark</i>
            </button>
            <button class="mdui-fab mdui-fab-mini mdui-ripple mdui-color-orange">
                <i class="mdui-icon material-icons">access_alarms</i>
            </button>
            <button class="mdui-fab mdui-fab-mini mdui-ripple mdui-color-blue">
                <i class="mdui-icon material-icons">touch_app</i>
            </button>
        </div>
    </div>
    
</body>


<script>

    var chart_flag = false;

    function send(para) {

        if (para == 0) {
            chart_flag = true;
        } else if(para == 2) {
            chart_flag = false;
        }

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
    }, 350);

    function getData() {

        // var date = new Date();
        // label = date.getHours() + ':' + date.getMinutes() + ':' + date.getSeconds();

        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                const json = this.responseText;
                const jsonObj = JSON.parse(json);
                document.getElementById("enc_a").innerHTML = jsonObj.ena;
                document.getElementById("enc_c").innerHTML = jsonObj.enc;
                document.getElementById("odometer").innerHTML = jsonObj.odometer;

                // for pwms
                document.getElementById("pwm1").innerHTML = jsonObj.pwma;
                document.getElementById("pwm2").innerHTML = jsonObj.pwmb;

                document.getElementById("pwm_slider_a").value = jsonObj.pwma;
                document.getElementById("pwm_slider_b").value = jsonObj.pwmb;
            }
        };
        xhttp.open("GET", "update_varible", true);
        xhttp.send();
        // above is updater for encoder A/C and odometer

        var yhttp = new XMLHttpRequest();
        yhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                const json = this.responseText;
                const jsonObj = JSON.parse(json);
                document.getElementById("left_spd").innerHTML = jsonObj.left;
                document.getElementById("right_spd").innerHTML = jsonObj.right;
                if (chart_flag) {
                    addData(chart, '', jsonObj.left, jsonObj.right);
                }

            }
        };
        yhttp.open("GET", "update_speed", true);
        yhttp.send();

    }

</script>

</html>
)=====";
