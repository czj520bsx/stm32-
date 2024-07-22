import service from "@/plugins/axios";
export const loginAPI = (userId, password) => {
    const url = `http://8.130.146.112:8099/user/login?userId=${userId}&passwd=${password}`;
    // 发起网络请求
    return service({
        url: url,
        method: 'POST'
    });
}
export const registerAPI = (userId, password,phoneNumber) => {
    const url =`http://8.130.146.112:8099/user/register?userId=${userId}&passwd=${password}&phone_number=${phoneNumber}`;
    // 发起网络请求
    return service({
        url: url,
        method: 'POST'
    });
}
export const CO2ValueAPI = () => {
    const url =`http://8.130.146.112:8099/user/Co2Info`;
    return service({
        url: url,
        method: 'GET'
    });
}
export const COValueAPI = () => {
    const url =`http://8.130.146.112:8099/user/CoInfo`;
    // 发起网络请求
    return service({
        url: url,
        method: 'GET'
    });
}
export const CH4ValueAPI = () => {
    const url =`http://8.130.146.112:8099/user/CH4Info`;
    return service({
        url: url,
        method: 'GET'
    });
}
export const H2ValueAPI = () => {
    const url =`http://8.130.146.112:8099/user/H2Info`;
    return service({
        url: url,
        method: 'GET'
    });
}

export const ErrorInfoAPI = () => {
    const url =`http://8.130.146.112:8099/user/ERRORInfo`;
    return service({
        url: url,
        method: 'GET'
    });
}


export const changeValueAPI = (CH4,H2,CO,CO2) => {
    let key_th = {
        CH4_max: parseInt(CH4),
        H2_max: parseInt(H2),
        CO_max: parseInt(CO),
        CO2_max: parseInt(CO2),
    };
    const url =`https://iot-api.heclouds.com/thingmodel/set-device-property`;
    return service({
        url: url,
        method: 'POST',
        data: {
            product_id: 'CFcMIQYDRa',
            device_name: 'D1',
            params: key_th
        },
        headers: {
            'authorization': 'version=2022-05-01&res=userid%2F376681&et=1744899837&method=sha1&sign=rLGWwiRM8saWyc%2F%2FnjBtjsd2SUg%3D' //自定义请求头信息
        }
    }).then(response => {
        console.log('请求成功，响应数据为：', response.data);
        return response; // 返回响应数据
    }).catch(error => {
        console.error('请求失败，错误信息为：', error);
        throw error; // 抛出错误
    });
}

export const changeSwitchAPI = (CH4,H2,CO,CO2) => {
    let key_th = {
        CH4_bool: CH4,
        H2_bool: H2,
        CO_bool: CO,
        CO2_bool: CO2,
    };
    const url =`https://iot-api.heclouds.com/thingmodel/set-device-property`;
    return service({
        url: url,
        method: 'POST',
        data: {
            product_id: 'CFcMIQYDRa',
            device_name: 'D1',
            params: key_th
        },
        headers: {
            'authorization': 'BGjW9C6BkVrAyiyIK9gGihC/aiVZpACBsNKeW1dvXifY3jCbAqO5AvydhSum7X+3c86hRjvl/T/7OUpoSN1OOg==' //自定义请求头信息
        }
    }).then(response => {
        console.log('请求成功，响应数据为：', response.data);
        return response; // 返回响应数据
    }).catch(error => {
        console.error('请求失败，错误信息为：', error);
        throw error; // 抛出错误
    });
}

export const DeviceInfoAPI = () => {
    const url =`http://8.130.146.112:8099/user/DeviceInfo`;
    return service({
        url: url,
        method: 'GET'
    });
}

export const DeviceDelete = (ID) => {
    const url =`http://8.130.146.112:8099/user/device_delete?device_id=${ID}`;
    return service({
        url: url,
        method: 'POST'
    });
}

export const DeviceLogin = (ID) => {
    const url =`http://8.130.146.112:8099/user/device?device_id=${ID}`;
    return service({
        url: url,
        method: 'POST'
    });
}
