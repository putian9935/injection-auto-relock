cmake_minimum_required(VERSION 3.10)

project(injection_relock)

# our library 
add_library(inj-relck src/read.c src/write.c src/peak_detect.c src/current_lock.c src/length_lock.c)

# common libraries
find_library(RP_LIB rp /opt/redpitaya/lib)
find_library(CURL_LIB curl)
include_directories( /opt/redpitaya/include )

# common compiler flags 
add_definitions(-DZ10)

# un-comment the following if send slack is needed 
# add_compile_definitions(USE_SLACK)
# add_compile_definitions(SLACK_WEBHOOK_URL="https://hooks.slack.com/services/...")
# add_library(send-slack src/send_slack.c)

# TARGET1: lock 
add_executable(lock src/main.c)
target_compile_options(lock PRIVATE -Wall -Wpedantic -Wextra) 
set_property(TARGET lock PROPERTY C_STANDARD 11)

target_link_libraries( lock inj-relck ${RP_LIB} ${CURL_LIB} )
# un-comment the following and comment the previous line if send slack is needed 
# target_link_libraries( lock inj-relck send-slack ${RP_LIB} ${CURL_LIB} )

# TARGET2: sweep
add_executable(sweep src/sweep.c)
target_compile_options(sweep PRIVATE -Wall -Wpedantic -Wextra) 
set_property(TARGET sweep PROPERTY C_STANDARD 11)
target_link_libraries( sweep inj-relck ${RP_LIB} )
