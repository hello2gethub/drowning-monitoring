const { mysql } = require('../qcloud')

module.exports = async (ctx) => {
    try {
        const { opinion, src, wechat, openid } = ctx.request.body
        await mysql('opinions').insert({
            opinion, src, wechat, openid
        })
        ctx.state.data = {
            code: 0,
            msg: "success",
        }
        console.log("执行成功")
    } catch (e) {
        ctx.state = {
            code:-1,
            data:{
                msg:"添加失败" + e.sqlMessage
            }
        }
        console.log("执行错误")
    }
}