<template>
  <div class="card-container">
    <el-card class="box-card">
      <div style="height: 300px">
        <value-c-o></value-c-o>
      </div>
    </el-card>

    <el-card class="box-card">
      <div style="height: 300px">
        <value-c-o2></value-c-o2>
      </div>
    </el-card>

    <el-card class="box-card">
      <div style="height: 300px">
        <value-c-h4></value-c-h4>
      </div>
    </el-card>

    <el-card class="box-card">
      <div style="height: 300px">
        <value-h2></value-h2>
      </div>
    </el-card>
  </div>
</template>

<script>
import ValueCO from "@/components/ValueCO.vue";
import ValueCH4 from "@/components/ValueCH4.vue";
import ValueCO2 from "@/components/ValueCO2.vue";
import ValueH2 from "@/components/ValueH2.vue";

export default {
  components: {
    ValueCO,
    ValueCH4,
    ValueCO2,
    ValueH2,
  },
};
</script>

<style scoped>
.card-container {
  display: flex;
  flex-wrap: wrap;
  justify-content: space-between;
}

.box-card {
  margin: 10px 10px;
  width: 600px;
  height: 295px;
}

p {
  text-align: center;
  font-size: 15px;
}
</style>
