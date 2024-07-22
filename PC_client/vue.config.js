const { defineConfig } = require('@vue/cli-service')
module.exports = defineConfig({
  transpileDependencies: true,
  lintOnSave: false,//关闭eslint检查
  devServer: {
    host: '0.0.0.0',
    port: 8088
  },
})
