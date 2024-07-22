<template>
  <el-container>

    <el-aside width="200px">
      <common-aside></common-aside>
    </el-aside>

    <el-container>
      <el-header>
        <common-header></common-header>
      </el-header>

      <el-main>
        <router-view></router-view>
      </el-main>
    </el-container>
  </el-container>
</template>

<script>
import commonAside from "@/components/CommonAside.vue";
import commonHeader from "@/components/CommonHeader.vue";

export default {
  components: {
    commonAside,
    commonHeader
  },
  data() {
    return {
    };
  },
}
</script>

<style scoped>

</style>

