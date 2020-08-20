cmake_minimum_required(VERSION 2.8.4)
project("BaseLib")
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC ")
set(CMAKE_C_FLAGS "-std=c++11 -fPIC ")

find_package(Threads)

#设置包含的目录
include_directories(
	.
    common
    jsoncpp
    sqlite3
	tinyxml
)

#file(GLOB SRC_COMMON_LIST common/ace.cpp common/AnalyIPRange.cpp common/Base64.cpp common/CircleAction.cpp common/CodeConver.cpp common/DbgPrint.cpp common/FileVersion.cpp common/function.cpp common/inifile.cpp common/iprange.cpp common/ipconfig.cpp common/MacroDefine.cpp common/MD5.cpp common/ProcessMan.cpp common/os.cpp common/RC4-LIB.cpp common/socket.cpp common/stdfile.cpp common/StringDetermine.cpp common/StringMatch.cpp common/task.cpp common/TimeFun.cpp common/tolists.cpp common/XmlReader.cpp common/RunSqlCommand.cpp common/RunSqlConnect.cpp common/RunSqlRead.cpp common/RunSqlRead.cpp common/RunSqlTransaction.cpp common/GetHardInfo.cpp common/user.cpp)
file(GLOB SRC_COMMON_LIST common/ace.cpp common/Base64.cpp common/os.cpp common/function.cpp common/inifile.cpp common/tolists.cpp common/XmlReader.cpp common/CodeConver.cpp common/DbgPrint.cpp common/stdfile.cpp common/RunSqlCommand.cpp common/RunSqlConnect.cpp common/RunSqlRead.cpp common/RunSqlRead.cpp common/RunSqlTransaction.cpp common/task.cpp )
IF (WIN32)
file(GLOB SRC_COMMON_LIST common/funcpoint.cpp ${SRC_COMMON_LIST})
ENDIF ()

aux_source_directory(jsoncpp SRC_JSONCPP_LIST)
aux_source_directory(sqlite3 SRC_SQLITE3_LIST)
aux_source_directory(tinyxml SRC_TINYXML_LIST)

SET(LIBBASE_SRC ${SRC_COMMON_LIST} ${SRC_JSONCPP_LIST} ${SRC_SQLITE3_LIST} ${SRC_TINYXML_LIST})

add_library(BaseLib STATIC ${LIBBASE_SRC}) 	# 生成静态库
#add_library(BaseLib SHARED ${LIBBASE_SRC}) 	# 生成动态库或共享库
target_link_libraries(BaseLib ${CMAKE_THREAD_LIBS_INIT})

#SET_TARGET_PROPERTIES (BaseLib_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
#SET_TARGET_PROPERTIES (BaseLib PROPERTIES CLEAN_DIRECT_OUTPUT 1)

#SET_TARGET_PROPERTIES (BaseLib_static PROPERTIES OUTPUT_NAME "BaseLib")
#SET_TARGET_PROPERTIES (BaseLib PROPERTIES VERSION 1.2 SOVERSION 1) 添加版本号，链接

#hrRobotPub
aux_source_directory(hrRobotPub SRC_hrRobotPub_LIST)
add_library(hrRobotPub STATIC ${SRC_hrRobotPub_LIST})
target_link_libraries(hrRobotPub BaseLib)
target_link_libraries(hrRobotPub ${CMAKE_THREAD_LIBS_INIT})

#安装文件
#install(TARGETS BaseLib BaseLib_static 
        #RUNTIME DESTINATION bin	
install(TARGETS BaseLib hrRobotPub 
#        LIBRARY DESTINATION BaseLib/lib
        ARCHIVE DESTINATION BaseLib/lib)

file(GLOB COMMON_HEAD_LIST  "common/*.h")
file(GLOB TINYXML_HEAD_LIST  "tinyxml/*.h")	
file(GLOB HRROBOTPUB_HEAD_LIST  "hrRobotPub/*.h")

install(FILES "baselib.h" DESTINATION BaseLib/include/)
install(FILES ${COMMON_HEAD_LIST} DESTINATION BaseLib/include/common)
install(FILES ${TINYXML_HEAD_LIST} DESTINATION BaseLib/include/tinyxml)
install(FILES ${HRROBOTPUB_HEAD_LIST} DESTINATION BaseLib/include/hrRobotPub)
install(FILES ${PROJECT_SOURCE_DIR}/jsoncpp/json.h DESTINATION BaseLib/include/jsoncpp)
install(FILES ${PROJECT_SOURCE_DIR}/sqlite3/sqlite3.h DESTINATION BaseLib/include/sqlite3)
