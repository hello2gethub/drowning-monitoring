<template>
  <div>
    <div class="header">
      <div v-if="isLogin">
        <div class="header-title">请登录</div>
        <div class="header-info">Please Login Your Account</div>
      </div>
      <div v-else>
        <div class="header-title">请注册</div>
        <div class="header-info">Please Register Your Account</div>
      </div>
    </div>
    <div class="body">
      <div class="login-form">
        <van-field
          placeholder="请输入用户名"
          :value="inputUserName"
          @change="onUserNameChange"
        />
        <van-field
          type="password"
          placeholder="请输入密码"
          :value="inputPassWord"
          @change="onPassWordChange"
        />
        <div v-if="!isLogin">
          <van-field
            placeholder="请输入手机号"
            :value="inputContect"
            @change="onContectChange"
          />
        </div>
      </div>
      <van-button slot="button" round block color="#3d7ef9" @click="onClick">
        {{ isLogin ? "登录" : "注册" }}</van-button
      >
      <div class="other-option">
        <div @click="onOptionClick">
          <span>{{ isLogin ? "注册账户" : "登录账户" }}</span>
        </div>
        <span style="margin: 0 30px">|</span>
        <div @click="onForgetClick">
          <span>忘记密码</span>
        </div>
      </div>
      <div class="copyright-wrapper">
        <span class="copyright">梦想起航项目组</span>
      </div>
      <van-dialog
        use-slot
        title="找回密码校验"
        :show="showFindPW"
        show-cancel-button
        transition="fade"
        @confirm="onFindPWConfirm"
        @cancel="onFindPWCancel"
      >
        <van-field
          label="手机号"
          title-width="60px"
          placeholder="请输入账户绑定的手机号"
          :value="inputContect"
          @change="onContectChange"
        />
      </van-dialog>
      <van-dialog
        use-slot
        title="重置密码"
        :show="showResetPW"
        show-cancel-button
        transition="fade"
        @confirm="onResetPWConfirm"
        @cancel="onResetPWCancel"
      >
        <van-field
          label="用户名"
          title-width="60px"
          :value="inputUserName"
          readonly
        />
        <van-field
          label="新密码"
          type="password"
          title-width="60px"
          placeholder="请输入新密码"
          :value="inputPassWord"
          @change="onPassWordChange"
        />
      </van-dialog>
      <van-toast id="van-toast" />
    </div>
  </div>
</template>

<script>
import Toast from "vant-weapp/dist/toast/toast";

export default {
  data() {
    return {
      isLogin: true,
      inputUserName: "",
      inputPassWord: "",
      inputContect: "",
      showFindPW: false,
      showResetPW: false,
    };
  },
  methods: {
    onUserNameChange(event) {
      var that = this;
      that.$set(that, "inputUserName", event.mp.detail); //当输入值发生改变，赋值给inputUserName
      console.log("当前输入用户名 = ", event.mp.detail);
    },
    onPassWordChange(event) {
      var that = this;
      that.$set(that, "inputPassWord", event.mp.detail); //当输入值发生改变，赋值给inputPassWord
      console.log("当前输入的密码 = ", event.mp.detail);
    },
    onContectChange(event) {
      var that = this;
      that.$set(that, "inputContect", event.mp.detail); //当输入值发生改变，赋值给inputContect
      console.log("当前输入的手机号 = ", event.mp.detail);
    },
    onClick(event) {
      var that = this;
      Toast.loading({
        duration: 0, //持续展示Toast
        forbidClick: true, //禁止背景能够被点击
        message: that.isLogin ? "登陆中..." : "注册中...",
      });
      //模拟请求服务器延时1000ms
      setTimeout(() => {
        //箭头函数，不懂百度JS的ES6语法
        if (!that.isLogin) {
          //注册
          var reg = /^(1[3|4|5|6|7|8|9])\d{9}$/; //正则校验手机号格式
          if (!reg.test(that.inputContect)) {
            Toast.fail("手机号格式错误");
            return;
          }
          var info = {
            username: that.inputUserName,
            password: that.inputPassWord,
            contect: that.inputContect,
          };
          wx.setStorageSync("User", info);
          Toast.success("注册成功！");
        } else {
          var User = wx.getStorageSync("User");
          console.log(User);
          if (
            that.inputUserName === User.username &&
            that.inputPassWord === User.password
          ) {
            Toast.success("登录成功");
            //500ms以后跳转到首页
            setTimeout(() => {
              wx.switchTab({
                url: "/pages/index/main",
              });
            }, 500);
          } else {
            Toast.fail("用户名或密码错误");
          }
        }
      }, 500);
    },
    onOptionClick(event) {
      var that = this;
      that.onResetPWCancel();
      that.isLogin = !that.isLogin;
      console.log(
        `注册账户/登录被点击了！当前处于${that.isLogin ? "登录" : "注册"}状态`
      );
    },
    onForgetClick(event) {
      var that = this;
      that.showFindPW = true;
    },
    onFindPWConfirm(event) {
      var that = this;
      wx.getStorage({
        key: "User",
        success(res) {
          console.log(res.data);
          if (that.inputContect === res.data.contect) {
            that.inputUserName = res.data.username;
            that.showResetPW = true;
            console.log("找到用户：", res.data.username);
          } else {
            Toast.fail("无该用户信息");
          }
        },
        fail(res) {
          console.log(res);
          Toast.fail("数据库暂无注册用户");
        },
      });
      that.showFindPW = false;
    },
    onFindPWCancel(event) {
      var that = this;
      that.showFindPW = false;
      that.inputContect = "";
    },
    onResetPWConfirm(event) {
      var that = this;
      wx.setStorage({
        //存储数据API
        key: "User",
        data: {
          username: that.inputUserName,
          password: that.inputPassWord,
          contect: that.inputContect,
        },
        success(res) {
          console.log(res);
          console.log("密码重置成功！");
          Toast.success("密码重置成功！");
          that.onResetPWCancel();
        },
        fail(res) {
          console.log(res);
          console.log("密码重置失败！");
          Toast.success("密码重置失败！");
        },
      });
    },
    onResetPWCancel(event) {
      var that = this;
      that.inputUserName = "";
      that.inputPassWord = "";
      that.inputContect = "";
      that.showResetPW = false;
    },
  },
  onShow() {
    var that = this;
    var User = wx.getStorageSync("User");
    if (User == null) {
      that.$set(that, "isLogin", "false");
    }else {
      wx.switchTab({
        url: "/pages/index/main",
      });
    }
  },
}; 
</script>

 
<style lang="scss" scoped>
.header {
  height: 100px;
  padding: 25px 0; //纵向内边距和横向内边距
  background-color: #3d7ef9;
  color: #fff;
  .header-title {
    font-size: 28px;
    font-weight: 500;
    margin-left: 20px;
  }
  .header-info {
    font-size: 14px;
    color: #fff;
    margin-left: 20px;
  }
}

.body {
  padding: 20px;
  margin-top: -20px; //把整块往头移动
  border-radius: 15px 15px 0 0;
  background-color: #fff;
  .login-form {
    margin-bottom: 30px; //与按钮的间隔
  }
  .other-option {
    display: flex;
    justify-content: center;
    margin-top: 20px;
    color: #9f9f9f;
  }
  .copyright-wrapper {
    display: flex;
    justify-content: center;
    .copyright {
      //版权样式
      color: #ddd;
      position: fixed;
      bottom: 50px;
    }
  }
}
</style>

