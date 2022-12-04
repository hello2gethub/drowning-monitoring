<template>
  <div class="instruction">
    <div class="title">
      <img class="img" :src="src">
      <p class="content content-title">智能溺水监测手环系统是游泳必备神器的小程序端，对人体体征、运动体态、位置信息进行实时监测，以助安全健康的游泳运动。</p>
      <p class="content content-title">在低龄儿童中由于溺水难发现尤其需要</p>
    </div>
    <p class="little-tip">1、给大家分享一下湿性溺水者溺水时的表现：</p>
    <p class="content tip"><label class="point">◉</label> 人是直立在水中的，而不是平躺在水中。</p>
    <p class="content tip"><label class="point">◉</label> 手脚好像是在爬一个隐性的楼梯，也可能没有动作。</p>
    <p class="content tip"><label class="point">◉</label> 头大多时候在外面，头可能往后仰（小孩儿可能前倾。</p>
    <p class="content tip"><label class="point">◉</label> 双眼睁开但无神，无法聚焦或者紧闭双眼。</p>
    <p class="content tip"><label class="point">◉</label> 嘴巴半闭，嘴巴有时候在外面，一上一下的浮动。</p>
    <p class="content tip"><label class="point">◉</label> 也可能就是直立在水中没有动作，当然头部还是上下浮动。</p>
    <p class="content tip"><label class="point">◉</label> 从远看就像是人站在水中抬着头看风景。</p>
    <p class="content prompts">1、溺水的人是静静站在水里，而不是平躺在水里。很多时候是站立的样子，或者好像在水里垂直爬一个隐形的楼梯。头大多数在水面上，嘴巴有时候在水外，有时候在水里，上下浮动。总而言之，当孩子玩水时候没有了声音就要注意了。</p>
    <p class="little-tip">2、怎么才能更保证自己的安全呢？</p>
    <p class="content prompts">（1）在安全的环境内游泳</p>
    <p class="content tip"><label class="point">◉</label> 不要远离身边的朋友。</br>比如很多人就认为自己有水性会游泳就会独自一人去游玩，出了一点意外很多时候不关注你的人是发现不了的~</p>
    <p class="content tip"><label class="point">◉</label> 不要放松警惕。</br>不论是在深水区还是浅水区都不要放松警惕，有很多人在很浅的泳池都会溺水，溺水和深度没有绝对关系~</p>
    <p class="content tip"><label class="point">◉</label> 了解干性溺水。</br>游泳呛水后，可能导致喉部痉挛，上岸后因喉部肌肉痉挛关闭呼吸道导致缺氧而亡的。~</p>
    <p class="content prompts">（2）溺水不慌张</p>
    <p class="content tip">发现自己身体动不了或者升不上去之后不要慌张，不要将体内的气放出去，降低身体密度，双手双脚平摊开，尽量让自己平躺浮起来。</p>
    <p class="content prompts">（3）了解溺水后救治常识</p>
    <p class="content tip"><label class="point">◉</label> 通畅呼吸道。</br>将溺水者抬出水面后，迅速清楚口、鼻腔内污水污物或其它异物，将溺水者舌头拉出口外，解开衣领衣扣，保持呼吸道通畅。</p>
    <p class="content tip"><label class="point">◉</label> 倒水。</br>抱起溺水者腰腹部，使其背朝上、头下垂进行倒水。</p>
    <p class="content tip"><label class="point">◉</label> 人工呼吸。</br>托起溺水者下颌，捏住鼻孔，生息一口气后，往其嘴里缓缓吹气，待其胸廓稍微有抬起时，放松其鼻孔，并用一手压其胸部以助呼吸。反复并有节律的进行。（每分钟吹16--20次）</p>
    <p class="content tip"><label class="point">◉</label> 心脏按压。</br>心脏骤停者应先进行胸外心脏按压。</p>

    <p class="little-tip">3、小程序使用介绍</p>
    <p class="content prompts">（1）登录界面</p>
    <p class="content tip"><label class="point">◉</label> 登录注册功能：用户通过手机号自己设定账户、密码即可注册成功。输入账户密码即可登录成功。</p>
    <p class="content tip"><label class="point">◉</label> 找回密码：忘记密码时点击忘记密码输入注册时填入的手机号即可重新设置密码</p>
   
    <p class="content prompts">（2）监测页面</p>
    <p class="content tip"><label class="point">◉</label> 授权登录：用户初次登录此界面会弹出定位授权与微信登录授权，以获得用户位置信息和微信openid等信息。</p>
    <p class="content tip"><label class="point">◉</label> 天气和定位：对手机用户进行定位，并显示天气、空气质量等信息。</p>
    <p class="content tip"><label class="point">◉</label> 体征监测：实时显示游泳者的温度、心率、压力等信息，并在溺水时报警按钮自动打开。</p>
    <p class="content tip"><label class="point">◉</label> 定位追踪：点击定位追踪按钮即可对游泳者进行定位并将规划出一条到定位点最近的一条路线。</p>
    <p class="content tip"><label class="point">◉</label> 下拉刷新：网络状态不好时，用户下拉刷新即可重新连接服务器</p>
   
    <p class="content prompts">（3 记录页面</p>
    <p class="content tip"><label class="point">◉</label> 记录显示：将游泳者的体征信息记录在此，每页15条记录，往下滑动到底部自动加载下一页。</p>
    <p class="content tip"><label class="point">◉</label> 记录备注：每一条记录右端有添加备注按钮，点击即可添加对于这条记录的备注，记录后添加备注将变成修改按钮，点击即可修改。</p>
    <p class="content tip"><label class="point">◉</label> 下拉刷新：用户下滑记录页面即可重新调用读取数据库的操作以更新记录。</p>

    <p class="content prompts">（4 我的页面</p>
    <p class="content tip"><label class="point">◉</label> 头像与昵称：通过用户微信授权登录的信息，获取用户微信头像及昵称并显示到顶部。</p>
    <p class="content tip"><label class="point">◉</label> 操作指引：点击操作指引即可打开此页面，内有小程序功能介绍和溺水小知识。</p>
    <p class="content tip"><label class="point">◉</label> 小程序版本号：显示本小程序的版本。</p>
    <p class="content tip"><label class="point">◉</label> 数据清零：点击清空清零将会弹出是否确定清空弹框，确定后将会把所有数据归零。</p>
    <p class="content tip"><label class="point">◉</label> 意见反馈：对于小程序的建议、问题反馈、合作信息等可通过文字、截图以及留下微信号的方式与开发人员沟通。</p>
    
    </div>
</template>

<script>
export default {
  data() {
    return {
      src: "../../static/images/ssly.png",
    };
  },
};
</script>

<style lang='scss'>
.instruction {
  .title {
    background: #e2f107;
    border-radius: 16rpx;
    font-size: 16px;
    .img {
      width: 100%;
      height: 120px;
    }
    .content-title {
      padding: 4px 20px 10px 20px;
    }
  }
  .content {
    padding: 4px 15px;
    font-size: 16px;
    font-weight: bold;
  }
  .little-tip {
    padding: 15px 15px 3px 15px;
    font-size: 15px;
    font-weight: bold;
    color: #e2f107;
  }
  .tip {
    font-size: 15px;
    font-weight: normal;
    .point {
      color: #e2f107;
    }
  }
  .prompts {
    font-size: 15px;
  }
  .index-img {
    display: block;
    margin: 10px auto;
    height: 560px;
    border-radius: 10px;
    margin-bottom: 30px;
  }
}
</style>
