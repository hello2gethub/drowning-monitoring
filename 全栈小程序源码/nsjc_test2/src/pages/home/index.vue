<template>
  <div>
    <swiper
      autoplay
      interval="2000"
      circular
      indicator-dots
      indicator-active-color="#0094ff"
    >
      <swiper-item>
        <img class="logo" src="/static/images/swimming.png" />
      </swiper-item>
      <swiper-item>
        <img class="logo" src="/static/images/shouhuan.png" />
      </swiper-item>
      <swiper-item>
        <img class="logo" src="/static/images/child.png" />
      </swiper-item>
      <swiper-item>
        <img class="logo" src="/static/images/hotel.png" />
      </swiper-item>
      <swiper-item>
        <img class="logo" src="/static/images/logo.png" />
      </swiper-item>
    </swiper>
    <div v-if="show_record">
      <div class="prompt">还没有任何记录~</div>
    </div>
    <div v-else>
      <div class="table th">
        <div class="date">时间</div>
        <div class="temp">温度</div>
        <div class="busi">心率</div>
        <div class="busi">压力</div>
        <div class="busi">溺水</div>
        <div class="net">备注</div>
      </div>
      <RecordList
        v-for="(record, index) in records"
        :key="index"
        :record="record"
      ></RecordList>
      <p class="text-footer" v-if="!this.more">没有更多的数据</p>
      <p class="text-footer" v-else>加载中...</p>
    </div>
  </div>
</template>

<script>
import RecordList from "@/components/RecordList";
import { get } from "@/util";

export default {
  components: {
    RecordList,
  },
  data() {
    return {
      bianhao: "STM32-001",
      show_record: false,
      records: [],
      page: 0,
      more: true,
    };
  },
  methods: {
    async getRecords(init) {
      var that = this;
      wx.showToast({
        title: "加载中",
        icon: "loading",
      });
      if (init) {
        that.page = 0;
        that.more = true;
      }
      if (that.page === 0) {
        that.records = [];
      }
      try {
        const data = {
          openid: that.bianhao,
          page: that.page,
        };
        const res = await get("/weapp/getrecords", data);
        console.log("从后端读取记录成功：", res);
        that.records = that.records.concat(res.records);
        if (res.records.length < 15 && this.page > 0) {
          that.more = false;
        }
        if (that.records.length === 0) {
          that.show_record = true;
        } else {
          that.show_record = false;
        }
        wx.hideToast();
      } catch (e) {
        console.log("从后端读取记录执行错误信息：", e);
        wx.hideToast();
      }
    },
  },
  onShow() {
    var that = this;
    that.getRecords(true);
  },
  onReachBottom() {
    var that = this;
    if (!that.more) {
      return;
    }
    that.page += 1;
    console.log("当前在第几页", that.page);
    that.getRecords();
  },
  onPullDownRefresh() {
    var that = this;
    that.getRecords(true);
    console.log("正在下拉刷新，刷新记录数据");
    wx.stopPullDownRefresh();
  },
};
</script>

<style lang="scss" scoped>
.add {
  margin-top: 20px;
  margin-bottom: 10px;
  text-align: center;
  p {
    font-size: 15px;
  }
}
.logo {
  height: 178px;
  width: 100%;
  position: absolute;
}
.th {
  width: 100%;
  height: 30px;
  line-height: 30px;
  background: #49d2ea;
  color: #ffffff;
  font-size: 16px;
  font-weight: bold;
  display: flex;
}
.prompt {
  margin-top: 50px;
  margin-bottom: 30px;
  font-size: 14px;
  color: #ffffff;
  text-align: center;
}
.date {
  width: 13%;
  padding-left: 40px;
}
.temp {
  width: 13%;
  margin-left: 57px;
}
.busi {
  width: 13%;
  margin-left: 15px;
}
.net {
  width: 20%;
  margin-left: 20px;
}
.text-footer {
  text-align: center;
  font-size: 15px;
  margin-bottom: 5px;
  padding-top: 5px;
  color: #ffffff;
}
</style>