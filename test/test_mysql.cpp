#include "config.h"
using namespace std;

connection_pool *connPool = connection_pool::GetInstance();

//初始化数据库连接池
void initSqlresultConnPool(YAML::Node node)
{
    connPool->init(node["url"].as<string>(), \
                node["username"].as<string>(), \
                node["password"].as<string>(), \
                node["database"].as<string>(), \
                node["port"].as<int>(), \
                node["MaxConn"].as<int>(), \
                node["close_log"].as<int>() );
}

int main()
{
    int m_close_log = 0;
    YAML::Node config = YAML::LoadFile("config.yaml");
    m_close_log = config["WebServer"]["close_log"].as<int>();
    Log::get_instance()->init("log/ServerLog", m_close_log);
    try {
        YAML::Node sql_config = config["Mysql"];
        initSqlresultConnPool(sql_config);
    }catch (const YAML::TypedBadConversion<int>& e) {
        std::cerr << "YAML error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    LOG_INFO("initSqlresultConnPool success");
    MYSQL *sql = connPool->GetConnection();
    string sql_query = "select * from tb_user";
    int res = mysql_query(sql, sql_query.c_str());
    if (!res)
    {
        MYSQL_RES *result = mysql_store_result(sql);
        int num_fields = mysql_num_fields(result);
        MYSQL_FIELD *fields = mysql_fetch_fields(result);
        for (int i = 0; i < num_fields; i++)
        {
            cout << fields[i].name << "\t";
        }
        cout << endl;
        while(MYSQL_ROW row = mysql_fetch_row(result))
        {
            for(int i = 0; i < num_fields; i++)
            {
                cout << row[i] << "\t";
            }
            cout << endl;
        }
    } 
    connPool->ReleaseConnection(sql);
    return 0;
}