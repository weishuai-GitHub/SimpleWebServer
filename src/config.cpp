#include "config.h"

Config::Config(){
    //端口号,默认9006
    PORT = 9006;

    //日志写入方式，默认同步
    LOGWrite = 0;

    //触发组合模式,默认listenfd LT + connfd LT
    TRIGMode = 0;

    //listenfd触发模式，默认LT
    LISTENTrigmode = 0;

    //connfd触发模式，默认LT
    CONNTrigmode = 0;

    //优雅关闭链接，默认不使用
    OPT_LINGER = 0;

    //数据库连接池数量,默认8
    sql_num = 8;

    //线程池内的线程数量,默认8
    thread_num = 8;

    //关闭日志,默认不关闭
    close_log = 0;

    //并发模型,默认是proactor
    actor_model = 0;
}

void Config::parse_arg(int argc, char*argv[]){
    int opt;
    const char *str = "p:l:m:o:s:t:c:a:";
    while ((opt = getopt(argc, argv, str)) != -1)
    {
        switch (opt)
        {
        case 'p':
        {
            PORT = atoi(optarg);
            break;
        }
        case 'l':
        {
            LOGWrite = atoi(optarg);
            break;
        }
        case 'm':
        {
            TRIGMode = atoi(optarg);
            break;
        }
        case 'o':
        {
            OPT_LINGER = atoi(optarg);
            break;
        }
        case 's':
        {
            sql_num = atoi(optarg);
            break;
        }
        case 't':
        {
            thread_num = atoi(optarg);
            break;
        }
        case 'c':
        {
            close_log = atoi(optarg);
            break;
        }
        case 'a':
        {
            actor_model = atoi(optarg);
            break;
        }
        default:
            break;
        }
    }
}


void Config::parse_conf(YAML::Node config){
    PORT = config["WebServer"]["PORT"].as<int>();
    LOGWrite = config["WebServer"]["LOGWrite"].as<int>();
    TRIGMode = config["WebServer"]["TRIGMode"].as<int>();
    LISTENTrigmode = config["WebServer"]["LISTENTrigmode"].as<int>();
    CONNTrigmode = config["WebServer"]["CONNTrigmode"].as<int>();
    OPT_LINGER = config["WebServer"]["OPT_LINGER"].as<int>();
    sql_num = config["WebServer"]["sql_num"].as<int>();
    thread_num = config["WebServer"]["thread_num"].as<int>();
    close_log = config["WebServer"]["close_log"].as<int>();
    actor_model = config["WebServer"]["actor_model"].as<int>();
}