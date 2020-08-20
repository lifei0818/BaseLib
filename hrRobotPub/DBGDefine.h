#ifndef BASELIB_DBGDEFINE_H
#define BASELIB_DBGDEFINE_H

/************************************************************************/
/* lifei
/************************************************************************/
const long DBG_SERVER_RECV = 0x1001;			///服务端接收消息
const long DBG_SERVER_SEND = 0x1002;			///服务端发送消息

const long DBG_CLIENT_RECV = 0x1003;			///客户端接收消息
const long DBG_CLIENT_SEND = 0x1004;			///客户端发送消息
const long DBG_INTERPRETER_SEND = 0x1005;		///解析器

const long DBG_LINK_ETHERCAT = 0x1006;  		///ethercat

const long DBG_ServerAPI = 0x1007;  		///ethercat
const long DBG_ClientAPI = 0x2007;  		///ethercat

#endif
