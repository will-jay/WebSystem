<!--  -->
<template>
  <div class="Details">

    <el-container>
  <el-aside width="200px">疗程</el-aside>
  <el-container>
    <el-main>
        <el-table :data="tableData" style="width: 100%" align="center" >

        


<!--         <el-table-column prop="execOrganno" label="序号" width="150" align="center"></el-table-column>

        <el-table-column prop="execDate" label="机器" width="150" align="center"></el-table-column>

        <el-table-column prop="amount" label="穴位" width="150" align="center"></el-table-column>

        <el-table-column prop="balance" label="磁场大小" width="150" align="center"></el-table-column>

        <el-table-column prop="txnId" label="波形" width="150" align="center"></el-table-column>

        <el-table-column prop="starttime" label="开始时间" width="150" align="center"></el-table-column>

        <el-table-column prop="time" label="持续时间" width="150" align="center"></el-table-column>

         <el-table-column fixed="right" label="操作" width="150" align="center">
        <template slot-scope="scope">
        <el-button @click="handleClick(scope.row)" type="text" size="small">编辑</el-button>
        <el-button type="text" size="small">删除</el-button>
      </template>
    </el-table-column> -->
    
      </el-table>
    </el-main>

    <el-footer>
      <el-row>
   <el-button type="primary" plain>新建疗程</el-button>
   <el-button type="info" plain>删除疗程</el-button>
  <el-button type="success" plain>增加步骤</el-button>
  <el-button type="warning" plain>保存修改</el-button>
</el-row>
    </el-footer>
  </el-container>
</el-container>


<!--     <el-main>


   <el-select v-model="value" placeholder="请选择">
    <el-option
      v-for="item in options"
      :key="item.value"
      :label="item.label"
      :value="item.value">
    </el-option>
  </el-select>





      <el-table :data="tableData"  style="width: 100%">
        <el-table-column prop="execOrganno" label="执行机构" width="200"></el-table-column>

        <el-table-column prop="execDate" label="交易日期" width="200"></el-table-column>

        <el-table-column prop="amount" label="发生额" width="200"></el-table-column>

        <el-table-column prop="balance" label="余额" width="200"></el-table-column>

        <el-table-column prop="currType" label="币种" width="200">
          <template
            slot-scope="scope"
          >{{ scope.row.currType == 0 ? '人民币' : scope.row.currType == 1 ?'其他币种': '' }}</template>
        </el-table-column>

        <el-table-column prop="txnId" label="交易订单号" width="250"></el-table-column>
      </el-table>
    </el-main> -->
  </div>
</template>

<script>
import global from "@/api/global";
export default {
  data() {
    return {
       options: [],
        value:"",
      time: "",
      name: global.name,
      tableData: [],
      accid: global.accid,
    };
  },
  created() {
    this.addDate();
    this.getinfo();
  },
  methods: {
    addDate() {
      var date = new Date();
      var seperator1 = "-";
      var year = date.getFullYear();
      var month = date.getMonth() + 1;
      var strDate = date.getDate();

      if (month >= 1 && month <= 9) {
        month = "0" + month;
      }
      if (strDate >= 0 && strDate <= 9) {
        strDate = "0" + strDate;
      }
      this.time = year + "-" + month + "-" + strDate;
    },
    getinfo() {
      axios({
        method: "post",
     //    url: "http://127.0.0.1:25008/details/query/detail", 
        url:"http://127.0.0.1:25002/query/detail",
      //    url:'http://10.23.14.167:25002/query/detail', 
        headers: {
          "Content-Type": "application/json",
          token: global.jwt,
        },
        data: JSON.stringify({
          currType: "1",
          execOrganno: "123",
          execTellerno: "123",
          txnCode: "1",
          accId: global.accid,
          txnDate: this.time,
        }),
      })
        .then((response) => {
          if (
            global.accid == "" ||
            response.data.code == 500 ||
            response.data.code == "500"
          ) {
            this.$alert(response.data.msg, "当前网页环境不安全，请重新登录", {
              confirmButtonText: "确定",
              callback: (action) => {
                this.$router.push({ path: "/" });
              },
            });
          } else {
            this.tableData = JSON.parse(response.data.账号信息);
          }
        })
        .catch(function (error) {
          this.$alert(response.data.msg, "当前网页环境不安全，请重新登录", {
            confirmButtonText: "确定",
            callback: (action) => {
              this.$router.push({ path: "/" });
            },
          });
          console.log("error");
        });
    },
  },
};

// };
</script>
<style>
  .el-header {
    background-color: #B3C0D1;
    color: #333;
    text-align: center;
    line-height: 60px;
  }
    .el-footer {
    text-align: center;
    line-height: 60px;
  }
  
  .el-aside {
    background-color: #D3DCE6;
    color: #333;
    text-align: center;
    line-height: 458px;
  }
  
  body > .el-container {
    margin-bottom: 40px;
  }
  
  .el-container:nth-child(5) .el-aside,
  .el-container:nth-child(6) .el-aside {
    line-height: 260px;
  }
  
  .el-container:nth-child(7) .el-aside {
    line-height: 320px;
  }
</style>