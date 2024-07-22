<template>
  <div class = "common">
    <div class="container">
      <div class="form-box" :style="{ transform: formBoxTransform }">
        <!-- 注册 -->
        <div class="register-box" :class="{ hidden: !isRegister }">
          <h1>register</h1>
          <input v-model="registerUser" type="text" placeholder="用户名">
          <input v-model="registerNumber" type="email" placeholder="电话号">
          <input v-model="registerPwd" type="password" placeholder="密码">
          <input v-model="registerPwd_2" type="password" placeholder="确认密码">
          <button @click="registerButton">注册</button>
        </div>
        <!-- 登录 -->
        <div class="login-box" :class="{ hidden: isRegister }">
          <h1>login</h1>
          <input v-model="username" type="text" placeholder="用户名">
          <input v-model="password" type="password" placeholder="密码">
          <button @click="LoginButton">登录</button>
        </div>
      </div>
      <div class="con-box left">
        <h2>欢迎使用<span>弦上乐</span></h2>
        <p>快来查看你的专属<span>仪器</span>吧</p>
        <img src="../assets/register.png" alt="">
        <p>没有账号？</p>
        <button @click="toggleForm(true)">去注册</button>
      </div>
      <div class="con-box right">
        <h2>欢迎使用<span>弦上乐</span></h2>
        <p>快来查看你的专属<span>仪器</span>吧</p>
        <img src="../assets/login.png" alt="">
        <p>已有账号</p>
        <button @click="toggleForm(false)">去登录</button>
      </div>
    </div>
  </div>

</template>

<script>
import {loginAPI, registerAPI} from "@/plugins";
export default {
  data() {
    return {
      username:'',
      password:'',
      registerUser:'',
      registerNumber:'',
      registerPwd:'',
      registerPwd_2:'',
      isRegister: false,
      formBoxTransform: 'translateX(95%)'
    };
  },
  methods: {
    toggleForm(isRegister) {
      this.isRegister = isRegister;
      this.formBoxTransform = isRegister ? 'translateX(10%)' : 'translateX(95%)';
    },

    LoginButton() {
      loginAPI(this.username, this.password).then((res) => {
        if (res.data.data === '登录成功') {
          this.$router.push('/home/index');
          return this.$message({
            type:"success",
            message:"登录成功",
          })
        } else{
          return this.$message({
            type:"error",
            message:"用户名或密码错误",
          })
        }
      });
    },

    registerButton(){
      if(this.registerPwd !== this.registerPwd_2){
        return this.$message({
          type:"error",
          message:"密码不一致，无法注册",
        })
      }else{
        registerAPI(this.registerUser,this.registerPwd,this.registerNumber).then((res) => {
          if (res.data.data === '注册成功') {
            return this.$message({
              type:"success",
              message:"注册成功",
            })
          } else{
            return this.$message({
              type:"error",
              message:"用户名已存在，注册失败",
            })
          }
        });
      }
    }
  }
};
</script>

<style scoped>

.common {
  /* 100%窗口高度 */
  height: 100vh;
  /* 渐变背景 */
  background: linear-gradient(200deg,#f3e7e9,#e3eeff);
}

.container {
  background-color: #fff;
  width: 650px;
  height: 415px;
  border-radius: 5px;
  /* 阴影 */
  box-shadow: 5px 5px 5px rgba(0,0,0,0.1);
  /* 相对定位 */
  position: absolute;
  /* 垂直和水平居中 */
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
}

.form-box {
  /* 绝对定位 */
  position: absolute;
  top: -10%;
  transform: translate(-50%, -50%);
  background-color: #d3b7d8;
  width: 320px;
  height: 500px;
  border-radius: 5px;
  box-shadow: 2px 0 10px rgba(0, 0, 0, 0.1);
  display: flex;
  justify-content: center;
  align-items: center;
  z-index: 2;
  /* 动画过渡 加速后减速 */
  transition: 0.5s ease-in-out;
}

.register-box,.login-box{
  /* 弹性布局 垂直排列 */
  display: flex;
  flex-direction: column;
  align-items: center;
  width: 100%;
}

.hidden{
  display: none;
  transition: 0.5s;
}

h1{
  text-align: center;
  margin-bottom: 25px;
  /* 大写 */
  text-transform: uppercase;
  color: #fff;
  /* 字间距 */
  letter-spacing: 5px;
}

input{
  background-color: transparent;
  width: 70%;
  color: #fff;
  border: none;
  /* 下边框样式 */
  border-bottom: 1px solid rgba(255,255,255,0.4);
  padding: 10px 0;
  text-indent: 10px;
  margin: 8px 0;
  font-size: 14px;
  letter-spacing: 2px;
}

input::placeholder{
  color: #fff;
}

input:focus{
  color: #a262ad;
  outline: none;
  border-bottom: 1px solid #a262ad80;
  transition: 0.5s;
}

input:focus::placeholder{
  opacity: 0;
}

.form-box button{
  width: 70%;
  margin-top: 35px;
  background-color: #f6f6f6;
  outline: none;
  border-radius: 8px;
  padding: 13px;
  color: #a262ad;
  letter-spacing: 2px;
  border: none;
  cursor: pointer;
}
.form-box button:hover{
  background-color: #a262ad;
  color: #f6f6f6;
  transition: background-color 0.5s ease;
}
.con-box{
  width: 50%;
  /* 弹性布局 垂直排列 居中 */
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  /* 绝对定位 居中 */
  position: absolute;
  top: 50%;
  transform: translateY(-50%);
}
.con-box.left{
  left: -2%;
}
.con-box.right{
  right: -2%;
}
.con-box h2{
  color: #8e9aaf;
  font-size: 25px;
  font-weight: bold;
  letter-spacing: 3px;
  text-align: center;
  margin-bottom: 4px;
}
.con-box p{
  font-size: 12px;
  letter-spacing: 2px;
  color: #8e9aaf;
  text-align: center;
}
.con-box span{
  color: #d3b7d8;
}
.con-box img{
  width: 150px;
  height: 150px;
  opacity: 0.9;
  margin: 40px 0;
}
.con-box button{
  margin-top: 3%;
  background-color: #fff;
  color: #a262ad;
  border: 1px solid #d3b7d8;
  padding: 6px 10px;
  border-radius: 5px;
  letter-spacing: 1px;
  outline: none;
  cursor: pointer;
}
.con-box button:hover{
  background-color: #d3b7d8;
  color: #fff;
}
</style>
