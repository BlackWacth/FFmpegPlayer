cmake_minimum_required(VERSION 3.4.1)


# 创建一个变量 source_file 它的值就是src/main/cpp/ 所有的.cpp文件
file(GLOB source_file src/main/cpp/*.cpp)

add_library(
             native-lib

             SHARED

             ${source_file} )

include_directories(libs/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}")

target_link_libraries(
                        native-lib
                        avformat
                        avcodec
                        avfilter
                        avutil
                        swresample
                        swscale
                        log
                        z
                        android
                       )