#ifndef KERNELQUERY_H
#define KERNELQUERY_H

#define EDS_KERNEL_LOCALPORT		"eds_kernel_localport"			///< 本地侦听端口
#define EDS_KERNEL_APPDIRECTORY		"eds_kernel_appdirectory"		///< 本程序所在目录
#define EDS_KERNEL_DATADIRECTORY	"eds_kernel_datadirectory"		///< 数据存在的目录
#define EDS_KERNEL_SERVERIPADDR		"eds_kernel_serveripaddr"		///< 服务器IP(v4或v6)地址
#define EDS_KERNEL_SERVERIPV4ADDR	"eds_kernel_serveripv4addr"		///< 服务器IPv4地址
#define EDS_KERNEL_SERVERIPV6ADDR	"eds_kernel_serveripv6addr"		///< 服务器IPv6地址
#define EDS_KERNEL_ONLYID			"eds_kernel_onlyid"				///< onlyId
#define EDS_KERNEL_VERSION			"eds_kernel_version"			///< 版本
#define EDS_KERNEL_MAINIPADDR		"eds_kernel_mainipaddr"			///< 主通讯 IP 地址
#define EDS_KERNEL_VRVSERVERIPADDR	"eds_kernel_vrvserveripaddr"	///< vrv服务器IP地址
#define EDS_KERNEL_LOGONUSER		"eds_kernel_logonuser"			///< 当前登录用户
#define EDS_KERNEL_LOGONUSERSID		"eds_kernel_logonusersid"		///< 当前登录用户 SID
#define EDS_KERNEL_HWND				"eds_kernel_hwnd"				///< 主窗口句柄
#define EDS_KERNEL_POINTIP			"eds_kernel_pointip"			///< 超级IP(P2P绑定IP)
#define EDS_KERNEL_POWERTIME		"eds_kernel_powertime"			///< 本机开机时间
#define EDS_KERNEL_STARTTIME		"eds_kernel_starttime"			///< Agent启动时间
#define EDS_KERNEL_FWSTATE			"eds_kernel_fwstate"			///< 防火墙状态
#define EDS_KERNEL_IOREAD			"eds_kernel_ioread"		    	///< io读取阀值
#define EDS_KERNEL_PATCHXMLCRC		"eds_kernel_patchxmlcrc"    	///< 补丁索引文件CRC
#define EDS_KERNEL_PATCHSERVER		"eds_kernel_patchserver"    	///< 补丁服务器地址
#define EDS_KERNEL_TEMPDIRECTORY	"eds_kernel_tempdirectory"		///< 临时数据的目录
#define	EDS_KERNEL_ARPGRACESEC	    "eds_kernel_arpgraceperiod"		///< ARP阻断宽限期（即注册保护期），单位秒
#define	EDS_KERNEL_ARPADMITVRV	    "eds_kernel_arpadmitvrv"        ///< ARP阻断是否兼容VRV客户端（即客户端安装VRV时不会被阻断）
#define	EDS_KERNEL_HTTPAUDITALIVE	"eds_kernel_HttpAuditAlive"     ///< 判断综合应用审计是否存活
#define	EDS_KERNEL_PROTECTSTATUS	"eds_kernel_ProtectStatus"      ///< 获取设备保护状态
#define	EDS_KERNEL_LOCKSTATUS	    "eds_kernel_LockStatus"         ///< 获取设备锁定状态
#define	EDS_KERNEL_ONLINESTATUS	    "eds_kernel_OnlineStatus"       ///< 本机在网状态（即能连通管控中心或者指定IP）
#define	EDS_KERNEL_ONLINECHECKIP	"eds_kernel_OnlineCheckIP"      ///< 检测本机在网状态的IP
#define	EDS_KERNEL_SECURITYLEVEL	"eds_kernel_SecurityLevel"      ///< 本机系统安全级别
#define EDS_KERNEL_LOGONPKISTATE	"eds_kernel_LogonPkiState"		///< 当前登录PKI状态, 0->有效, 其他无效
#define EDS_KERNEL_LOGONPKIUSER		"eds_kernel_LogonPkiUser"		///< 当前登录PKI用户
#define EDS_KERNEL_LOGONPKIID		"eds_kernel_LogonPkiId"			///< 当前登录PKI编号
#define	EDS_KERNEL_PKIUSERNAME		"eds_kernel_PkiUserName"		///< 当前PKI用户名
#define	EDS_KERNEL_PKIID			"eds_kernel_PkiID"				///< 当前PKI用户ID号
#define	EDS_KERNEL_PKIUNITNO		"eds_kernel_PkiUnitNo"			///< 当前PKI单位编号
#define EDS_KERNEL_BOXIP			"eds_kernel_BoxIP"				///< linux 跳转设备ip
#define EDS_KERNEL_ISARPSCANNER		"eds_kernel_IsArpScanner"		///< 是否在进行ARP扫描 0-否 1-是
#define EDS_KERNEL_REMOVALDISK		"eds_kernel_removaldisk"		///< 查询移动介质盘符
#define EDS_KERNEL_DIFFTIME 		"eds_kernel_difftime"		    ///< 服务器时间与本机时间的差值，单位为秒
#define EDS_KERNEL_ISSINGLE			"eds_kernel_isSingle"			///< 是否为单机版 0-否 1-是 （目前仅用于判断是否为单机版主机监控与审计系统）

#endif
