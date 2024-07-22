#include <drogon/drogon.h>

using namespace drogon;

int main() {
    drogon::app().loadConfigFile("../config.json");
    drogon:: app().registerPostHandlingAdvice([](const drogon::HttpRequestPtr &, const drogon::HttpResponsePtr &resp) {
        resp->addHeader("Access-Control-Allow-Origin", "*");
    });
    drogon::app().run();
    return 0;
}
