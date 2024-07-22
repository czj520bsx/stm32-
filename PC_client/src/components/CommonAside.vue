<template>
  <div>
    <el-menu
        default-active="1"
        class="el-menu-vertical-demo"
        background-color="#545c64"
        text-color="#fff"
        active-text-color="#ffd04b"
        router
    >
      <el-menu-item
          index="/home/index"
          :class="{ 'active-item': isActive('/home/index') }"
      >
        <i class="el-icon-house"></i>
        <span slot="title">首页</span>
      </el-menu-item>
      <el-menu-item
          index="/home/monitoringCenter"
          :class="{ 'active-item': isActive('/home/monitoringCenter') }"
      >
        <i class="el-icon-monitor"></i>
        <span slot="title">监测中心</span>
      </el-menu-item>
      <el-menu-item
          index="/home/dataCenter"
          :class="{ 'active-item': isActive('/home/dataCenter') }"
      >
        <i class="el-icon-warning"></i>
        <span slot="title">历史数据</span>
      </el-menu-item>
      <el-menu-item
          index="/home/setting"
          :class="{ 'active-item': isActive('/home/setting') }"
      >
        <i class="el-icon-setting"></i>
        <span slot="title">设备控制</span>
      </el-menu-item>
      <el-menu-item
          index="/home/place"
          :class="{ 'active-item': isActive('/home/place') }"
      >
        <i class="el-icon-s-flag"></i>
        <span slot="title">设备分布</span>
      </el-menu-item>

      <el-menu-item
          index="/home/device"
          :class="{ 'active-item': isActive('/home/device') }"
      >
        <i class="el-icon-menu"></i>
        <span slot="title">设备中心</span>
      </el-menu-item>
    </el-menu>
  </div>
</template>

<script>
export default {
  methods: {
    isActive(route) {
      return this.$route.path === route;
    },
  }
};
</script>

<style>
.el-menu-vertical-demo {
  width: 200px;
  height: 100vh;
}

</style>
