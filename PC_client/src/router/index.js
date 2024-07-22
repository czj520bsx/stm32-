import Vue from 'vue';
import Router from 'vue-router';
import login from "@/views/Login.vue";
import home from "@/views/Home.vue";
import index from "@/views/Index.vue";
import monitoringCenter from "@/views/MonitoringCenter.vue";
import setting from "@/views/Setting.vue";
import dataCenter from "@/views/DataCenter.vue";
import map from "@/components/Map.vue";
import device from "@/components/Device.vue";

Vue.use(Router);

const router = new Router({
    routes: [
        {
            path: '/',
            redirect:'/login',
        },
        {
            path:'/home',
            redirect:'home/index'
        },

        {
            path: '/home',
            name: 'home',
            component: home,
            children:[
                //子路由
                {path: 'index',component: index}, //首页
                {path: 'monitoringCenter',component: monitoringCenter}, //监控中心
                {path: 'setting',component: setting}, //设备控制
                {path: 'dataCenter',component: dataCenter}, //历史数据
                {path: 'place',component: map}, //历史数据
                {path: 'device',component: device}, //历史数据
            ],
        },
        {
            path: '/login',
            name: 'login',
            component: login
        },
    ],
});

const originalPush = Router.prototype.push;
Router.prototype.push = function push(location) {
    return originalPush.call(this, location).catch((err) => {
        if (err.name === 'NavigationDuplicated') {
            console.warn('NavigationDuplicated error occurred. Skipping duplicate navigation.');
        } else {
            throw err;
        }
    });
};
export default router;