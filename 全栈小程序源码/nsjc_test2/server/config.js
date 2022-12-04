const CONF = {
    serverHost:'https://nsjc.xyz',
    tunnelServerUrl:'',
    tunnelSignatureKey:'27fb7d1c161b7ca52d73cce0f1d833f9f5b5ec89',

    qcloudAppId:'1308311023',
    qcloudSecretId:'AKID1sUqvkJbeQv96mBJAk5Azf8j4A0ZsteB',
    qcloudSecrertkey:'IK5ZmkU3C9rLZXFivsa3GDuLXTAbniNm',
    wxMessageToken:'weixinmsgtoken',
    
    networkTimeour:30000,

    port: '5757',
    rootPathname: '/mnt/nsjc/server',

    // 微信小程序 App ID
    appId: 'wx93b1147d3e05bd0a',

    // 微信小程序 App Secret
    appSecret: 'd36608b0bb35e11f854573eeb712cecd',

    // 是否使用腾讯云代理登录小程序
    //因为用的阿里云，所以这里必须要为false；腾讯云只申请了id没买O.O
    useQcloudLogin: false,

    /**
     * MySQL 配置，用来存储 session 和用户信息
     * 若使用了腾讯云微信小程序解决方案
     * 开发环境下，MySQL 的初始密码为您的微信小程序 appid
     */
    mysql: {
        host: 'localhost',
        port: 3306,
        user: 'root',
        db: 'cAuth',
        pass: 'qq3218428',
        char: 'utf8mb4'
    },

    cos: {
        /**
         * 地区简称
         * @查看 https://cloud.tencent.com/document/product/436/6224
         */
        region: 'ap-guangzhou',
        // Bucket 名称
        fileBucket: 'qcloudtest',
        // 文件夹
        uploadFolder: ''
    },

    // 微信登录态有效期
    wxLoginExpires: 7200,
    wxMessageToken: 'abcdefgh'
}

module.exports = CONF
