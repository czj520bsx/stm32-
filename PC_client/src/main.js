import Vue from 'vue';
import ECharts from 'echarts'; // 导入 ECharts 组件
import App from './App.vue';
import router from './router';
import './plugins/axios';
import './plugins/element.js';
import './plugins/charts.js';
import store from './store'

Vue.config.productionTip = false;

Vue.component('e-charts', ECharts); // 注册 ECharts 组件

new Vue({
  router,
  store,
  render: h => h(App)
}).$mount('#app');
