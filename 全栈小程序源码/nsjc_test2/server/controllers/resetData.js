const { mysql } = require('../qcloud')

module.exports = async (ctx) => {
    const { openid } = ctx.request.body
    try {
        let note = "清零"
        let Temp = 0
        let Press = 0
        let Heart = 0
        let Beep = false
        await mysql('records').insert({
            openid, Temp, Heart, Press, Beep, note
        })
        ctx.state.data = {
            code: 0,
            msg: "success",
        }
        console.log("清空记录执行成功")
    } catch (e) {
        ctx.state = {
            code: -1,
            data: {
                msg: "清空记录失败" + e.sqlMessage
            }
        }
        console.log("执行清空记录失败的错误信息", e)
    }
}