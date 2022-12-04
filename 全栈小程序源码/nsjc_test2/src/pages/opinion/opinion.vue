<template>
  <div>
    <div class="contain">
      <div class="row">
        <label class="name">意见反馈</label>
      </div>
      <div class="row text">
        <div>
          <textarea
            v-model="opinion"
            maxlength="200"
            placeholder="点击这里输入您的建议~"
            class="input"
          ></textarea>
          <label class="word-count">{{ word_count }}/200</label>
        </div>
      </div>
      <div class="row">
        <div>
          <label>相关截图 (选填) </label>
          <label class="img-count">{{ img_count }}/2</label>
        </div>
        <label v-for="(item, index) in src" :key="index">
          <img class="img" :src="item" />
        </label>
        <label v-if="img_count < 2" @click="uploadPicture">
          <img class="add-img" src="/static/images/kongtupian.png" />
        </label>
      </div>
      <div class="row">
        <div class="name">微信号（选填）</div>
        <input
          v-model="wechat"
          maxlength="20"
          placeholder="如果想要详细交流，点击这里留下微信号哦~"
          class="wechat-input"
        />
      </div>
    </div>
    <button @click="submmit">提交</button>
  </div>
</template>

<script>
import { post, showModel } from "@/util";

export default {
  data() {
    return {
      opinion: "",
      word_count: 0,
      src: [],
      img_count: 0,
      wechat: "",
    };
  },
  watch: {
    opinion() {
      var that = this;
      that.word_count = that.opinion.length;
    },
    src() {
      var that = this;
      that.img_count = that.src.length;
    },
  },
  methods: {
    uploadPicture() {
      var that = this;
      wx.chooseImage({
        count: 2,
        sizeType: ["original", "compressed"],
        sourceType: ["album", "camera"],
        success(res) {
          const tempFilePaths = res.tempFilePaths;
          that.src.push(tempFilePaths);
          console.log("图片路径", that.src);
        },
      });
    },
    async submmit() {
      var that = this;
      if (that.word_count > 0) {
        const data = {
          opinion: that.opinion,
          src: that.src.join(","), //mysql没有数组的形式所以转换成字符串
          wechat: that.wechat,
          openid: wx.getStorageSync("userinfo").openId,
        };
        try {
          const res = await post("/weapp/createopinion", data);
          console.log("从后端返回执行正确的信息是：", res);
          showModel("提交成功", "已经将您的反馈提交给了开发者~");
        } catch (e) {
          console.log("从后端返回执行错误的信息是:", e);
          showModel("提交失败", "服务器出了一点问题，请稍后再试~");
        }
        setTimeout(() => {
          (that.src = []),
            (that.opinion = ""),
            (that.word_count = 0),
            (that.img_count = 0),
            (that.wechat = "");
        }, 1000);
      } else {
        showModel("提交失败", "反馈信息不能为空");
      }
    },
  },
};
</script>

<style lang="scss">
.contain {
  background: #ffffff;
  font-size: 15px;
  .text {
    height: 110px;
  }
  .row {
    border-bottom: 1px #e8e8e8 solid;
    padding: 5px 15px;
    .name {
      width: 80%;
      height: 40px;
      line-height: 40px;
    }
    .input {
      width: 100%;
      height: 85px;
      font-size: 14px;
      padding-top: 5px;
    }
    .word-count {
      float: right;
      color: #808080;
    }
    .img-count {
      float: right;
      line-height: 40px;
      color: #808080;
    }
    .add-img {
      width: 80px;
      height: 80px;
    }
    .img {
      width: 66px;
      height: 66px;
      margin-bottom: 7px;
      margin-right: 10px;
    }
    .wechat-input {
      font-size: 14px;
    }
  }
}
button {
  margin: 20px auto;
  width: 90%;
  border-radius: 5px;
  background: #7fd2f3;
  font-size: 16px;
  color: #ffffff;
  font-weight: bold;
}
</style>