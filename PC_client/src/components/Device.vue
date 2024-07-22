<template>
  <div>
    <el-form :inline="true" style="margin-bottom: 20px;">
      <el-form-item label="搜索设备">
        <el-input
            v-model="searchInput"
            placeholder="请输入设备ID">
        </el-input>
      </el-form-item>
      <el-form-item>
        <el-button
            type="primary"
            @click="showAddDialog = true">
          添加设备
        </el-button>
      </el-form-item>
    </el-form>

    <el-dialog
        title="添加设备"
        :visible.sync="showAddDialog"
        width="30%">
      <el-form ref="addForm" :model="addForm">
        <el-form-item label="设备ID" prop="deviceId">
          <el-input v-model="addForm.deviceId"></el-input>
        </el-form-item>
      </el-form>
      <span slot="footer" class="dialog-footer">
        <el-button @click="showAddDialog = false">取消</el-button>
        <el-button type="primary" @click="handleAdd">添加</el-button>
      </span>
    </el-dialog>

    <el-table
        :data="filteredTableData"
        style="width: 100%">
      <el-table-column
          prop="device_id"
          label="设备ID">
      </el-table-column>
      <el-table-column
          prop="site"
          label="设备地点">
      </el-table-column>
      <el-table-column
          prop="live"
          label="在线状态">
      </el-table-column>
      <el-table-column
          label="操作">
        <template slot-scope="scope">
          <el-button
              type="text"
              @click="handleDelete(scope.$index)">
            删除设备
          </el-button>
        </template>
      </el-table-column>
    </el-table>
  </div>
</template>

<script>
import {DeviceInfoAPI,DeviceDelete,DeviceLogin} from "@/plugins";


export default {
  data() {
    return {
      tableData: [],
      searchInput: '',
      showAddDialog: false,
      addForm: {
        device_id: ''
      }
    };
  },
  computed: {
    filteredTableData() {
      if (!this.searchInput) return this.tableData;
      return this.tableData.filter(item => item.device_id.includes(this.searchInput.trim()));
    }
  },
  methods: {
    handleDelete(index) {
      const deviceIdToDelete = this.tableData[index].device_id;
      this.tableData.splice(index, 1);
      DeviceDelete(deviceIdToDelete);
    },

    handleAdd() {
      DeviceLogin(this.addForm.deviceId);
      this.showAddDialog = false;
      this.$refs.addForm.resetFields(); // 重置表单
    },

    getDeviceInfo() {
      DeviceInfoAPI().then((res) => {
        this.tableData = res.data
      }).catch((error) => {
        console.error("Error fetching data: ", error);
      });
    }
  },
  beforeDestroy() {
    clearInterval(this.intervalId);
  },
  mounted() {
    this.getDeviceInfo();
    this.intervalId = setInterval(() => {
      this.getDeviceInfo();
    }, 1500);
  },
};
</script>


<style>

</style>
