const char static_site[] = R"=====(
<!DOCTYPE html>
<html lang="en-US">

<head>
  <title>ESP8266 Web Console</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
  <meta name="renderer" content="webkit" />
  <meta name="force-rendering" content="webkit" />
  <meta http-equiv="X-UA-Compatible" contxent="IE=edge,chrome=1" />
  <link rel="stylesheet" href="https://unpkg.com/mdui@1.0.2/dist/css/mdui.min.css" />

  <script src="https://cdn.staticfile.org/echarts/4.3.0/echarts.min.js"></script>
  <script src="https://unpkg.com/mdui@1.0.2/dist/js/mdui.min.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/echarts@5.4.1/dist/echarts.min.js"></script>

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
                <span id="current_spd" style="font-weight:500;font-size: x-large;">N/A</span>
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
  var hb_lever = 0;

  function GETRequest(url) {
    return new Promise(function (resolve, reject) {
      var xhr = new XMLHttpRequest();
      xhr.open('GET', url);
      xhr.setRequestHeader('Cache-Control', 'no-cache');
      xhr.onload = function () {
        if (xhr.status === 200) {
          resolve(xhr.response);
        } else {
          reject(Error(xhr.statusText));
        }
      };
      xhr.onerror = function () {
        reject(Error('Network Error'));
      };
      xhr.send();
    });
  }

  function heartbeat() {

    if (hb_lever == 1) return null;

    GETRequest('heartbeat')
      .then(function (res) {
        console.log(res)
        if (res == "beat_ack") {
          status_icon.innerHTML = "cloud";
          status_text.innerHTML = "CONNECTED";
        } else {
          status_icon.innerHTML = "cloud_off";
          status_text.innerHTML = "NOT CONNECTED";
        }
      })
      .catch(function (err) {
        console.log(err);
      })
  }

  setInterval(function () {
    loop();
    loop_index++;
    // caution: loop_index may exceed maxium limit
  }, 200);

  function loop() {
    if (loop_index == 1) {
      // do sth else...
      // update graph
    } else if (loop_index == 15) {
      hb_lever = 1;
    }

    heartbeat();

  }
</script>

</html>
)=====";