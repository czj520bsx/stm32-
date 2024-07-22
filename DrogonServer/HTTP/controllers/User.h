#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class User : public drogon::HttpController<User>
{
  public:
    METHOD_LIST_BEGIN
       METHOD_ADD(User::loginInfo, "login?userId={1}&passwd={2}",Post,Options);
       //url: http://8.130.146.112:8099/user/login?userId={1}&passwd={2}(登录api）
       METHOD_ADD(User::registerInfo, "register?userId={1}&passwd={2}&phone_number={3}", Post,Options);
       //url: http://8.130.146.112:8099/user/register?userId={1}&passwd={2}&phone_number={3}(注册api)
       METHOD_ADD(User::oneNetInfo,"oneNet?co={1}&co2={2}&ch4={3}&h2={4}&time={5}&flag={6}&info={7}",Post,Options);
       //url: http://8.130.146.112:8099/user/oneNet?co={1}&co2={2}&ch4={3}&h2={4}&time={5}&flag={6}&info={7}
       METHOD_ADD(User::deviceInfo,"device?device_id={1}",Post,Options);
       //url: http://8.130.146.112:8099/user/device?device_id={1}(注册设备api)
       METHOD_ADD(User::device_deleteInfo,"device_delete?device_id={1}",Post,Options);
       //url: http://8.130.146.112:8099/user/device_delete?device_id={1}(删除设备api)
       METHOD_ADD(User::getDeviceInfo,"DeviceInfo",Get,Options);
       //url: http://8.130.146.112:8099/user/DeviceInfo(设备详情api)
       METHOD_ADD(User::getCoInfo,"CoInfo",Get,Options);
       //url: http://8.130.146.112:8099/user/CoInfo(一氧化碳含量api)
       METHOD_ADD(User::getCo2Info,"Co2Info",Get,Options);
       //url: http://8.130.146.112:8099/user/Co2Info(二氧化碳含量api)
       METHOD_ADD(User::getCH4Info,"CH4Info",Get,Options);
       //url: http://8.130.146.112:8099/user/CH4Info(甲烷含量api)
       METHOD_ADD(User::getH2Info,"H2Info",Get,Options);
       //url: http://8.130.146.112:8099/user/H2Info(氢气含量api)
       METHOD_ADD(User::getERRORInfo,"errorInfo",Get,Options);
       //url: http://8.130.146.112:8099/user/errorInfo(警报api)

    METHOD_LIST_END
       //post函数实现虚函数
       void loginInfo(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback,
               std::string &&userId,
               const std::string &password);
       void registerInfo(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback,
               std::string &&userId,
               const std::string &password,
               const std::string &phone_number);
       void oneNetInfo(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback,
               std::string &&co,
               std::string &&co2,
               std::string &&ch4,
               std::string &&h2,
               std::string &&time,
               std::string &&flag,
               std::string &&info);
	void deviceInfo(const HttpRequestPtr &req,
                       std::function<void(const HttpResponsePtr &)> &&callback,
                       std::string &&device_id);
	void device_deleteInfo(const HttpRequestPtr &req,
                    std::function<void(const HttpResponsePtr &)> &&callback,
                    std::string &&device_id);
    //get函数实现虚函数
       void getCoInfo(const HttpRequestPtr &req,std::function<void(const HttpResponsePtr &)> &&callback);
       void getCo2Info(const HttpRequestPtr &req,std::function<void(const HttpResponsePtr &)> &&callback);
       void getCH4Info(const HttpRequestPtr &req,std::function<void(const HttpResponsePtr &)> &&callback);
       void getH2Info(const HttpRequestPtr &req,std::function<void(const HttpResponsePtr &)> &&callback);
       void getERRORInfo(const HttpRequestPtr &req,std::function<void(const HttpResponsePtr &)> &&callback);
	void getDeviceInfo(const HttpRequestPtr &req,std::function<void(const HttpResponsePtr &)> &&callback);
};
