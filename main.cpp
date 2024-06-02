#include "config.h"

int main(int argc, char *argv[])
{
    
    YAML::Node config_yaml = YAML::LoadFile("config.yaml");

    //需要修改的数据库信息,登录名,密码,库名
    string url = config_yaml["Mysql"]["url"].as<string>();
    string user =  config_yaml["Mysql"]["username"].as<string>();
    string passwd =  config_yaml["Mysql"]["password"].as<string>();
    string databasename = config_yaml["Mysql"]["database"].as<string>();

    //命令行解析
    Config config;
    config.parse_conf(config_yaml);
    config.parse_arg(argc, argv);
    int m_close_log = config.close_log;
    WebServer server;
    //初始化
    server.init(url,config.PORT, user, passwd, databasename, config.LOGWrite, 
                config.OPT_LINGER, config.TRIGMode,  config.sql_num,  config.thread_num, 
                config.close_log, config.actor_model);
    //日志
    server.log_write();
    LOG_INFO("%s:%d server log success",__FILE__,__LINE__);
    //数据库
    server.sql_pool();

    LOG_INFO("%s:%d server sql success",__FILE__,__LINE__);

    //线程池
    server.thread_pool();

    //触发模式
    server.trig_mode();

    //监听
    server.eventListen();

    //运行
    server.eventLoop();

    return 0;
}