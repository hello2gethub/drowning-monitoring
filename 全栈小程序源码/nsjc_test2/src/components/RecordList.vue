<template>
  <div class="book-card">
    <div class="table" width="98%">
      <div class="tr">
        <div class="date">
          {{ create_time }}
        </div>
        <div class="busi">
          {{ record.Temp }}℃
        </div>
        <div class="busi2">
          {{ record.Heart }}
        </div>
        <div class="busi3">
          {{ record.Press }}g
        </div>
         <div class="busi2">
          {{ record.Beep }}
        </div>
        <div class="net" @click="changeShow">
          <label v-if="note">{{ note }}</label>
          <label v-else class="no-note">点击添加</label>
        </div>
        <label v-if="show_input">
          <div class="image" @click="cancel">
            <image class="img" src="/static/images/cancel-test.png"></image>
          </div>
        </label>
        <label v-else>
          <div class="image" @click="changeShow">
            <image class="img" :src="note ? src : ''"></image>
          </div>
        </label>
      </div>
    </div>
    <div class="hide" v-if="show_input">
      <button class="btn" @click="updateNote">
        <label v-if="record.note"> 修改 </label>
        <label v-else> 添加 </label>
      </button>
      <input
        v-model="note"
        class="input"
        maxlength="10"
        placeholder="最多输入10个字"
      />
    </div>
  </div>
</template>

<script>
import { formatTime } from "../utils/index.js";
import { post, showModel } from "@/util";

export default {
  props: ["record"],
  data() {
    return {
      create_time: formatTime(new Date(this.record.create_time)),
      show_input: false,
      note: this.record.note,
      src: "/static/images/approval.png",
    };
  },
  methods: {
    changeShow() {
      var that = this;
      that.show_input = !that.show_input;
    },
    async updateNote() {
      var that = this;
      try {
        const data = {
          id: that.record.id,
          note: that.note,
        };
        const res = await post("/weapp/updatenote", data);
        console.log("修改记录从后端传回来的执行正确信息：", res);
        that.show_input = false
        that.record.note = that.note
      } catch (e) {
        showModel("失败", "请重新提交");
        console.log("修改记录从后端返回的执行错误信息：", e);
      }
    },
    cancel(){
      var that = this;
      that.show_input =!that.show_input;
      that.note = that.record.note;
    }
  },
};
</script>

<style lang="scss">
.book-card {
  background: #ffffff;
  margin-top: 0px;
  .table {
    border: 0px solid darkgray;
    font-size: 15px;
    height: 42px;
    line-height: 42px;
    .tr {
      display: flex;
      width: 100%;
    }
    .date {
      width: 45%;
    }
    .busi {
      width: 10%;
      margin-right: 10px;
      font-weight: bold;
    }
    .busi2 {
      width: 10%;
      margin-left: 15px;
      font-weight: bold;
    }
    .busi3 {
      width: 10%;
      margin-left: 13px;
      font-weight: bold;
    }
    .mark {
      width: 15%;
      margin-left: 20px;
      font-weight: bold;
    }
    .net {
      width: 16%;
      text-align: center;
      width: 60px;
      overflow: hidden;
      text-overflow: ellipsis;
      white-space: nowrap;
      font-size: 14px;
      margin-left: 5px;
      line-height: 42px;
      .no-note {
        text-decoration: underline;
        color: #c0c0c0;
        font-size: 13px;
      }
    }
    .image {
      padding-top: 1px;
      float: right;
      margin-left: 5px;
    }
  }
  .hide {
    background: #f0f0f0;
    font-size: 15px;
    padding: 10px 10px 3px 30px;
    .input {
      width: 60%;
      height: 30px;
      background: #ffffff;
      border: 1px solid black;
      border-radius: 5px;
      text-align: center;
    }
    .btn {
      color: white;
      background: #3d7ef9;
      padding-left: 15px;
      margin-right: 20px;
      border-radius: 5px;
      font-size: 13px;
      line-height: 30px;
      height: 30px;
      width: 18%;
      float: right;
    }
  }
  .img {
    width: 20px;
    height: 20px;
    margin-top: 10px;
    margin-right: 5px;
  }
}
::-webkit-scrollbar {
  width: 0;
  height: 0;
  color: transparent;
}
</style>