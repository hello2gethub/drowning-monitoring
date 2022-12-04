<template>
  <div>
    <div class="modal-mask"></div>
    <div class="modal-dialog">
      <div class="modal-content">
        <img class="img" src="/static/images/youyong2.png" />
        <div class="content-text">
          <p class="key-bold">智能溺水监测手环系统是游泳安全神器的小程序端。</p>
          <p class="key-bold">
            提供完备的体征监测、溺水提醒功能、定位追踪的功能。
          </p>
          <p class="little-tip">举个例子:</p>
          <p class="little-content">
            点击体征监测进入体征监测页面即可实时监测游泳者的体征信息。
          </p>
          <p class="little-content">
            点击定位追踪按钮，即可以进入腾讯地图，为您规划一条到游泳者的最短路线。
          </p>
        </div>
      </div>
      <div class="modal-footer">
        <button
          open-type="getUserInfo"
          lang="zh_CN"
          class="btn"
          @getuserinfo="login"
        >
          授权登录
        </button>
      </div>
    </div>
  </div>
</template>

<script>
const host = "https://nsjc.xyz";

const longinUrl = "https://nsjc.xyz/weapp/login";

import qcloud from "wafer2-client-sdk";
import {showSuccess} from "@/util"

export default {
  methods: {
    login() {
      var that = this;
      console.log("测试", host);
      qcloud.setLoginUrl(longinUrl);
      qcloud.login({
        success: (userInfo) => {
          console.log("登录成功", userInfo);
          that.loginSuccess(userInfo);
          that.$emit("changeShow", false, userInfo);
          wx.showTabBar();
          showSuccess("登录成功")
        },
        fail: (err) => {
          console.log("登录失败", err);
        },
      });
    },
    loginSuccess(userInfo) {
      var that = this;
      wx.setStorageSync("userinfo", userInfo);
    },
  },
};
</script>

<style lang="scss">
.modal-mask {
  width: 100%;
  height: 100%;
  position: fixed;
  top: 0;
  left: 0;
  background: #000;
  opacity: 0.5;
  overflow: hidden;
  z-index: 9000;
  color: #fff;
}
.modal-dialog {
  box-sizing: border-box;
  width: 560rpx;
  overflow: hidden;
  position: fixed;
  top: 30%;
  left: 0;
  z-index: 9999;
  background: #fff;
  margin: -150rpx 95rpx;
  border-radius: 16rpx;
}
.modal-content {
  box-sizing: border-box;
  display: flex;
  padding: 0rpx 53rpx 50rpx 53rpx;
  font-size: 32rpx;
  align-items: center;
  justify-content: center;
  flex-direction: column;
}
.content-tip {
  text-align: center;
  font-size: 36rpx;
  color: #333333;
}
.content-text {
  height: 130px;
  padding: 10px 0px 50px 0px;
  font-size: 14px;
}
.modal-footer {
  box-sizing: border-box;
  display: flex;
  flex-direction: row;
  border-top: 1px solid #e5e5e5;
  font-size: 16px;
  font-weight: bold;
  height: 45px;
  line-height: 45px;
  text-align: center;
  background: #7fd2f3;
}
button {
  width: 100%;
  background: #7fd2f3;
  color: #ffffff;
  font-weight: bold;
}
.img {
  width: 280px;
  height: 90px;
}
.little-tip {
  padding-top: 15px;
  padding-bottom: 3px;
  font-size: 14px;
  font-weight: bold;
  color: #7fd2f3;
}
.little-content {
  padding-top: 5px;
  font-size: 13px;
  color: #606060;
}
.key-bold {
  padding-top: 5px;
  font-size: 14px;
  font-weight: bold;
}
</style>