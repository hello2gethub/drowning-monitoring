const { mysql } = require('../qcloud')

module.exports = async (ctx) => {
    const { openid } = ctx.request.query
    try {
        const res = await mysql('records')
            .where('openid', openid)
            .orderBy('id', 'desc')
            .first()
        if (res) {
            var Temp = res.Temp, Press = res.Press, Heart = res.Heart;
        } else {
            var Temp = 0, Press = 0, Heart = 0;
        }
        ctx.state.data = {
            code: 0,
            msg: "success",
            Temp: Temp,
            Press: Press,
            Heart: Heart,
        }
        console.log("执行成功")
    } catch (e) {
        ctx.state = {
            code: -1,
            data: {
                msg: "添加失败" + e.sqlMessage
            }
        }
        console.log("执行错误", e)
    }
}