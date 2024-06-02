cmake_minimum_required(VERSION 3.5)
project(SimpleWebServer)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(ROOT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(ROOT_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
# # 设置输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include_directories(${ROOT_INCLUDE_DIR})


# # 添加源文件
file(GLOB_RECURSE SRC_FILES ${ROOT_SRC_DIR}/*.cpp)
message("SRC_FILES: ${SRC_FILES}")
# # 设置include路径
add_executable(TestSql ${PROJECT_SOURCE_DIR}/test/test_mysql.cpp ${SRC_FILES} )
target_include_directories(TestSql PRIVATE ${ROOT_INCLUDE_DIR})

# 添加链接库
find_library(MYSQL_LIB mysqlclient HINTS)
if (MYSQL_LIB)
    message("MySQL library found: ${MYSQL_LIB}")
    target_link_libraries(TestSql PRIVATE ${MYSQL_LIB})
else()
    message(FATAL_ERROR "MySQL library not found")
endif()
# 查找 pthread 库
find_package(Threads REQUIRED)
target_link_libraries(TestSql PRIVATE Threads::Threads)

# 查找yaml库
find_package(yaml-cpp REQUIRED)
target_link_libraries(TestSql PRIVATE yaml-cpp)

# 编译main
# # 设置include路径
add_executable(main ${PROJECT_SOURCE_DIR}/main.cpp ${SRC_FILES} )
target_include_directories(main PRIVATE ${ROOT_INCLUDE_DIR})

# 添加链接库
target_link_libraries(main PRIVATE ${MYSQL_LIB})
# 链接 pthread 库
target_link_libraries(main PRIVATE Threads::Threads)
# 链接 yaml 库
target_link_libraries(main PRIVATE yaml-cpp)

