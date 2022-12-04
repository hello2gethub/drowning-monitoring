import config from './config'

export function showSuccess(text) {
    wx.showToast({
        title: text,
        icon: 'success',
        duration: 2000
    })
}

export function showModel(title, content) {
    wx.showModal({
        title: title,
        content: content,
        showCancel: false
    })
}

function request(url, method, data) {
    return new Promise(
        (resolve, reject) => {
            wx.request({
                url: config.host + url,
                method: method,
                data: data,
                success: function (res) {
                    console.log("请求成功，在前端打印信息", res);
                    if (res.data.code == 0) {
                        //pending状态变成resolved状态，异步事件执行成功
                        resolve(res.data.data)
                    } else {
                        //pending状态变成rejected状态，异步事件执行失败
                        reject(res.data)
                    }
                },
            });
        }
    )
}

export function get(url, data) {
    return request(url, "GET", data)
}

export function post(url, data) {
    return request(url, "POST", data)
}

