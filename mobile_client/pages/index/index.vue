<template>
  <view>
    <!-- 添加标题 -->
    <view class="title">智能语音气体监测系统</view>

    <!-- 原有内容 -->
    <view class="wrap">
      <view class="dev-area">
        <view class="dev-cart">
          <view class="">
            <view class="dev-name">CH4</view>
            <image class="dev-logo" src="../../static/CH4.png" mode=""></image>
          </view>
          <view class="dev-data">{{CH4}} PPM</view>
        </view>
        <view class="dev-cart">
          <view class="">
            <view class="dev-name">H2</view>
            <image class="dev-logo" src="../../static/H2.png" mode=""></image>
          </view>
          <view class="dev-data">{{H2}} PPM</view>
        </view>
        <view class="dev-cart">
          <view class="">
            <view class="dev-name">CO</view>
            <image class="dev-logo" src="../../static/CO.png" mode=""></image>
          </view>
          <view class="dev-data">{{CO}} PPM</view>
        </view>
        <view class="dev-cart">
          <view class="">
            <view class="dev-name">CO2</view>
            <image class="dev-logo" src="../../static/CO2.png" mode=""></image>
          </view>
          <view class="dev-data">{{CO2}} PPM</view>
        </view>
        <view class="dev-cart">
          <view class="">
            <view class="dev-name">报警灯</view>
            <image class="dev-logo" src="../../static/Led.png" mode=""></image>
          </view>
          <switch :checked="led" @change="onLedSwitch" color="#8f8f94"/>
        </view>
        <view class="dev-cart">
          <view class="">
            <view class="dev-name">报警器</view>
            <image class="dev-logo" src="../../static/Beep.png" mode=""></image>
          </view>
          <switch :checked="BEEP" @change="onBeepSwitch" color="#8f8f94"/>
        </view>
		
		<view class="device-cart-l">
			<view>
				<view class="dev-name">甲烷阈值</view>
				
				<view class="dev-name">{{CH4_max}}ppm</view>
			</view>
			<view class="ctrl-slider">
				<!-- 滑动条调用slider组件，实时更新阈值 -->
				<slider :value="CH4_max" @change="sliderChange($event, 'slider1')" min="0" max="100" step="1"
					block-size="20" show-value />
			</view>
		</view>
		
		<view class="device-cart-l">
			<view>
				<view class="dev-name">氢气阈值</view>
				
				<view class="dev-name">{{H2_max}}ppm</view>
			</view>
			<view class="ctrl-slider">
				<!-- 滑动条调用slider组件，实时更新阈值 -->
				<slider :value="H2_max" @change="sliderChange($event, 'slider2')" min="0" max="100" step="1"
					block-size="20" show-value />
			</view>
		</view>
		
		<view class="device-cart-l">
			<view>
				<view class="dev-name">CO阈值</view>
				
				<view class="dev-name">{{CO_max}}ppm</view>
			</view>
			<view class="ctrl-slider">
				<!-- 滑动条调用slider组件，实时更新阈值 -->
				<slider :value="CO_max" @change="sliderChange($event, 'slider3')" min="0" max="100" step="1"
					block-size="20" show-value />
			</view>
		</view>
		
		<view class="device-cart-l">
			<view>
				<view class="dev-name">CO2阈值</view>
				
				<view class="dev-name">{{CO2_max}}ppm</view>
			</view>
			<view class="ctrl-slider">
				<!-- 滑动条调用slider组件，实时更新阈值 -->
				<slider :value="CO2_max" @change="sliderChange($event, 'slider4')" min="0" max="200" step="2"
					block-size="20" show-value />
			</view>
		</view>
		
		
		
		
      </view>
    </view>
  </view>
</template>

<script>
 // 引入字符串处理函数
 import {
 	stringify
 } from 'querystring';
 // 引入创建通用令牌函数
 const {
 	createCommonToken
 } = require('@/key.js')
 
 // 产品ID和设备名称要替换成自己的
 const product_id = 'CFcMIQYDRa';
 const device_name = 'D1';
  export default {
    data() {
      return {
        BEEP: true,
        CH4:'',
        CO:'',
        CO2:'',
        H2:'',
        led:true,
		CH4_max:'17',
		CO_max:'23',
		CO2_max:'45',
		H2_max:'25',
        token: '',
      }
    },
    onLoad() {
      const params = {
        author_key: 'BGjW9C6BkVrAyiyIK9gGihC/aiVZpACBsNKeW1dvXifY3jCbAqO5AvydhSum7X+3c86hRjvl/T/7OUpoSN1OOg==',
        version: '2022-05-01',
        user_id: '376681',
      }
      this.token = createCommonToken(params);
    },
    onShow() {
      this.fetchDevData();
       setInterval(()=>{
		this.fetchDevData();
      },3000)
    },
    methods: {
      fetchDevData(){
        uni.request({
          url: 'https://iot-api.heclouds.com/thingmodel/query-device-property', //仅为示例，并非真实接口地址。
          method: 'GET',
          data: {
            product_id: 'CFcMIQYDRa',
            device_name: 'D1'
          },
          header: {
            'authorization': this.token //自定义请求头信息
          },
          success: (res) => {
            console.log(res.data);
            this.BEEP=res.data.data[0].value==='true';
            this.CH4=res.data.data[1].value;
            this.CO=res.data.data[3].value;
            this.CO2=res.data.data[4].value;
            this.H2=res.data.data[7].value;
            //console.log(res.data.data[5].value);
            this.led=res.data.data[10].value==='true';
          }
        });
      },
      onLedSwitch(event) {
        console.log(event.detail.value);
        let value = event.detail.value;
        uni.request({
          url: 'https://iot-api.heclouds.com/thingmodel/set-device-property',
          method: 'POST',
          data: {
            product_id: 'CFcMIQYDRa',
            device_name: 'D1',
            params: { "led": value }
          },
          header: {
            'authorization': this.token
          },
          success: () => {
            console.log('LED ' + (value ? 'ON' : 'OFF') + ' !');
          }
        });
      },
      onBeepSwitch(event) {
        console.log(event.detail.value);
        let value = event.detail.value;
        uni.request({
          url: 'https://iot-api.heclouds.com/thingmodel/set-device-property',
          method: 'POST',
          data: {
            product_id: 'CFcMIQYDRa',
            device_name: 'D1',
            params: { "BEEP": value }
          },
          header: {
            'authorization': this.token
          },
          success: () => {
            console.log('BEEP ' + (value ? 'ON' : 'OFF') + ' !');
          }
        });
      },
	  // 滑动条变化事件的方法
	  sliderChange(e, id) {
	  	console.log(id)
	  	console.log('value 发生变化：' + e.detail.value) // 控制台打印信息，调试用
	  	let key_th = {}
	  	if (id == 'slider1') {
	  		this.CH4_max = e.detail.value
	  		// 更新温度阈值
	  		this.key_th = {
	  			CH4_max: this.CH4_max,
	  		};
	  	}else if (id == 'slider2') {
	  		this.H2_max = e.detail.value
	  		// 更新温度阈值
	  		this.key_th = {
	  			H2_max: this.H2_max,
	  		};
	  	} else if (id == 'slider3') {
	  		this.CO_max = e.detail.value
	  		// 更新温度阈值
	  		this.key_th = {
	  			CO_max: this.CO_max,
	  		};
	  	}else if (id == 'slider4') {
	  		this.CO2_max = e.detail.value
	  		// 更新温度阈值
	  		this.key_th = {
	  			CO2_max: this.CO2_max,
	  		};
	  	}  
	  	console.log('value 发生变化：>>>>>' + stringify(this.key_th) + '<<<<<<<')
	  	// 向后端发送设备属性更新请求
	  	uni.request({
	  		url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', // 仅为示例，并非真实接口地址。
	  		method: 'POST',
	  		data: {
	  			product_id: product_id,
	  			device_name: device_name,
	  			params: this.key_th
	  		},
	  		header: {
	  			'authorization': this.token // 自定义请求头信息
	  		},
	  		// 请求成功，打印提示信息，主要用于调试，可自定义
	  		success: () => {
	  			console.log(this.key_th);
				console.log(this.token);
	  		}
	  	});
	  }
	  
    }
  }
</script>

<style scoped>
  .title {
    font-size: 48rpx; /* 修改标题字体大小为48rpx */
    text-align: center;
    margin-bottom: 20rpx;
  }
  .wrap {
    padding: 40rpx;
  }
  .dev-area {
    display: flex;
    justify-content: space-between;
    flex-wrap: wrap;
  }
  .dev-cart {
    height: 140rpx;
    width: 320rpx;
    border-radius: 30rpx;
    margin-top: 30rpx;
    display: flex;
    justify-content: space-around;
    align-items: center;
    box-shadow: 0 0 15rpx #ccc;
  }
  .dev-name {
    font-size: 25rpx;
    text-align: center;
    color: #6d6d6d;
  }
  /* 长设备卡片样式 */
  .device-cart-l {
  	height: 150rpx;
  	/* 设置高度为150像素 */
  	width: 700rpx;
  	/* 设置宽度为700像素 */
  	border-radius: 30rpx;
  	/* 设置边框圆角为30像素 */
  	margin-top: 30rpx;
  	/* 设置上外边距为30像素 */
  	display: flex;
  	/* 使用弹性盒子布局 */
  	justify-content: space-around;
  	/* 在弹性容器中均匀分布子元素，两端对齐 */
  	align-items: center;
  	/* 在弹性容器中垂直居中对齐子元素 */
  	box-shadow: 0 0 15rpx #ccc;
  	/* 设置盒子阴影，颜色为灰色 */
  }
  /* 滑动条样式 */
  .ctrl-slider {
  	width: 550rpx;

  }
  .dev-logo {
    width: 80rpx;
    height: 80rpx;
    margin-top: 0rpx;
  }
  .dev-data {
    font-size: 40rpx;
    color: #6d6d6d;
  }
</style>