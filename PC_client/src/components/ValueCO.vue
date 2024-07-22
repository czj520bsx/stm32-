<template>
  <div class="echarts-container" ref="echartsCO"></div>
</template>

<script>
import * as echarts from 'echarts';
import {COValueAPI} from "@/plugins";

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
    this.getCOvalue()
    this.intervalId = setInterval(() => {
      this.getCOvalue();
    }, 1000);
  },
  methods: {
    initChart() {
      const chart = echarts.init(this.$refs.echartsCO);
      const option = {
        title: {
          text: '一氧化碳含量',
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
          data: this.data.map(d => d.co_data),
          type: 'line',
        }],
      };
      chart.setOption(option);
    },

    getCOvalue() {
      COValueAPI().then((res) => {
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
