const { mysql } = require('../qcloud')

module.exports = async (ctx) => {
    const { id, note } = ctx.request.body
    try {
        await mysql("records").where("id", id).update('note', note)
        ctx.state.data = {
            code: 0,
            msg: "success",
        }
        console.log("修改记录成功")
    } catch (e) {
        ctx.state = {
            code: -1,
            data: {
                msg: "修改备注失败" + e.sqlMessage
            }
        }
        console.log("修改记录错误", e)
    }
}