<template>
  <div class="warning-table">
    <el-table
        :data="tableData"
        border
        style="width: 100%">
      <el-table-column
          prop="time"
          label="时间"
          width="180">
      </el-table-column>
      <el-table-column
          prop="info"
          label="预警信息"
          show-overflow-tooltip
          align="center">
        <template slot-scope="scope">
          <div class="warning-content">{{ scope.row.info }}</div>
        </template>
      </el-table-column>
    </el-table>
  </div>
</template>

<script>
import { ErrorInfoAPI } from "@/plugins";

export default {
  data() {
    return {
      tableData: []
    }
  },
  methods: {
    fetchErrorInfo() {
      ErrorInfoAPI().then((res) => {
        console.log(this.tableData)
        this.tableData = res.data;
      }).catch((error) => {
        console.error("Error fetching data: ", error);
      });
    }
  },
  mounted() {
    this.intervalId = setInterval(() => {
      this.fetchErrorInfo();
    }, 1000);
  },
  beforeDestroy() {
    clearInterval(this.intervalId);
  },

}
</script>

<style scoped>

</style>
