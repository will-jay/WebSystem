<template>
<div class="login">
  <h1>欢迎使用智能磁疗仪云服务系统</h1>
  <el-form v-show="!flag_forgetPwd" :model="dataForm" :rules="dataRule" ref="dataForm">
    <el-form-item prop="accid">
      <el-input v-model="dataForm.accid" placeholder="手机号" @input="onInput()"></el-input>
    </el-form-item>
    <el-form-item prop="password">
      <el-input v-model="dataForm.password" show-password type="password" placeholder="密码" @input="onInput()"></el-input>
    </el-form-item>
    <el-form-item >
          <el-radio v-model="dataForm.type" label="1" border size="medium" >普通用户</el-radio>
          <el-radio v-model="dataForm.type" label="2" border size="medium" >医生</el-radio>
    </el-form-item>
    <el-form-item>
      <el-button type="primary" @click="login('dataForm')">登录</el-button>
      <el-button @click="openacc()">注册</el-button>
    </el-form-item>
    <el-button type="text" @click="forgetPwd()">忘记密码？</el-button>
  </el-form>

  <el-form
    v-show="flag_forgetPwd"
    :model="setupPwdform"
    :rules="setupPwdRule"
    ref="setupPwdform"
    label-width="180px"
  >
    <font size="3" color="brown">请提供以下信息用于重置密码:</font>
<!--     <el-form-item label="账号：" prop="accid">
      <el-input v-model="setupPwdform.accid" placeholder="请输入账号" @input="onInput()"></el-input>
    </el-form-item> -->
  <!--    <el-form-item label="身份证号码：" prop="IDnumber">
      <el-input v-model="setupPwdform.IDnumber" placeholder="请输入身份证号码" @input="onInput()"></el-input>
    </el-form-item> -->

    <el-form-item label="手机号码：" prop="phone">
      <el-input v-model="setupPwdform.phone" placeholder="请输入手机号码" @input="onInput()"></el-input>
    </el-form-item>

      <el-form-item  label="验证码：" prop="checkcode">
        <el-input   placeholder="请输入验证码" v-model="setupPwdform.checkcode" @input="onInput()">
          <el-button slot="append"  @click="getcode()">获取验证码</el-button>
      </el-input>
    </el-form-item>

    <!-- <el-form-item label="交易密码：" prop="payPwd">
      <el-input v-model="setupPwdform.payPwd" placeholder="交易密码" show-password @input="onInput()"></el-input>
    </el-form-item> -->

    <el-form-item label="新密码：" prop="newPwd">
      <el-input
        v-model="setupPwdform.newPwd"
        placeholder="请输入新密码"
        show-password
        @input="onInput()"
      ></el-input>
    </el-form-item>

    <el-form-item label="确认密码：" prop="checknewPwd">
      <el-input
        v-model="setupPwdform.checknewPwd"
        placeholder="请再次输入新密码"
        show-password
        @input="onInput()"
      ></el-input>
    </el-form-item>
    <br />
    <el-form-item>
      <el-button type="primary" @click="setupPwd('setupPwdform')">重置密码</el-button>
      <el-button @click="forgetPwd()">前往登录</el-button>
    </el-form-item>
  </el-form>
</div>
</template>
<script>
import global from "@/api/global";
import { Loading } from "element-ui";
export default {
data() {
/*   let validateIDnumber = (rule, value, callback) => {
    let reg =/^[1-9]\d{5}(18|19|20|(3\d))\d{2}((0[1-9])|(1[0-2]))(([0-2][1-9])|10|20|30|31)\d{3}[0-9Xx]$/;
    if (!reg.test(value)) {
      callback(new Error("身份证格式错误"));
    } else {
      callback();
    }
  }; */
  let validatePhonenumber = (rule, value, callback) => {
    let reg = /^\d{11}$/;
    if (!reg.test(value)) {
      callback(new Error("手机号不正确"));
    } else {
      callback();
    }
  };
    let validatecheckcode = (rule, value, callback) => {
    let reg = /^\d{6}$/;
    if (!reg.test(value)) {
      callback(new Error("验证码不正确"));
    } else {
      callback();
    }
  };
/*   let validatePayPass1 = (rule, value, callback) => {
    // 6位数字
    let reg = /^\d{6}$/;
    if (!reg.test(value)) {
      callback(new Error("交易密码错误"));
    } else {
      callback();
    }
  }; */

  let validatePass1 = (rule, value, callback) => {
    // 6-16位, 数字, 字母, 字符至少包含两种, 同时不能包含中文和空格
    let reg = /(?!^[0-9]+$)(?!^[A-z]+$)(?!^[^A-z0-9]+$)^[^\s\u4e00-\u9fa5]{6,16}$/;
    if (!reg.test(value)) {
      callback(new Error("密码长度需6-16位，数字，字母，字符至少包含两种"));
    } else {
      callback();
    }
  };
  //验证密码是否重复
  let validatePass2 = (rule, value, callback) => {
    if (value !== this.setupPwdform.newPwd) {
      callback(new Error("两次密码输入不一致"));
    } else {
      callback();
    }
  };
  let validateAcc = (rule, value, callback) => {
    let reg = /^\d{11}$/;
    if (!reg.test(value)) {
      callback(new Error("手机号不正确"));
    } else {
      callback();
    }
  };
 /* // luhn算法校验位
  function checkCard(cardNo) {
    if (isNaN(cardNo)) return false;
    if (cardNo.length != 16) {
      //判断长度
      return false;
    }
    var nums = cardNo.split("");
    var sum = 0;
    var index = 1;
    for (var i = 0; i < nums.length; i++) {
      if ((i + 1) % 2 == 0) {
        var tmp = Number(nums[nums.length - index]) * 2;
        if (tmp >= 10) {
          var t = tmp + "".split("");
          tmp = Number(t[0]) + Number(t[1]);
        }
        sum += tmp;
      } else {
        sum += Number(nums[nums.length - index]);
      }
      index++;
    }
    if (sum % 10 != 0) {
      return false;
    }
    return true;
  } */

  return {
    flag_forgetPwd: false,
    dataForm: {//用户登录信息
      accid: "13592873371",//账号
      password: "5213.0.zxc",//密码
      type:"1",//用户类型

    },
    dataRule: {
      accid: [
        { required: true, message: "手机号不能为空", trigger: "blur" },
        { validator: validateAcc, trigger: "blur" },
      ],
      password: [
        { required: true, message: "密码不能为空", trigger: "blur" },
      ],
    },

    setupPwdform: {//重置密码信息
      checkcode:"",//验证码
     // accid: "",//账号
     // IDnumber: "",
      phone: "",//手机号
      // payPwd: "",
      newPwd: "",//新密码
      checknewPwd: "",//确认密码
    },
    setupPwdRule: {
      /* accid: [
        { required: true, message: "账号不能为空", trigger: "blur" },
        { validator: validateAcc, trigger: "blur" },
      ], */
    /*   IDnumber: [
        { required: true, message: "身份证不能为空", trigger: "blur" },
        { validator: validateIDnumber, trigger: "blur" },
      ], */
      phone: [
        { required: true, message: "手机号不能为空", trigger: "blur" },
        { validator: validatePhonenumber, trigger: "blur" },
      ],
      // payPwd: [
      //   { required: true, message: "交易密码不能为空", trigger: "blur" },
      //   { validator: validatePayPass1, trigger: "blur" },
      // ],
      checkcode: [
        { required: true, message: "验证码不能为空", trigger: "blur" },
        { validator: validatecheckcode, trigger: "blur" },
      ],
      newPwd: [
        { required: true, message: "新密码不能为空", trigger: "blur" },
        { validator: validatePass1, trigger: "blur" },
      ],
      checknewPwd: [
        { required: true, message: "验证新密码不能为空", trigger: "blur" },
        { validator: validatePass2, trigger: "blur" },
      ],
    },
  };
},
methods: {
  //重置密码
  setupPwd(formName) {
    this.$refs[formName].validate((valid) => {
      if (valid) {
        axios.interceptors.request.use(
          function (config) {
            Loading.service({
              lock: true,
              text: "加载中……",
              background: "rgba(0, 0, 0, 0.7)",
            });
            return config;
          },
          function (error) {
            console.logo("req error");
            // 对请求错误做些什么
            return Promise.reject(error);
          }
        );
        // 添加响应拦截器
        axios.interceptors.response.use(
          function (response) {
            Loading.service().close();
            // 对响应数据做点什么
            return response;
          },
          function (error) {
            Loading.service().close();
            // 对响应错误做点什么
            return Promise.reject(error);
          }
        );
        axios({
          method: "post",
          url: "http://39.102.63.137/reset",

          headers: {
            "Content-Type": "application/json",
          },
          data: {
            accId: this.setupPwdform.accid,
            checkcode:this.setupPwdform.checkcode,
            //number: this.setupPwdform.IDnumber,
            phone: this.setupPwdform.phone,
            newpassword: this.setupPwdform.newPwd,
          },
        })
          .then((response) => {
            if (response.data.code == "0") {
              var msg = "重置密码成功";
              this.$alert(msg, "重置密码成功", {
                confirmButtonText: "前往登录",
                callback: (action) => {
                  this.forgetPwd();
                },
              });
            } else {
              this.$alert(response.data.msg, "重置密码失败", {
                confirmButtonText: "确定",
                callback: (action) => {
                  // this.$options.methods.back.bind(this)();
                },
              });
            }
          })
          .catch(function (error) {
            console.log(error);
          });
      }
    });
  },
  //进入重置密码页
  forgetPwd() {
    if (this.flag_forgetPwd == true) {
      this.$refs["setupPwdform"].resetFields();
      this.flag_forgetPwd = false;
    } else {
      this.$refs["dataForm"].resetFields();
      this.flag_forgetPwd = true;
    }
  },

  openacc() {
    this.$router.push({ path: "/OpenAcc" });
  },
  getcode(formName){
      if(this.setupPwdform.phone=="")
      {
          this.$message('手机号不能为空');
      }
      else{
        // 获取远端图片
        // 添加请求拦截器
        axios.interceptors.request.use(
          function (config) {
            Loading.service({
              lock: true,
              text: "加载中……",
              background: "rgba(0, 0, 0, 0.7)",
            });
            return config;
          },
          function (error) {
            console.logo("req error");
            // 对请求错误做些什么
            return Promise.reject(error);
          }
        );

        // 添加响应拦截器
        axios.interceptors.response.use(
          function (response) {
            Loading.service().close();
            // 对响应数据做点什么
            return response;
          },
          function (error) {
            Loading.service().close();
            // 对响应错误做点什么
            return Promise.reject(error);
          }
        );

        axios({
          method: "post", 
              url:"http://39.102.63.137/getcode",
          headers: {
            "Content-Type": "application/json",
          },
          data: {
             phone: this.setupPwdform.phone,
          },
        })
          .then((response) => {
            if (response.data.code== 1||response.data.code== "1") { 
              this.$message("请求发送成功");
            } 
          else{
            this.$message("请求发送失败");
          } 
      })
          .catch(function (error) {
            console.log(error);
          });
      }
  },
  login(formName) {
    this.$refs[formName].validate((valid) => {
      if (valid) {
        // 获取远端图片
        // 添加请求拦截器
        axios.interceptors.request.use(
          function (config) {
            Loading.service({
              lock: true,
              text: "加载中……",
              background: "rgba(0, 0, 0, 0.7)",
            });
            return config;
          },
          function (error) {
            console.logo("req error");
            // 对请求错误做些什么
            return Promise.reject(error);
          }
        );

        // 添加响应拦截器
        axios.interceptors.response.use(
          function (response) {
            Loading.service().close();
            // 对响应数据做点什么
            return response;
          },
          function (error) {
            Loading.service().close();
            // 对响应错误做点什么
            return Promise.reject(error);
          }
        );

        axios({
          method: "post", 
              //url:"http://39.102.63.137/login",
              url:"http://localhost:3000/login",
          headers: {
            "Content-Type": "application/json",
          },
          data: {
            accid: this.dataForm.accid,
            password: this.dataForm.password,
            type: this.dataForm.type,
          },
        })
          .then((response) => {
            if (response.data.code== 10009||response.data.code== "10009") { 
              global.accid = this.dataForm.accid;
              global.type = this.dataForm.type;
              this.$router.push({ path: "/Info" });
            } 
            else if(response.data.code== 10008||response.data.code== "10008") { 
            this.n=2,

          this.$message(response.data.ret);
          }
          else{
            this.$message(response.data.msg);
          } 
      })
          .catch(function (error) {
            console.log(error);
          });
      }
    });
  },

  onInput() {
    this.$forceUpdate();
  },
},
};
</script>


<!-- Add "scoped" attribute to limit CSS to this component only -->
<style>
</style>