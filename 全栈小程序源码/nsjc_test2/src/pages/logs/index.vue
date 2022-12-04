<template>
  <div>
    <div class="header">
      <div class="header-wrapper">
        <img
          class="logo"
          src="/static/images/logo.png"
        />
        <div class="user-info">
          <div class="avatar-wrapper">
            <open-data
              type="userAvatarUrl"
              default-avatar="/static/images/logo.png"
            ></open-data>
          </div>
          <div class="username-wrapper">
            <open-data default-text="读取中" type="userNickName"></open-data>
          </div>
        </div>
      </div>
    </div>
    <div class="contain">
      <div class="row" @click="showGuide">
        <label class="left">
          <img class="img" src="/static/images/caozuozhiyin.png" />
        </label>
        <label class="name">&nbsp;&nbsp;操作指引</label>
        <label class="right"> > </label>
      </div>

      <div class="row">
        <label class="left">
          <img class="img" src="/static/images/wodejiance.png" />
        </label>
        <label class="name">&nbsp;&nbsp;智能溺水监测手环系统V1.0</label>
        <label class="right"> > </label>
      </div>
      <div class="row" @click="reset">
        <label class="left">
          <img class="img" src="/static/images/delete.png" />
        </label>
        <label class="name">&nbsp;&nbsp;数据清零</label>
        <label class="right"> > </label>
      </div>

      <div class="row" @click="showOpinion">
        <label class="left">
          <img class="img" src="/static/images/approval.png" />
        </label>
        <label class="name">&nbsp;&nbsp;意见反馈</label>
        <label class="right"> > </label>
      </div>
    </div>
  </div>
</template>

<script>
import Toast from "vant-weapp/dist/toast/toast";
import { showSuccess, showModel, post, get } from "@/util";

export default {
  data() {
    return {
      openid: "STM32-001",
    };
  },
  methods: {
    showOpinion() {
      wx.navigateTo({
        url: "/pages/opinion/main",
      });
    },
    showGuide() {
      wx.navigateTo({
        url: "/pages/guide/main",
      });
    },
    async resetData() {
      var that = this;
      try {
        const res = await post("/weapp/resetData", { openid: that.openid });
        console.log("清空记录操作后端传回执行成功的信息：",res);
      } catch (e) {
        showModel("清空记录失败","请重试喔~");
        console.log("清空记录操作从后端返回的执行失败信息：",e);
      }
    },
    reset() {
      var that = this;
      wx.showModal({
        content: "确定要将数据清零吗？不可恢复哦~",
        success: function (res) {
          if (res.confirm) {
            that.resetData();
          }
        },
      });
    },
  },
};
</script>

<style lang="scss" scoped>
.header-wrapper {
  background-blend-mode: multiply;
  height: 40vh;
  color: #fff;
  align-items: center;
}
.logo{
  height: 250px;
  width:100%;
  z-index: -999; 
  position: absolute;
}
.user-info {
  margin-left: 140px;
  margin-top: 70px;
  padding-bottom: 5px;
  float: left;
  .avatar-wrapper {
    margin-right: 300px;
    height: 100px;
    width: 100px;
    border-radius: 50%;
    overflow: hidden;
  }
  .username-wrapper {
    padding-top: 5px;
    margin-left: 25px;
    color: #ffff00;
    font-size: 23px;
    float: left;
  }
}
.contain {
  background: #ffffff;
  font-size: 16px;
  .row {
    padding: 0px 18px;
    border-bottom: 1px #e8e8e8 solid;
    height: 55px;
    line-height: 55px;
    .img {
      float: left;
      width: 25px;
      height: 25px;
      padding-top: 16px;
    }
    .name {
      float: left;
    }
  }
  .right {
    float: right;
    color: #c8c8c8;
    line-height: 55px;
  }
  .left {
    width: 80%;
  }
}
</style>