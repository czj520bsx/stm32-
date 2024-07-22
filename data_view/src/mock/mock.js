import Mock from 'mockjs'
//延时200-600毫秒请求到数据
Mock.setup({
    timeout: '200-600'
})

const Random = Mock.Random;
//用户总览
function countUserNum() {
    const a = Mock.mock({
        success: true,
        data: {
            offlineNum:'@integer(50, 300)',
            lockNum: '@integer(50, 100)',
            totalNum:513
        }
    })
    a.data.onlineNum=a.data.totalNum-a.data.offlineNum-a.data.lockNum
    return a
}
// 接口，第一个参数url，第二个参数请求类型，第三个参数响应回调
Mock.mock(new RegExp('countUserNum'), 'get', countUserNum)

// /设备总览
function countDeviceNum() {
    const a = Mock.mock({
        success: true,
        data: {
            alarmNum: '@integer(100, 1000)',
            offlineNum: '@integer(0, 50)',
            totalNum:698
        }
    })
    a.data.onlineNum=a.data.totalNum-a.data.offlineNum
    return a
}
Mock.mock(new RegExp('countDeviceNum'), 'get', countDeviceNum)


// /设备提醒
function sbtx() {
    const a = Mock.mock({
        success: true,
        data: {
            "list|20": [
                {
                    provinceName: "@province()",
                    cityName: '@city()',
                    countyName: "@county()",
                    createTime: "@datetime('2024-05-dd HH:mm:ss')",
                    "gatewayno|+1": 10000,
                    "onlineState|1": [0, 1],
                }
            ]
        }
    })
    return a
}
Mock.mock(new RegExp('sbtx'), 'get', sbtx)



//中间地图
function centermap(options) {
    let params = parameteUrl(options.url)
    if (params.regionCode && params.regionCode != 'china') {
        const a = Mock.mock({
            success: true,
            data: {
                "dataList|30": [
                    {
                        name: "@city()",
                        value: '@integer(1, 1000)'
                    }
                ],
                regionCode: params.regionCode,//-代表中国
            }
        })
        return a
    } else {
        const a = Mock.mock({
            success: true,
            data: {
                "dataList|8": [
                    {
                        name: "@province()",
                        value: '@integer(1, 1000)'
                    }
                ],
                regionCode: 'china',
            }
        })
        return a
    }
}
Mock.mock(new RegExp('centermap'), 'get', centermap)

let lastStartMonth = 1; // 记录上一次生成的起始月份
// 报警次数
function alarmNum() {
// 当前生成的起始月份为上一次生成的起始月份加1
    const startMonth = lastStartMonth === 12 ? 1 : lastStartMonth + 1;
    lastStartMonth = startMonth; // 更新上一次生成的起始月份

    const months = [];
    // 生成五个月份
    for (let month = startMonth; month < startMonth + 5; month++) {
        const adjustedMonth = (month % 12) + (month % 12 === 0 ? 12 : 0); // 处理月份超过12的情况
        months.push(`${adjustedMonth < 10 ? '0' : ''}${adjustedMonth}`);
    }
    const a = Mock.mock({
        success: true,
        data: {
            dateList: months.map(month => `@datetime('2024-${month}')`),
            "numList|5":[
                '@integer(0, 1000)'
            ],
            "numList2|5":[
                '@integer(0, 1000)'
            ],
            "numList3|5":[
                '@integer(0, 1000)'
            ],
            "numList4|5":[
                '@integer(0, 1000)'
            ]
        }
    })
    return a
}
Mock.mock(new RegExp('alarmNum'), 'get', alarmNum)


// 数据统计图
function ssyj() {
    const leak_types = ['二氧化碳泄漏', '氢气泄漏', '一氧化碳泄漏', '甲烷泄漏'];
    const a = Mock.mock({
        success: true,
        data: {
            "list|40":[{
                alertdetail: () => {
                    return leak_types[Math.floor(Math.random() * leak_types.length)];
                },
                alertvalue: "@float(60, 200)",
                createtime: "@datetime('2024-05-dd HH:mm:ss')",
                "gatewayno|+1": 10000,
                provinceName: "@province()",
                cityName: '@city()',
                countyName: "@county()",
            }],
        }
    })
    return a
}

Mock.mock(new RegExp('ssyj'), 'get', ssyj)


//安装计划 
function installationPlan() {
    let num=  RandomNumBoth(26,32);
    const a = Mock.mock({
        ["category|"+num]:["@city()"],
        ["barData|"+num]:["@integer(10, 100)"],
    })
    let lineData=[],rateData=[];
    for (let index = 0; index < num; index++) {
        let lineNum = Mock.mock('@integer(0, 100)')+a.barData[index]
        lineData.push(lineNum)
        let rate = a.barData[index] / lineNum;
        rateData.push((rate*100).toFixed(0))
    }
    a.lineData=lineData
    a.rateData=rateData
    return {
        success: true,
        data:a
    }
}
Mock.mock(new RegExp('installationPlan'), 'get', installationPlan)


//报警排名 
function ranking() {
   //多生成几个避免重复 重复会报错
  let num =Mock.mock({"list|48":[{ value:"@integer(50,1000)",name:"@city()"}]}).list
//   console.log(num);
  let newNum =[],numObj={}
  num.map(item=>{
    if(!numObj[item.name] && newNum.length<8){
        numObj[item.name] =true
        newNum.push(item)
    }
  })
  let arr = newNum.sort((a,b)=>{
    return b.value-a.value
  })
  let a ={
      success:true,
      data:arr
  }
  return a
}
Mock.mock(new RegExp('ranking'), 'get', ranking)
/**
 * @description: min ≤ r ≤ max  随机数
 * @param {*} Min
 * @param {*} Max
 * @return {*}
 */
function RandomNumBoth(Min,Max){
    var Range = Max - Min;
    var Rand = Math.random();
    var num = Min + Math.round(Rand * Range); //四舍五入
    return num;
}
/**
 * @description: 获取路径参数
 * @param {*} url
 * @return {*}
 */
function parameteUrl(url) {
    var json = {}
    if (/\?/.test(url)) {
        var urlString = url.substring(url.indexOf("?") + 1);
        var urlArray = urlString.split("&");
        for (var i = 0; i < urlArray.length; i++) {
            var urlItem = urlArray[i];
            var item = urlItem.split("=");
            console.log(item);
            json[item[0]] = item[1];
        }
        return json;
    }
    return {};
}
