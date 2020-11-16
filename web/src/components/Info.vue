<template>
  <div class="Info">
    <el-main>
          <el-form ref="form" :model="infoform" label-width="80px">
            <el-form-item label="手机号码">
              <el-input :disabled="true"  v-model="accid"></el-input>
            </el-form-item>
            <el-form-item label="性别">
              <el-input :disabled="true" v-model="infoform.sex"></el-input>
            </el-form-item>
                <el-form-item label="出生日期">
              <el-input  :disabled="true" :type="tpen" v-model="infoform.birthday"></el-input>
            </el-form-item>
            
             <el-button type="primary" @click="show()">{{showdata}}</el-button> 
             <el-button type="primary" @click="reset()">重置密码</el-button>
          </el-form>
        </el-main>
  </div>
</template>

<script>
import global from "@/api/global";
export default {
  data() {
    return {
      showdata: "显示生日",
      n: "1",
      tpen: "password",
      time: "",
      name: "",
      accid: global.accid,
      sex:global.sex,
      bb: "",
      infoform: {
        sex: "",
        birthday: 9999999999,
      },
    };
  },
  
  created() {
    this.name="";
    this.addDate();
    this.getinfo();
  },

  methods:{
/*     check()
    {
        if(global.accid=="")
          {
               this.$alert("请重新登录", {
                  confirmButtonText: "确定",
                  callback: (action) => {
                      this.$router.push({ path: "/" });
                  },
                });
          }
    }, */
    reset()
    {
      this.$router.push({ path: "/Reset" });
    },
   show()
    {
        if(this.n=="1"||this.n==1)
        {
        this.infoform.birthday = this.bb
        this.tpen="text";
        this.n="0";
        this.showdata="隐藏生日"
        }
        else
        {
          this.infoform.birthday = 999999
          this.tpen="password";
          this.n="1";
          this.showdata="显示生日"
        }
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
		      this.time= year + "-" + month + "-" + strDate;
      },
      getinfo() {
      axios({
        method: 'post',
        //url:'http://127.0.0.1:25003/query/info', 
        url:'http://localhost:3000/info', 
        headers: {
   /*         'Content-type': 'application/x-www-form-urlencoded', */
          "Content-Type": "application/json", 
          //"token":global.jwt,
        },
        data:JSON.stringify( 
        {
          'accId':global.accid,
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
            this.accid=global.accid
            global.name = response.data.name;
            this.infoform.sex=response.data.sex;
            this.name=global.name;
            this.bb=response.data.birthday;
            }
        })
        .catch(function (error) {
          console.log(error);
        });
    },
  }
};

</script>
<style>

</style>