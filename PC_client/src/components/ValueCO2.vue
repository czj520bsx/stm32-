<template>
  <div class="echarts-container" ref="echartsCO"></div>
</template>

<script>
import * as echarts from 'echarts';
import {CO2ValueAPI} from "@/plugins";

export default {
  data() {
    return {
      data: [],
    }
  },
  beforeDestroy() {
    clearInterval(this.intervalId);
  },
  mounted() {
    this.getCO2value();
    this.intervalId = setInterval(() => {
      this.getCO2value();
    }, 1000);
  },
  methods: {
    initChart() {
      const chart = echarts.init(this.$refs.echartsCO);
      const option = {
        title: {
          text: '二氧化碳含量',
          left: 'center',
        },
        tooltip: {
          trigger: 'axis',
        },
        xAxis: {
          type: 'category',
          data: this.data.map(d => d.time),
        },
        yAxis: {
          type: 'value',
        },
        series: [{
          data: this.data.map(d => d.co2_data),
          type: 'line',
        }],
      };
      chart.setOption(option);
    },

    getCO2value() {
      CO2ValueAPI().then((res) => {
        this.data = res.data
        this.initChart(); // 确保在数据获取后再初始化图表
      }).catch((error) => {
        console.error("Error fetching data: ", error);
      });
    }
  }
};
</script>

<style scoped>
.echarts-container {
  height: 100%;
}
</style>
