#include "User.h"
#include <string>
#include <random>

// Add definition of your processing function here
void User::loginInfo(const HttpRequestPtr &req,
                 std::function<void(const HttpResponsePtr &)> &&callback,
                 std::string &&userId,
                 const std::string &password) {
    HttpResponsePtr resp;
    auto clientPtr = drogon::app().getDbClient("default");
    auto f = clientPtr->execSqlAsyncFuture("SELECT * FROM public.user WHERE username = $1 AND password = $2", userId, password);
    try {
        auto r = f.get();
        if (r.size() > 0) {
            LOG_DEBUG << "User " << userId << "登陆成功";
            Json::Value ret;
            ret["result"] = "ok";
            ret["data"] = "登录成功";
            ret["token"] = drogon::utils::getUuid(); //生成token
            resp = HttpResponse::newHttpJsonResponse(ret);
        } else {
            LOG_DEBUG << "User " << userId << "尝试登录失败";
            Json::Value ret;
            ret["result"] = "ok";
            ret["data"] = "登录失败";
            resp = HttpResponse::newHttpJsonResponse(ret);
        }
        callback(resp);
    }catch (const std::exception& e) {
        LOG_ERROR << e.what();
    }
}

void User::registerInfo(const HttpRequestPtr &req,
                        std::function<void(const HttpResponsePtr &)> &&callback,
                        std::string &&userId,
                        const std::string &password,
			const std::string &phone_number) {
    HttpResponsePtr resp;
    auto clientPtr = drogon::app().getDbClient("default");
    auto f = clientPtr->execSqlAsyncFuture("SELECT * FROM public.user WHERE username = $1", userId);
    try {
        auto r = f.get();
        if (r.size() > 0) {
            LOG_DEBUG << "User " << userId << "已存在，无法注册";
            Json::Value ret;
            ret["result"] = "ok";
            ret["data"] = "用户名已存在";
            resp = HttpResponse::newHttpJsonResponse(ret);
        } else {
            clientPtr->execSqlAsyncFuture("INSERT INTO public.user(username,password,phone_number)VALUES ($1,$2,$3)", userId, password, phone_number);
            LOG_DEBUG << "User " << userId << "注册成功";
            Json::Value ret;
            ret["result"] = "ok";
            ret["data"] = "注册成功";
            resp = HttpResponse::newHttpJsonResponse(ret);
        }
        callback(resp);
    } catch (const std::exception &e) {
        LOG_ERROR << e.what();
    }
}

void User::oneNetInfo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                      std::string &&co,
                      std::string &&co2,
                      std::string &&ch4,
                      std::string &&h2,
                      std::string &&time,
                      std::string &&flag,
                      std::string &&info) {
    HttpResponsePtr resp;
    auto clientPtr = drogon::app().getDbClient("default");
    try{
        clientPtr->execSqlAsyncFuture("INSERT INTO public.sensor_data(co2_data,ch4_data,co_data,h2_data,time,flag,info)VALUES ($1,$2,$3,$4,$5,$6,$7)",co2,ch4,co,h2,time,flag,info);
        LOG_DEBUG << "数据传输成功";
        Json::Value ret;
        ret["result"] = "ok";
        ret["data"] = "数据传输成功";
        resp = HttpResponse::newHttpJsonResponse(ret);
        callback(resp);
    }catch(const std::exception &e){
        LOG_ERROR << e.what();
    }
}

void User::deviceInfo(const HttpRequestPtr &req,
                      std::function<void(const HttpResponsePtr &)> &&callback,
                      std::string &&device_id) {
    HttpResponsePtr resp;
    auto clientPtr = drogon::app().getDbClient("default");
    try{
	// 定义城市列表
        std::vector<std::string> cities = {"上海", "重庆", "成都", "深圳", "北京"};
        // 生成随机城市
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, cities.size() - 1);
        std::string site = cities[dis(gen)];

        // 生成随机的 live 值
        std::vector<std::string> liveOptions = {"在线", "离线"};
        std::uniform_int_distribution<> disLive(0, liveOptions.size() - 1);
        std::string live = liveOptions[disLive(gen)];


        clientPtr->execSqlAsyncFuture("INSERT INTO public.device(device_id,site,live)VALUES ($1,$2,$3)",device_id,site,live);
        LOG_DEBUG << "数据传输成功";
        Json::Value ret;
        ret["result"] = "ok";
        ret["data"] = "数据传输成功";
        resp = HttpResponse::newHttpJsonResponse(ret);
        callback(resp);
    }catch(const std::exception &e){
        LOG_ERROR << e.what();
    }
}

void User::device_deleteInfo(const HttpRequestPtr &req,
                      std::function<void(const HttpResponsePtr &)> &&callback,
                      std::string &&device_id)
{
    HttpResponsePtr resp;
    auto clientPtr = drogon::app().getDbClient("default");
    auto f = clientPtr->execSqlAsyncFuture("SELECT * FROM public.device WHERE device_id = $1", device_id);
    try {
        auto r = f.get();
        if (r.size() > 0) {
            clientPtr->execSqlAsyncFuture("DELETE FROM device WHERE device_id = $1",device_id );
            Json::Value ret;
            ret["result"] = "ok";
            ret["data"] = "已删除设备";
            resp = HttpResponse::newHttpJsonResponse(ret);
        } else {
            LOG_DEBUG << "device " << device_id  << "不存在";
            Json::Value ret;
            ret["result"] = "ok";
            ret["data"] = "设备不存在";
            resp = HttpResponse::newHttpJsonResponse(ret);
        }
        callback(resp);
    } catch (const std::exception &e) {
        LOG_ERROR << e.what();
}
}


void User::getCH4Info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value result;
    auto clientPtr = drogon::app().getDbClient("default");
    auto f = clientPtr->execSqlAsyncFuture("SELECT ch4_data,time FROM public.sensor_data order by id desc limit 5");
    try{
        auto r = f.get();
        if(r.size() > 0){
            for(const auto& row : r){
                Json::Value obj;
                obj["time"] = row["time"].as<std::string>().substr(11, 8);
                obj["ch4_data"] = row["ch4_data"].as<std::string>();
                result.append(obj);
            }
            std::reverse(result.begin(), result.end());
            auto resp = HttpResponse::newHttpJsonResponse(result);
            callback(resp);
        }
    }catch (const std::exception& e){
        LOG_ERROR << e.what();
    }
}

void User::getCoInfo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value result;
    auto clientPtr = drogon::app().getDbClient("default");
    auto f = clientPtr->execSqlAsyncFuture("SELECT co_data, time FROM public.sensor_data order by id desc limit 5");
    try{
        auto r = f.get();
        if(r.size() > 0){
            for(const auto& row : r){
                Json::Value obj;
                obj["time"] = row["time"].as<std::string>().substr(11, 8);
                obj["co_data"] = row["co_data"].as<std::string>();
                result.append(obj);
            }
            std::reverse(result.begin(), result.end());
            auto resp = HttpResponse::newHttpJsonResponse(result);
            callback(resp);
        }
    }catch (const std::exception& e){
        LOG_ERROR << e.what();
    }
}

void User::getCo2Info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value result;
    auto clientPtr = drogon::app().getDbClient("default");
    auto f = clientPtr->execSqlAsyncFuture("SELECT co2_data,time FROM public.sensor_data order by id desc limit 5");
    try{
        auto r = f.get();
        if(r.size() > 0){
            for(const auto& row : r){
                Json::Value obj;
                obj["time"] = row["time"].as<std::string>().substr(11, 8);
                obj["co2_data"] = row["co2_data"].as<std::string>();
                result.append(obj);
            }
            std::reverse(result.begin(), result.end());
            auto resp = HttpResponse::newHttpJsonResponse(result);
            callback(resp);
        }
    }catch (const std::exception& e){
        LOG_ERROR << e.what();
    }
}

void User::getH2Info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value result;
    auto clientPtr = drogon::app().getDbClient("default");
    auto f = clientPtr->execSqlAsyncFuture("SELECT h2_data,time FROM public.sensor_data order by id desc limit 5");
    try{
        auto r = f.get();
        if(r.size() > 0){
            for(const auto& row : r){
                Json::Value obj;
                obj["time"] = row["time"].as<std::string>().substr(11, 8);
                obj["h2_data"] = row["h2_data"].as<std::string>();
                result.append(obj);
            }
            std::reverse(result.begin(), result.end());
            auto resp = HttpResponse::newHttpJsonResponse(result);
            callback(resp);
        }
    }catch (const std::exception& e){
        LOG_ERROR << e.what();
    }
}

void User::getERRORInfo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value result;
    auto clientPtr = drogon::app().getDbClient("default");
    auto f = clientPtr->execSqlAsyncFuture("SELECT info,time FROM public.sensor_data WHERE flag = 'true' ORDER BY time asc");
    try{
        auto r = f.get();
        if(r.size() > 0){
            for(const auto& row : r){
                Json::Value obj;
                obj["time"] = row["time"].as<std::string>();
                obj["info"] = row["info"].as<std::string>();
                result.append(obj);
            }
            std::reverse(result.begin(), result.end());
            auto resp = HttpResponse::newHttpJsonResponse(result);
            callback(resp);
        }
    }catch(const std::exception& e){
        LOG_ERROR << e.what();
    }
}

void User::getDeviceInfo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    Json::Value result;
    auto clientPtr = drogon::app().getDbClient("default");
    auto f = clientPtr->execSqlAsyncFuture("SELECT device_id,site,live FROM public.device ORDER BY id desc ");
    try{
        auto r = f.get();
        if(r.size() > 0){
            for(const auto& row : r){
                Json::Value obj;
                obj["device_id"] = row["device_id"].as<std::string>();
                obj["site"] = row["site"].as<std::string>();
                obj["live"] = row["live"].as<std::string>();
                result.append(obj);
            }
            std::reverse(result.begin(), result.end());
            auto resp = HttpResponse::newHttpJsonResponse(result);
            callback(resp);
        }
    }catch(const std::exception& e){
        LOG_ERROR << e.what();
    }
}


