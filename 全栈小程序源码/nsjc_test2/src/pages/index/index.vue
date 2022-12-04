<template>
  <div class="wrapper">
    <div v-if="showLogin">
      <LoginWindow @changeShow="getModel(arguments)"></LoginWindow>
    </div>
    <div class="header-wrapper">
      <div class="header-title">
        <span>空气质量-{{ airText }}</span>
        <span>{{ city }}-{{ area }}</span>
      </div>
      <div class="header-text">
        <span>PM:{{ airValue }}</span>
        <span>{{ weather }}</span>
      </div>
      <div class="weather-advice">{{ weatherAdvice }}</div>
    </div>

    <div class="body-wrapper">
      <div class="body">
        <div class="data-wrapper">
          <div class="data">
            <img class="data-logo" src="/static/images/press.png" />
            <div class="data-text">
              <div class="data-title">水压</div>
              <div class="data-value">{{ Press }}g</div>
            </div>
          </div>

          <div class="data">
            <img class="data-logo" src="/static/images/plusenor.png" />
            <div class="data-text">
              <div class="data-title">心率</div>
              <div class="data-value">{{ Heart }}/min</div>
            </div>
          </div>
        </div>

        <div class="data-wrapper">
          <div class="data">
            <img class="data-logo" src="/static/images/temp.png" />
            <div class="data-text">
              <div class="data-title">温度</div>
              <div class="data-value">{{ Temp }}℃</div>
            </div>
          </div>
          <div class="data">
            <img class="data-logo" src="/static/tabs/map-active.png" />
            <div class="data-text">
              <div class="data-title">定位追踪</div>
              <div class="data-value">
                <switch @change="onMapChange" :checked="Map" color="#3d7ef9" />
              </div>
            </div>
          </div>
        </div>

        <div class="data-wrapper">
          <div class="data">
            <img class="data-logo" src="/static/images/beep.png" />
            <div class="data-text">
              <div class="data-title">报警器</div>
              <div class="data-value">
                <switch
                  @change="onBeepChange"
                  :checked="Beep"
                  color="#3d7ef9"
                />
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { connect } from "mqtt/dist/mqtt.js";
import { showSuccess, showModel, post, get } from "@/util";
import LoginWindow from "@/components/LoginWindow";

const mqttUrl = "wxs://www.nsjc.xyz:8084/mqtt"; //服务器域名

export default {
  data() {
    return {
      userinfo: {},
      showLogin: false,
      client: {},
      bianhao: "STM32-001",
      add: false,
      //ans: 0,
      Temp: 0,
      Press: 0,
      Heart: 0,
      Beep: false,
      Map: false,
      area: "请求中", //城区
      city: "请求中", //城市
      airText: "请求中", //空气优良
      airValue: 0, //空气指数
      weather: "请求中", //天气
      weatherAdvice: "请求中", //天气建议
      plugin: requirePlugin("routePlan"),
      key: "K2TBZ-EB3K3-VOE3E-3BZP2-6D6SO-RMFFP", //使用在腾讯位置服务申请的key
      referer: "智能溺水监测手环系统", //调用插件的app的名称
      endPoint: JSON.stringify({
        //初始化终点
        name: "设备所在地",
        latitude: 28.226787,
        longitude: 112.999274,
      }),
    };
  },

  components: {
    LoginWindow,
  },

  methods: {
    getModel(val) {
      var that = this;
      that.showLogin = val[0];
      that.userinfo = val[1];
    },
    async addMysql() {
      var that = this;
      try {
        const data = {
          openid: that.bianhao,
          Temp: that.Temp,
          Press: that.Press,
          Heart: that.Heart,
          Beep: that.Beep,
          name:that.name,
          latitude:that.latitude,
          longitude:that.longitude
        };
        const res = await post("/weapp/createrecord", data);
      } catch (e) {
        showModel("请求插入数据失败", "请重试~");
        console.log("从后端返回的执行错误信息是:", e);
      }
    },
    async getCurrent() {
      try {
        var that = this;
        const res = await get("/weapp/getdata", { openid: that.bianhao });
        console.log("从后端传回的信息：", res);
        that.Temp = res.Temp;
        that.Press = res.Press;
        that.Heart = res.Heart;
        that.Beep = res.Beep;
      } catch (e) {
        showModel("请求读取后端数据失败", "请下拉页面重试~");
        console.log("从后端返回的执行错误信息是:", e);
      }
    },
    onMapChange(event) {
      var that = this;
      console.log(event.mp.detail);
      let sw = event.mp.detail.value;
      that.Map = sw;
      if (sw) {
        wx.navigateTo({
          url:
            "plugin://routePlan/index?key=" +
            that.key +
            "&referer=" +
            that.referer +
            "&endPoint=" +
            that.endPoint,
        });
        setTimeout(() => {
          that.Map = false;
        }, 500);
      }
    },
    onBeepChange(event) {
      var that = this;
      console.log(event.mp.detail);
      let sw = event.mp.detail.value;
      that.Beep = sw;
      if (sw) {
        //如果报警器为报警状态
        that.client.publish(
          "Topic/wx",
          '{"target":"BEEP","value":1}',
          function (err) {
            //发布消息至服务器
            if (!err) {
              console.log("成功开启声光报警！");
            }
          }
        );
      } else {
        that.client.publish(
          "Topic/wx",
          '{"target":"BEEP","value":0}',
          function (err) {
            //发布消息至服务器
            if (!err) {
              console.log("成功关闭声光报警！");
            }
          }
        );
      }
    },
    onClient() {
      var that = this;
      //if (that.ans == 0) {
        that.client = connect(mqttUrl);
        that.client.on("connect", function () {
          console.log("成功连接emqx服务器!");
          that.client.subscribe("Topic/stm32", function (err) {
            //订阅主题topic/#
            if (!err) {
              console.log("成功订阅设备上行数据topick");
             // that.ans = 1;
            }
          });
        });
     // }

      that.client.on("message", function (topic, message) {
        console.log(topic); //打印出topic
        let dataFromDev = {};
        dataFromDev = JSON.parse(message); //message是16进制的buffer字节流,要转换
        console.log(dataFromDev);
        that.add = true;
        that.Temp = dataFromDev.Temp; //取订阅到的温度数据
        that.Heart = dataFromDev.Heart;
        that.Press = dataFromDev.Press;
        that.Beep = dataFromDev.Beep;
        if (dataFromDev.latitude != null | dataFromDev.longitude != null)
          that.endPoint = JSON.stringify({
            name: dataFromDev.name,
            latitude: dataFromDev.latitude,
            longitude: dataFromDev.longitude
          });
        if (that.add) {
          that.addMysql();
          that.add = false;
        }
      });

      wx.getLocation({
        type: "wgs84",
        success(res) {
          const lat = res.latitude;
          const longt = res.longitude;
          //const speed = res.speed;
          //const accuracy = res.accuracy;
          const key = "58e08adce6914c65a13b17a132379239";
          wx.request({
            url: `https://free-api.heweather.net/s6/weather/now?location=${longt},${lat}&key=${key}`, //获取天气数据的API接口地址
            success(res) {
              //console.log(res.data);
              const { basic, now } = res.data.HeWeather6[0];
              //  console.log(basic)
              //  console.log(now)
              that.area = basic.location;
              that.city = basic.parent_city;
              that.weather = now.cond_txt;
              that.tmp = now.tmp;

              wx.request({
                url: `https://free-api.heweather.net/s6/air/now?location=${that.city}&key=${key}`, //获取空气数据的API接口地址
                success(res) {
                  //console.log(res.data);
                  const { air_now_city } = res.data.HeWeather6[0];
                  const { aqi, qlty } = air_now_city;
                  that.airText = qlty;
                  that.airValue = aqi;
                },
              });
            },
          });
          wx.request({
            url: `https://free-api.heweather.net/s6/weather/lifestyle?location=${longt},${lat}&key=${key}`, //获取空气数据的API接口地址
            success(res) {
              //console.log(res.data);
              const { lifestyle } = res.data.HeWeather6[0];
              console.log(lifestyle);
              that.weatherAdvice = lifestyle[1].txt;
            },
          });
        },
      });
    },
  },
  mounted() {
    var that = this;
    const userinfo = wx.getStorageSync("userinfo");
    if (userinfo.openId) {
      that.userinfo = userinfo;
      console.log("用户信息：", that.userinfo);
    } else {
      wx.hideTabBar();
      that.showLogin = true;
    }
  },
  onShow() {
    var that = this;
    that.onClient();
    that.getCurrent();
  },
  onPullDownRefresh() {
    var that = this;
    that.ans = 0;
    that.onClient();
    that.getCurrent();
    wx.stopPullDownRefresh();
    console.log("下拉刷新,重新连接服务器并显示数据库内最新的数据");
  },
  onShareAppMessage() {
    return {
      title: "溺水监测",
      path: "/pages/index/main",
    };
  },
};
</script>

<style lang="scss" scoped>
.wrapper {
  padding: 15px;
  .header-wrapper {
    border-start-start-radius: 20px; //圆角
    color: #fff; //文字颜色
    box-shadow: #d6d6d6 0px 0px 5px; //阴影
    padding: 15px 30px; //左右30px  上下15px
    .header-title {
      display: flex;
      justify-content: space-between; //这两行可实现顶部文字空开
    }
    .header-text {
      margin-top: 10px; //行与行之间的间隔
      font-size: 25px; //字体大小
      font-weight: 400; //字体粗细
      display: flex; //flex布局
      justify-content: space-between; //隔开
    }
    .weather-advice {
      margin-top: 10px; //行与行之间的间隔
      font-size: 12px;
    }
  }

  .data-wrapper {
    margin-top: 20px; //上边距
    display: flex; //布局
    justify-content: space-between; //隔开
    .data {
      background-color: #fff;
      width: 150px;
      height: 80px;
      border-radius: 20px; //圆角
      display: flex;
      justify-content: space-around;
      padding: 0 8px;
      box-shadow: #d6d6d6 0px 0px 5px; //阴影
      .data-logo {
        height: 40px;
        width: 40px;
        margin-top: 15px;
      }
      .data-text {
        margin-top: 15px;
        color: #7f7f7f;
        .data-title {
          //font-size: 15px;
          text-align: right;
        }
        .data-value {
          font-size: 20px;
        }
      }
    }
  }
}
</style>
