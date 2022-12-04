const { mysql } = require('../qcloud')

module.exports = async (ctx) => {
    const { openid,page } = ctx.request.query
    try {
        const records = await mysql("records")
            .where("openid", openid)
            .orderBy("id", "desc")
            .limit(15).offset(Number(page)*15)
        ctx.state.data = {
            records
        }
        console.log("执行成功")
    } catch (e) {
        ctx.state = {
            code: -1,
            data: {
                msg: "获取记录失败" + e.sqlMessage
            }
        }
        console.log("执行错误", e)
    }
}