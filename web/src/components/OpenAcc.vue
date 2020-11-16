<template>
  <div class="openacc">
    <h1>欢迎使用智能磁疗仪云服务系统</h1>

    <el-form :model="regForm" :rules="rules" label-width="180px" ref="regForm">
  <!--     <el-form-item label="请选择地区、网点" prop="valueBranches">
        <el-cascader
          placeholder="请选择执行地区、网点"
          style="width: 350px;"
          v-model="regForm.valueBranches"
          :props="{value: 'text',label:'text'}"
          :options="branches"
        ></el-cascader>
      </el-form-item> -->

      <!-- <el-form-item label="请选择执行柜员" :required="true" prop="ExecTellerno">
        <el-select style="width: 350px;" v-model="regForm.ExecTellerno" placeholder="请选择执行柜员">
          <el-option
            v-for="item in execTellerno"
            :key="item.label"
            :label="item.label"
            :value="item.value"
          ></el-option>
        </el-select>
      </el-form-item> -->

      <el-form-item label="用户名" prop="name">
        <el-input placeholder="请输入用户名:" v-model="regForm.name" @input="onInput()"></el-input>
      </el-form-item>
     
      <el-form-item label="性别" prop="name">
     <el-select v-model="regForm.sex" placeholder="请选择"  style="width: 320px">
    <el-option
      v-for="item in options"
      :key="item.sex"
      :label="item.label"
      :value="item.sex">
    </el-option>
  </el-select>
      </el-form-item>

      <el-form-item label="手机号" prop="phone">
        <el-input placeholder="请输入手机号:" v-model="regForm.phone" @input="onInput()"></el-input>
      </el-form-item>

     <el-form-item  label="验证码：" prop="checkcode">
        <el-input   placeholder="请输入验证码" v-model="regForm.checkcode" @input="onInput()">
         <el-button slot="append"  @click="getcode()">获取验证码</el-button>
      </el-input>
    </el-form-item>
 <!--      <el-form-item label="身份证号" prop="id">
        <el-input placeholder="请输入身份证号:" v-model="regForm.id" @input="onInput()"></el-input>
      </el-form-item> -->

      <el-form-item label="登录密码:" prop="password">
        <el-input
          placeholder="请输入登录密码:"
          v-model="regForm.password"
          type="password"
          show-password
          @input="onInput()"
        ></el-input>
      </el-form-item>
      <el-form-item label="确认登录密码:" prop="checkPassword">
        <el-input
          placeholder="请再次输入登录密码:"
          v-model="regForm.checkPassword"
          type="checkPassword"
          show-password
          @input="onInput()"
        ></el-input>
      </el-form-item>
   
    <!--    <el-form-item label="交易密码:" prop="pay_password">
         
        <el-input
          placeholder="请输入交易密码:"
          v-model="regForm.pay_password"
          type="pay_password"
          show-password
          @input="onInput()"
        ></el-input>
         <font size="2" color="brown"> ⚠️交易密码用于交易操作，请注意与登录密码区分。</font>
      </el-form-item>

      <el-form-item label="确认交易密码:" prop="checkPay_password">
        <el-input
          placeholder="请再次输入交易密码:"
          v-model="regForm.checkPay_password"
          type="checkPay_password"
          show-password
          @input="onInput()"
        ></el-input>
      </el-form-item> -->


      <el-form-item>
        <el-checkbox v-model="checked" @input="onInput()">本人同意开通智能磁疗仪云服务系统账号。</el-checkbox>
        <br />
        <el-button type="primary" @click="submit('regForm')" v-if="checked">确认开户</el-button>
        <el-button type="primary" v-else disabled>注册</el-button>
        <el-button @click="resetForm('regForm')">重置</el-button>
        <el-button @click="back">返回</el-button>
      </el-form-item>
    </el-form>
  </div>
</template>

<script>
import branches from "../api/branchs";
import { Loading } from 'element-ui'
export default {
  data() {
    //自定义验证规则
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
      if (value !== this.regForm.password) {
        callback(new Error("两次密码输入不一致"));
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
/*     let validateIDnumber = (rule, value, callback) => {
      let reg = /^[1-9]\d{5}(18|19|20|(3\d))\d{2}((0[1-9])|(1[0-2]))(([0-2][1-9])|10|20|30|31)\d{3}[0-9Xx]$/;;
      if (!reg.test(value)) {
        callback(new Error("身份证格式错误"));
      } else {
        callback();
      }
    }; */
 let validatePhonenumber = (rule, value, callback) => {
      let reg = /^1[3-9]\d{9}$/;
      if (!reg.test(value)) {
        callback(new Error("手机号不正确"));
      } else {
        callback();
      }
    };
/*     let validatePayPass1 = (rule, value, callback) => {
      // 6位数字
      let reg = /^\d{6}$/;
      if (!reg.test(value)) {
        callback(new Error("密码长度需6位数字"));
      } else {
        callback();
      }
    };


    let validatePayPass2 = (rule, value, callback) => {
      if (value !== this.regForm.pay_password) {
        callback(new Error("两次密码输入不一致"));
      } else {
        callback();
      }
    }; */
    return {
      options: [{
          sex: '男',
          label: '男'
        }, {
          sex: '女',
          label: '女'
        }],
      regForm: {
        name: "",
        phone: "",
        sex: "",
        password: "",
        checkcode: "",
        checkPassword: "",
      },
      branches: branches,
      // execTellerno: [
      //   {
      //     value: 1,
      //     label: "柜员1",
      //   },
      //   {
      //     value: 2,
      //     label: "柜员2",
      //   },
      //   {
      //     value: 3,
      //     label: "柜员3",
      //   },
      //   {
      //     value: 4,
      //     label: "柜员4",
      //   },
      // ],

      checked: false,

      rules: {
/*         valueBranches: [
          { required: true, message: "请选择网点", trigger: "blur" },
        ],
        ExecTellerno: [
          { required: true, message: "请选择执行柜员", trigger: "blur" },
        ], */
        name: [{ required: true, message: "请输入姓名", trigger: "blur" }],
        phone: [{ required: true, message: "请输入电话号码", trigger: "blur" },
        { validator: validatePhonenumber, trigger: "blur" }],
        /* id: [{ required: true, message: "请输入身份证号", trigger: "blur" },
         { validator: validateIDnumber, trigger: "blur" }], */
        checkcode: [
        { required: true, message: "验证码不能为空", trigger: "blur" },
        { validator: validatecheckcode, trigger: "blur" },
      ],       
       password: [
          { required: true, message: "请输入登录密码", trigger: "blur" },
          { validator: validatePass1, trigger: "blur" },
        ],
        checkPassword: [
          { required: true, message: "请再次输入登录密码", trigger: "blur" },
          { validator: validatePass2, trigger: "blur" },
        ],
       /*  pay_password:[
          { required: true, message: "请输入与登录密码不同的交易密码", trigger: "blur" },
          { validator: validatePayPass1, trigger: "blur" },
        ],

        checkPay_password:[
          { required: true, message: "请再次输入与登录密码不同的交易密码", trigger: "blur" },
          { validator: validatePayPass2, trigger: "blur" },
        ],
 */

      },
    };
  },

  methods: {
    resetForm(formName) {
      this.$refs[formName].resetFields();
    },
    getcode(formName){
      if(this.regForm.phone=="")
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
             phone: this.regForm.phone,
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
    onInput() {
      this.$forceUpdate();
    },
    back() {
      this.$router.push({ path: "/" });
    },

    submit(formName) {
      this.$refs[formName].validate((valid) => {
        if (valid) {

      // 添加请求拦截器
    axios.interceptors.request.use(function (config) {
    Loading.service({
    lock: true,
    text: '加载中……',
    background: 'rgba(0, 0, 0, 0.7)'
  })
      return config;
     }, function (error) {
       console.log("req error")
        // 对请求错误做些什么
        return Promise.reject(error);
    });


 // 添加响应拦截器
axios.interceptors.response.use(function (response) {
   Loading.service().close();
    // 对响应数据做点什么
    return response;
  }, function (error) {
    Loading.service().close();
    // 对响应错误做点什么
    return Promise.reject(error);
  });

          axios({
            method: "post",
            url:"http://39.102.63.137/accOpen",
            headers: {
              /*         'Content-type': 'application/x-www-form-urlencoded', */
              "Content-Type": "application/json",
            },
            transformRequest: [
              function (data) {
                data = JSON.stringify(data);
                return data;
              },
            ],
            data: {
            /*   execOrganno:
                this.regForm.valueBranches[0] +
                this.regForm.valueBranches[1] +
                this.regForm.valueBranches[2],
              // execTellerno: this.regForm.ExecTellerno,
              execTellerno: 1,
              region: this.regForm.valueBranches[0],
              branchId: this.regForm.valueBranches[2], */
              name: this.regForm.name,
              password: this.regForm.password,
              phone: this.regForm.phone,
              checkcode:this.regForm.checkcode,
              sex:this.regForm.sex,
/*               number: this.regForm.id,
              payPassword: this.regForm.pay_password */
            },
          })
            .then((response) => {
              if (response.data.code == "0") {
                this.$alert("注册成功", {
                  confirmButtonText: "前往登录",
                  callback: (action) => {
                    this.$options.methods.back.bind(this)();
                  },
                });
              } else {
                this.$alert("注册失败", {
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
  },
};
</script>



<style>
</style>