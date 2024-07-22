<template>
  <div class="card-container">
    <el-card v-for="(sensor, index) in sensors" :key="index" class="box-card">
      <p>{{ sensor.name }}</p>
      <el-slider v-model="sensor.value" vertical height="350px" :min="0" :max="sensor.max" @change="updateSensorValue(sensor.value, sensor.type)"></el-slider>
      <p>{{ sensor.label }}: {{ sensor.value }}</p>
      <el-switch v-model="sensor.switchValue" active-text="on" inactive-text="off" @change="updateSwitchValue(sensor.switchValue, sensor.switchType)"></el-switch>
    </el-card>
  </div>
</template>

<script>
import { changeValueAPI,changeSwitchAPI } from "@/plugins";

export default {
  data() {
    return {
      sensors: [
        { name: '一氧化碳传感器', value: this.$store.state.CO_value, max: 50, type: 'CO', label: '当前一氧化碳阈值', switchValue: this.$store.state.switch_CO, switchType: 'CO' },
        { name: '二氧化碳传感器', value: this.$store.state.CO2_value, max: 80, type: 'CO2', label: '当前二氧化碳阈值', switchValue: this.$store.state.switch_CO2, switchType: 'CO2' },
        { name: '甲烷传感器', value: this.$store.state.CH4_value, max: 50, type: 'CH4', label: '当前甲烷阈值', switchValue: this.$store.state.switch_CH4, switchType: 'CH4' },
        { name: '氢气传感器', value: this.$store.state.H2_value, max: 50, type: 'H2', label: '当前氢气阈值', switchValue: this.$store.state.switch_H2, switchType: 'H2' },
      ]
    }
  },
  methods: {
    updateSensorValue(value, type) {
      switch (type) {
        case 'CO':
          this.$store.commit('setCOValue', value);
          break;
        case 'CO2':
          this.$store.commit('setCO2Value', value);
          break;
        case 'CH4':
          this.$store.commit('setCH4Value', value);
          break;
        case 'H2':
          this.$store.commit('setH2Value', value);
          break;
      }
      changeValueAPI(this.$store.state.CH4_value, this.$store.state.H2_value, this.$store.state.CO_value, this.$store.state.CO2_value);
    },
    updateSwitchValue(value, type) {
      switch (type) {
        case 'CO':
          this.$store.commit('setSwitchCO', value);
          break;
        case 'CO2':
          this.$store.commit('setSwitchCO2', value);
          break;
        case 'CH4':
          this.$store.commit('setSwitchCH4', value);
          break;
        case 'H2':
          this.$store.commit('setSwitchH2', value);
          break;
      }
      changeSwitchAPI(this.$store.state.switch_CH4, this.$store.state.switch_H2, this.$store.state.switch_CO, this.$store.state.switch_CO2);
    },
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
  width: 300px;
  height: 620px;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
}

.el-slider.is-vertical/deep/ {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
}

.el-switch/deep/ {
  display: inline-flex;
  justify-content: space-between;
  align-items: center;
  position: relative;
  font-size: 14px;
  line-height: 20px;
  height: 20px;
  vertical-align: middle;
  padding-left: 40px;
}

p {
  text-align: center;
  font-size: 20px;
  margin-top: 40px;
  margin-bottom: 40px;
}
</style>
