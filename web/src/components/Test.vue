<!--  -->
<template>
  <div class="Details">
    <el-container>
      <el-dialog
        title="疗程名"
        :visible.sync="dialogVisible"
        width="30%"
        :before-close="handleClose"
      >
        <span>请输入疗程名</span>
        <el-input v-model="treatname" placeholder="请输入疗程名"></el-input>
        <span slot="footer" class="dialog-footer">
          <el-button @click="dialogVisible = false">取 消</el-button>
          <el-button type="primary" @click="addtreatment()"
            >确 定</el-button
          >
        </span>
      </el-dialog>

      <el-aside width="200px">
        <el-container>
          <el-header>疗程 </el-header>
          <el-menu :default-active="selectindex" class="el-menu-vertical-demo">
            <el-menu-item
              v-for="(item, key) in treatment"
              :key="key"
              :index="key"
              @click="menuclick(key)"
            >
              <i class="el-icon-first-aid-kit"></i>
              <span slot="title">{{ item.value }}</span>
            </el-menu-item>
          </el-menu>
          <el-footer>
            <el-button plain @click="dialogVisible = true">增加疗程</el-button>
          </el-footer>
        </el-container>
      </el-aside>
      <el-container>
        <el-main>
          <el-table
            :data="this.tableData"
            style="width: 100%"
            align="center"
            highlight-current-row
            @row-click="handleCurrentChange"
          >
            <el-table-column prop="id" label="序号" width="150" align="center">
              <template slot-scope="scope">
                <span>{{ scope.$index }}</span>
              </template>
            </el-table-column>

            <el-table-column
              prop="machine"
              label="机器"
              width="150"
              align="center"
            >
              <template slot-scope="scope">
                <el-input-number
                  v-if="scope.row.isset"
                  size="small"
                  v-model="scope.row.machine"
                  label="请输入内容"
                  controls-position="right"
                  @change="handleEdit(scope.$index, scope.row)"
                  :min="1"
                  :max="10"
                ></el-input-number>
                <span v-else>{{ scope.row.machine }}</span>
              </template>
            </el-table-column>

            <el-table-column
              prop="Acupoint"
              label="穴位"
              width="150"
              align="center"
            >
              <template slot-scope="scope">
                <el-select
                  v-if="scope.row.isset"
                  v-model="scope.row.Acupoint"
                  filterable
                  placeholder="请选择"
                >
                  <el-option
                    v-for="item in Acupointoptions"
                    :key="item.value"
                    :label="item.label"
                    :value="item.value"
                  ></el-option>
                </el-select>
                <span v-else>{{ scope.row.Acupoint }}</span>
              </template>
            </el-table-column>

            <el-table-column
              prop="intensity"
              label="磁场强度等级"
              width="150"
              align="center"
            >
              <template slot-scope="scope">
                <el-input-number
                  v-if="scope.row.isset"
                  size="small"
                  v-model="scope.row.intensity"
                  label="请输入内容"
                  controls-position="right"
                  @change="handleEdit(scope.$index, scope.row)"
                  :min="1"
                  :max="6"
                ></el-input-number>
                <span v-else>{{ scope.row.intensity }}</span>
              </template>
            </el-table-column>

            <el-table-column
              prop="waveform"
              label="波形"
              width="150"
              align="center"
            >
              <template slot-scope="scope">
                <el-select
                  v-if="scope.row.isset"
                  v-model="scope.row.waveform"
                  filterable
                  placeholder="请选择"
                  @change="handleEdit(scope.$index, scope.row)"
                >
                  <el-option
                    v-for="item in waveformoptions"
                    :key="item.value"
                    :label="item.label"
                    :value="item.value"
                  ></el-option>
                </el-select>
                <span v-else>{{ scope.row.waveform }}</span>
              </template>
            </el-table-column>

            <el-table-column
              prop="starttime"
              label="开始时间"
              width="150"
              align="center"
            >
              <template slot-scope="scope">
                <el-time-picker
                  v-if="scope.row.isset"
                  v-model="scope.row.starttime"
                  :picker-options="{
                    selectableRange: '00:00:00 - 23:59:59',
                  }"
                  @change="handleEdit(scope.$index, scope.row)"
                  placeholder="任意时间点"
                >
                </el-time-picker>
                <span v-else>{{ scope.row.starttime }}</span>
              </template>
            </el-table-column>

            <el-table-column
              prop="duration"
              label="结束时间"
              width="150"
              align="center"
            >
              <template slot-scope="scope">
                <el-time-picker
                  v-if="scope.row.isset"
                  v-model="scope.row.duration"
                  :picker-options="{
                    selectableRange: '00:00:00 - 23:59:59',
                  }"
                  placeholder="任意时间点"
                >
                </el-time-picker>

                <span v-else>{{ scope.row.duration }}</span>
              </template>
            </el-table-column>

            <el-table-column
              fixed="right"
              label="操作"
              width="150"
              align="center"
            >
              <template slot-scope="scope">
                <el-button size="mini" @click="modifyClick(scope.row)">{{
                  scope.row.isset ? "保存" : "修改"
                }}</el-button>
                <el-button
                  size="mini"
                  type="danger"
                  @click="deleteclick(scope.$index, tableData)"
                  >删除</el-button
                >
              </template>
            </el-table-column>
          </el-table>
        </el-main>

        <el-footer>
          <el-row>
            <el-button type="primary" plain   :disabled="setdisabeld"   @click="deletetable(index)"
              >删除疗程</el-button
            >
            <el-button type="success" plain :disabled="setdisabeld" @click="addclick()"
              >添加步骤</el-button
            >
            <el-button type="warning" plain :disabled="setdisabeld">提交修改</el-button>
          </el-row>
        </el-footer>
      </el-container>
    </el-container>
  </div>
</template>

<script>
import global from "@/api/global";
export default {
  data() {
    return {
      setdisabeld:true,
      dialogVisible: false,
      selectindex: "0",
      nhjk: "1",
      treatment: [],
      Acupointoptions: [
        {
          value: "内关穴",
          label: "内关穴",
        },
        {
          value: "太冲穴",
          label: "太冲穴",
        },
        {
          value: "脾腧穴",
          label: "脾腧穴",
        },
        {
          value: "少商穴",
          label: "少商穴",
        },
        {
          value: "涌泉穴",
          label: "涌泉穴",
        },
      ],
      waveformoptions: [
        {
          value: "正弦波",
          label: "正弦波",
        },
        {
          value: "方波",
          label: "方波",
        },
        {
          value: "脉冲波",
          label: "脉冲波",
        },
        {
          value: "三角波",
          label: "三角波",
        },
        {
          value: "稳定波",
          label: "稳定波",
        },
      ],
      value: "",
      time: "",
      name: global.name,
      treatmentData: [
        {
          list: "偏头痛",
        },
        {
          list: "高血压",
        },
      ],
      tableData: [],
      accid: global.accid,
      treatname:"",
    };
  },
  created() {
    this.addDate();
    this.getinfo();
  },
  methods: {
    addtreatment() {
      let k = {
        value: this.treatname,
      };
      this.treatment.push(k);
      this.dialogVisible = falsel
    },
    menuclick(index) {
      this.setdisabeld=false;
      this.selectindex = index;
      console.log(this.selectindex);
       axios({
        method: 'post',
        //url:'http://127.0.0.1:25003/query/info', 
        url:'http://localhost:3000/detail', 
        headers: {
   /*         'Content-type': 'application/x-www-form-urlencoded', */
          "Content-Type": "application/json", 
          //"token":global.jwt,
        },
        data:JSON.stringify( 
        {
          accId:global.accid,
          numkey:this.selectindex,
        }), 
      })
           .then((response) => {
            if(global.accid==""||response.data.code==500||response.data.code=="500")
            {
                  this.$alert(response.data.msg, "当前网页环境不安全，请重新登录", {
                  confirmButtonText: "确定",
                  callback: (action) => {
                      this.$router.push({ path: "/" });
                  },
                });
            }
            else
            {
              this.tableData = response.data.table;
              console.log(this.tableData)
            }
        })
        .catch(function (error) {
          console.log(error);
        });
    },
    deletetable(index) {
      this.treatment.splice(this.selectindex, 1);
    },
    modifyClick(row) {
      if (row.isset) {
        row.isset = false;
        row.machine++;
      } else {
        row.isset = true;
        row.machine++;
      }
    },
    saveClick(row) {
      row.isset = false;
      row.id++;
    },
    deleteclick(index, rows) {
      rows.splice(index, 1);
    },
    addclick() {
      let j = {
        machine: "1",
        Acupoint: "内光穴",
        intensity: "1",
        waveform: "方波",
        starttime: "",
        duration: "",
      };
      this.tableData.push(j);
    },
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
        method: 'post',
        //url:'http://127.0.0.1:25003/query/info', 
        url:'http://localhost:3000/list', 
        headers: {
   //         'Content-type': 'application/x-www-form-urlencoded', 
          "Content-Type": "application/json", 
        },
        data:JSON.stringify( 
        {
          accId:global.accid,
        }), 
      })
           .then((response) => {
            if(global.accid==""||response.data.code==500||response.data.code=="500")
            {
                  this.$alert(response.data.msg, "当前网页环境不安全，请重新登录", {
                  confirmButtonText: "确定",
                  callback: (action) => {
                      this.$router.push({ path: "/" });
                  },
                });
            }
            else
            {
              this.treatment = response.data.table;
              console.log(this.tableData)
            }
        })
        .catch(function (error) {
          console.log(error);
        }); 
    },
  },
};

// };
</script>
<style>
.el-header {
  background-color: #b3c0d1;
  color: #333;
  text-align: center;
  line-height: 60px;
}
.el-footer {
  background-color: white;
  color: white;
  text-align: center;
  line-height: 60px;
}

.el-aside {
  background-color: #d3dce6;
  color: #333;
  text-align: center;
  line-height: 500px;
}

body > .el-container {
  margin-bottom: 400px;
}

.el-container:nth-child(5) .el-aside,
.el-container:nth-child(6) .el-aside {
  line-height: 260px;
}

.el-container:nth-child(7) .el-aside {
  line-height: 320px;
}
</style>