<template>
  <div class="container">
    <h2>弦上乐智能气体泄漏系统</h2>
    <p>弦上乐智能气体泄漏系统是一款高效、可靠的气体泄漏检测系统，旨在保障您的家庭和工作场所的安全。</p>
    <p>主要功能包括：</p>
    <ul>
      <li><span class="icon">🔍</span>系统可以实时监测环境中的二氧化碳、一氧化碳、氢气和甲烷等气体浓度，及时发现气体泄漏情况</li>
      <li><span class="icon">🔊</span>用户可以通过语音指令对系统进行控制，例如启动检测、关闭报警等，并且系统能够通过语音播报警告信息，提高用户的感知能力</li>
      <li><span class="icon">📡</span>通过WiFi模块，系统可以将检测到的气体浓度数据实时传输到远程服务器或者用户手机端，使用户能够远程监控环境情况</li>
      <li><span class="icon">🚨</span>当系统检测到环境中存在可燃气体或有害气体浓度超过安全阈值时，会触发蜂鸣器进行预警，并同时将预警信息发送给用户，提醒用户及时采取措施</li>
      <li><span class="icon">🖥️</span>通过Vue框架设计的前端界面，用户可以直观地查看气体浓度曲线图、设备状态及警报信息，操作简单便捷</li>
    </ul>
    <p>产品优势：</p>
    <ul>
      <li><span class="icon">🛡️</span>系统集成了多种气体传感器，能够同时监测多种有害气体，提供全面的环境安全保障</li>
      <li><span class="icon">📶</span>通过WiFi模块实现数据传输，用户可以随时随地通过手机或者远程控制端监控环境状况，提高了监控的便捷性和实用性</li>
      <li><span class="icon">⚠️</span>当系统检测到危险气体浓度超标时，能够及时发出预警信号和语音提醒，帮助用户及时采取措施避免事故发生</li>
      <li><span class="icon">🎙️</span>通过语音控制功能，用户可以更加便捷地操作系统，提高了用户体验和系统的智能化程度</li>
      <li><span class="icon">💼</span>该系统不仅具备气体检测功能，还包括语音控制、远程监控等多种功能，为用户提供了全方位的环境安全保障，符合现代智能化需求</li>
    </ul>
    <p>弦上乐智能气体泄漏系统，让您的生活更安心、更放心！</p>
  </div>
</template>

<script>
export default {
}
</script>

<style>
body {
  font-family: Arial, sans-serif;
  background-color: #f0f0f0;
}

.container {
  max-width: 1600px;
  height: 580px;
  margin: 0 auto;
  padding: 20px;
  background-color: #fff;
  border-radius: 10px;
  box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
}

h2 {
  color: #333;
  text-align: center;
  font-size: 24px;
  margin-bottom: 20px;
}

p {
  color: #666;
  line-height: 1.6;
  font-size: 16px;
  margin-bottom: 15px;
}
ul {
  border-top: 1px solid #ccc;
  border-bottom: 1px solid #ccc;
  padding: 10px 0;
  list-style-type: none;

}
li {
  position: relative;
  padding-left: 20px;
  font-size: 16px;
  margin-bottom: 8px;
}
li::before {
  content: "\2022"; /* 使用实心圆点作为列表标记 */
  position: absolute;
  left: 0;
  color: #666;
}


</style>
