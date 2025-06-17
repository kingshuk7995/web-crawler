#include <drogon/drogon.h>

int main() {
    //Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", 5555);
    //Load yaml config file
    drogon::app().loadConfigFile("../config.json");
    drogon::app().run();
    return 0;
}
