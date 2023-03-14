const char site[] = R"=====(
    <!DOCTYPE html>
<html>

<head>
  <title>ESP8266 Web Console</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, shrink-to-fit=no" />
  <meta name="renderer" content="webkit" />
  <meta name="force-rendering" content="webkit" />
  <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1" />
  <link rel="stylesheet" href="./mdui-v1.0.2/css/mdui.min.css" />

  <script src="https://cdn.staticfile.org/echarts/4.3.0/echarts.min.js"></script>
  <script src="./mdui-v1.0.2/js/mdui.min.js"></script>
  <script src="./echarts/echarts.js"></script>
  
</head>

<body class="mdui-theme-layout-dark">

  <div class="mdui-appbar">
    <div class="mdui-toolbar mdui-color-theme">
      <i class="mdui-icon material-icons">developer_mode</i>
      <p class="mdui-typo-headline">NodeMCU Console</p>
      
      <div class="mdui-toolbar-spacer"></div>

      <a href="" class="mdui-btn mdui-btn-icon">
        <i id="status_icon" class="mdui-icon material-icons">cloud_off</i>
      </a>
      <p id="status_text" style="font-weight:500;">
        NOT CONNECTED
      </p>

    </div>
  </div>

  <div class="mdui-container-fluid">

    <div class="mdui-row">
      <!-- First Grid Size Modificator -->
      <div class="mdui-p-a-2 mdui-col-md-4 mdui-col-xs-12">
        <div class="mdui-card mdui-m-t-1">
          <div class="mdui-card-primary">
            <div class="mdui-card-primary-title">Gerneral Status:</div>
            <div class="mdui-card-primary-subtitle">Update per 200ms
            </div>
          </div>
          <div>
            <!-- add a status icon on the right -->
          </div>
          <div class="mdui-card-content">
            <ul class="mdui-list">
              <li class="mdui-list-item">
                <i class="mdui-list-item-icon mdui-icon material-icons">adjust</i>
                <div class="mdui-list-item-content">ENCODER:</div>
                <span id="enc_c" style="font-weight:500;font-size: x-large;">0</span>
              </li>
              
              <li class="mdui-list-item">
                <i class="mdui-list-item-icon mdui-icon material-icons">av_timer</i>
                <div class="mdui-list-item-content">ODOMETER:</div>
                <span id="odometer" style="font-weight:500;font-size: x-large;">0</span>
              </li>
            </ul>

            <div class="mdui-divider"></div>

          </div>

          <div class="mdui-card-actions ">
            <button class="mdui-btn mdui-ripple mdui-color-blue-800 " onclick="notify(0)">
              Initiate
            </button>

            <button class="mdui-btn mdui-ripple mdui-color-grey-300" onclick="notify(1)">
              Clear Encoders / Odometer
            </button>

            <button class="mdui-btn mdui-ripple mdui-color-red-500" onclick="notify(2)">
              Restart
            </button>
          </div>
        </div>
      </div>

      <div id="asd"> </div>

      <!-- Second Grid Size Modificator -->
      <div class="mdui-p-a-2 mdui-col-md-8 mdui-col-xs-12">
        <div class="mdui-card mdui-m-y-1 mdui-m-r">
          <div class="mdui-card-primary">
            <div class="mdui-card-primary-title">Speed Graph:</div>
            <div class="mdui-card-primary-subtitle">Unit: cm/s</div>
          </div>
          <div class="mdui-card-content">

            <div>
              <div id="spd_chart" style="width:100%;height:40vh;" />

              <script>
                var chart = echarts.init(document.getElementById("spd_chart"));
                var option = {
                  xAxis: {
                    data: ["1", "2", "3", "4", "5", "6", "9"]
                  },
                  yAxis: {},
                  series: [{
                    type: 'line',
                    data: [5, 20, 36, 10, 10, 20, 200]
                  }]
                }
                // 针对实时更新数据做优化
                chart.setOption(option)
              </script>

            </div>

            <ul class="mdui-list">
              <li class="mdui-list-item">
                <i class="mdui-list-item-icon mdui-icon material-icons">slow_motion_video</i>
                <div class="mdui-list-item-content">Current Speed: </div>
                <span id="left_spd" style="font-weight:500;font-size: x-large;">N/A</span>
              </li>
            </ul>
          </div>
        </div>
      </div>

    </div>
</body>



<script>

  var status_icon = document.getElementById("status_icon");
  var status_text = document.getElementById("status_text");
  var loop_index = 0;

  // gerneral xhr status check func
  function xhr_check(xhr_package) {
    if (xhr_check.readyState == 4 && xhr_check.status == 200) 
    {
      return true;
    } else {
      return false;
    } 
  }

  function heartbeat() {
    new XMLHttpRequest()
      .open("GET", "heartbeat", false)
      .send()
      .onreadystatechange = function () {
        if (xhr_check(this) && status_icon.innerHTML() == "cloud_off") {
          status_text.innerHTML("CONNECTED");
          status_icon.innerHTML("cloud");
        } else if ( !xhr_check(this) && status_icon.innerHTML() != "cloud_off") {
          status_text.innerHTML("NOT CONNECTED");
          status_icon.innerHTML("cloud_off");
        }
    }
  }

  setInterval(function () {
    loop();
  }, 100);

  function loop() {
    if (loop_index == 1) {
      loop_index = 0;
      // do sth else...
      // update graph
    }

    heartbeat();

  }

  function getData() {

    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        const json = this.responseText;
        const jsonObj = JSON.parse(json);
        document.getElementById("enc_a").innerHTML = jsonObj.ena;
        document.getElementById("enc_c").innerHTML = jsonObj.enc;
        document.getElementById("odometer").innerHTML = jsonObj.odometer;
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
      }
    };
    yhttp.open("GET", "update_speed", true);
    yhttp.send();

  }

</script>

</html>
)=====";