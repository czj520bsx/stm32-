import axios from "axios";

const service = axios.create({
  baseURL:'api',
  timeout:50000
})

//请求拦截器
service.interceptors.request.use(
    function (config){
      //在发送前之前做什么，例如加上token
      return config
    },

    function (error){
      //对请求错误做什么
      return Promise.reject(error)
    }
)

//响应拦截器
service.interceptors.response.use(
    function (response){
      //在接收响应时应该做什么，如token失效跳转登录
      return response
    },

    function (error){
      return error
    }
)

export default service
