const path = require("path");
function resolve(dir) {
  return path.join(__dirname, dir);
}

module.exports = {
  publicPath: './',
  outputDir: process.env.VUE_APP_outputDir || 'dist',
  assetsDir: 'static',
  filenameHashing: true,
  lintOnSave: false,
  runtimeCompiler: false,
  transpileDependencies: [],
  productionSourceMap: false,
  css: {
    // 是否使用css分离插件 ExtractTextPlugin
    extract: process.env.NODE_ENV === "production" ? true : false,//是否将组件中的 CSS 提取至一个独立的 CSS 文件中 (而不是动态注入到 JavaScript 中的 inline 代码)。
    sourceMap: false,//是否为 CSS 开启 source map。设置为 true 之后可能会影响构建的性能。
    loaderOptions: {
      sass: {
        prependData: `@import "@/assets/css/variable.scss";`
      }
    },
    requireModuleExtension: true,
  },

  chainWebpack: (config) => {
    // 配置别名
    config.resolve.alias
      .set('@', resolve('src'))
      .set('assets', resolve('src/assets'))
      .set('assetsBig', resolve('src/pages/big-screen/assets'))
      .set('components', resolve('src/components'))
      .set('views', resolve('src/views'))
      .set('api', resolve('src/api'))
      .set('lib', resolve('src/lib'))

    if (process.env.NODE_ENV === "production") {
      // 删除系统默认的splitChunk
      config.optimization.delete("splitChunks");
    }
  },
  configureWebpack: config => {
    // 给输出的js名称添加hash
    config.output.filename = "static/js/[name].[hash].js";
    config.output.chunkFilename = "static/js/[name].[hash].js";
    config.optimization = {
      splitChunks: {
        cacheGroups: {
          // 抽离所有入口的公用资源为一个chunk
          common: {
            name: "chunk-common",
            chunks: "initial",
            minChunks: 2,
            maxInitialRequests: 5,
            minSize: 0,
            priority: 1,
            reuseExistingChunk: true,
            enforce: true
          },
          element: {
            name: "chunk-element-ui",
            test: /[\\/]node_modules[\\/]element-ui[\\/]/,
            chunks: "all",
            priority: 3,
            reuseExistingChunk: true,
            enforce: true
          },
          yhhtUi: {
            name: "chunk-yhht-ui",
            test: /[\\/]node_modules[\\/]yhht-ui[\\/]/,
            chunks: "all",
            priority: 4,
            reuseExistingChunk: true,
            enforce: true
          },
          datav: {
            name: "chunk-datav",
            test: /[\\/]node_modules[\\/]@jiaminghi[\\/]data-view[\\/]/,
            chunks: "all",
            priority: 4,
            reuseExistingChunk: true,
            enforce: true
          },
        }
      }
    };
  },
  parallel: require('os').cpus().length > 1,
  devServer: {
    host: '0.0.0.0',
    port: 8086
  },
  pluginOptions: {
  }
}
