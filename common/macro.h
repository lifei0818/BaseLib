#ifndef BaseLib_MACRO_H
#define BaseLib_MACRO_H

/************************************************************************/
/* 
/************************************************************************/


/************************************************************************/
/* 
/************************************************************************/



#ifndef MAKELONGDWORD
#define MAKELONGDWORD(low,high) \
	((DWORDLONG)(((DWORD)(low)) | ((DWORDLONG)((DWORD)(high))) << 32))
#endif

// 数组元素的大小
#define ARRAY_SIZE(X) (sizeof(X)/sizeof(X[0]))

// 空字符串时使用
#define NULL_STRING _T("")
#define SAFE_STRING(STR) (STR!=NULL?STR:NULL_STRING)


///	缓冲区尺寸
#define STANDARD_SIZE		1024
#define PAGE_SIZE			4096
#define MAX_SIZE			65536

#define ST_LIVE_NORMAL 0	///< 普通状态
#define ST_LIVE_DELETE 1	///< 删除状态

/// 秒转换
#define		SEC_TIME(X)		(X)
#define 	MIN_TIME(X)		SEC_TIME((X)*60)
#define 	HOUR_TIME(X)	MIN_TIME((X)*60)
#define 	DAY_TIME(X)		HOUR_TIME((X)*24)

#define		MUL_1000(X)		((X)*1000)
#define		DIV_1000(X)		((X)/1000)

#define NEW_LINE			_T("\r\n")		///< 标准换行
#define LOG_NEW_LINE		_T("\n\n")		///< LOG标准换行
#define LOG_SEPARATOR		_T("<>")		///< 日志的分割符
#define POLICY_SEPARATOR	_T("\r\n\n\n")	///< 策略分割符号
#define SMP_SEPARATOR		_T("\1\1")		///< 不可见分割符号

#define EQUAL_STRING		_T("=")
#define LEFT_MIDDLE_BRACKET_STRING	_T("[")
#define RIGHT_MIDDLE_BRACKET_STRING _T("]")
#define LEFT_BRACKET_STRING _T("(") 
#define RIGHT_BRACKET_STRING _T(")")
#define TAB_STRING			_T("\t")
#define COLON_STRING		_T(":")
#define SEMICOLON			_T(";")
#define COMMA				_T(",")
#define BLANKSPACE          _T(" ")
#define BIG_BLANKSPACE		_T("　")
#define DOUBLE_QUOTATION    _T("\"")


//路径分隔符
#ifdef _MACRO_OS_WIN32
#define PATH_SEPARATOR      _T('\\')
#define PATH_SEP_STRING     _T("\\")
#else
#define PATH_SEPARATOR      _T('/')
#define PATH_SEP_STRING     _T("/")
#endif

///	格式输出
#define FORMAT_OUT(STR,VAL)									STR<<EQUAL_STRING<<VAL

#define FORMAT_OUT_SEMICOLON(STR, VAL)						STR<<EQUAL_STRING<<VAL<<SEMICOLON 
#define FORMAT_OUT_SEMICOLON_WITH_UNIT(STR1, VAL, STR2)		STR1<<EQUAL_STRING<<VAL<<STR2<<SEMICOLON

#define NEWLINE_FORMAT_OUT(STR,VAL)							STR<<EQUAL_STRING<<VAL<<NEW_LINE
#define NEWLINE_FORMAT_OUT_WITH_UNITS(STR1, VAL, STR2)		STR1<<EQUAL_STRING<<VAL<<STR2<<NEW_LINE
#define NEWLINE_FORMAT_COUNT_OUT(STR,I,VAL)					STR<<I<<EQUAL_STRING<<VAL<<NEW_LINE

#define SEMICOLON_FORMAT_OUT(STR)							STR<<SEMICOLON

#define LOG_FORMAT_OUT(STR, VAL)							FORMAT_OUT(STR,VAL)<<LOG_SEPARATOR
#define LOG_FORMAT_COUNT_OUT(STR, I, VAL)                   STR<<I<<EQUAL_STRING<<VAL<<LOG_SEPARATOR
#define POLICY_FORMAT_OUT(STR, VAL)							FORMAT_OUT(STR,VAL)<<POLICY_SEPARATOR
#define POLICY_FORMAT_COUNT_OUT(STR,I,VAL)					STR<<I<<EQUAL_STRING<<VAL<<POLICY_SEPARATOR

#define NEWLINE_INDEX_FORMAT_OUT(STR,I,VAL)					STR<<_T("_")<<I<<EQUAL_STRING<<VAL<<NEW_LINE
#define NEWLINE_INDEX_FORMAT_OUT_EX(STR,I,J,VAL)			STR<<I<<_T("-")<<J<<EQUAL_STRING<<VAL<<NEW_LINE
#define NEWLINE_FORMAT_COUNT_OUT_EX(STR1,I,STR2,VAL)		STR1<<I<<STR2<<EQUAL_STRING<<VAL<<NEW_LINE

#define PERCENT(X,SIZE) ((X)*100/(SIZE))

#define FORMAT_WITH_BRACKET(STR, VAL)                       STR<<LEFT_BRACKET_STRING<<VAL<<RIGHT_BRACKET_STRING
#define FORMAT_WITH_BRACKET_STRING(STR, VAL)                FORMAT_WITH_BRACKET(STR, VAL)<<COMMA<<BLANKSPACE                           

#define FORMAT_IPCONFIG_INFO(STR, VAL)                      STR<<COLON_STRING<<VAL<<NEW_LINE

#define COLON_STRING_FORMAT_BLANK(STR, VAL)             STR<<COLON_STRING<<VAL<<BLANKSPACE                      

#define FORMAT_MIDDLE_BRACKET(STR)                  LEFT_MIDDLE_BRACKET_STRING<<STR<<RIGHT_MIDDLE_BRACKET_STRING<<NEW_LINE           

///	参数检查
#define INVALID_PARAM_LPSTR(X) \
	((!X) || ('\0' == *X))

///	数字参数检查
#define INVALID_PARAM_DWORD(X) \
	((0 == X) || (-1 == X))

///	Handle 参数检查
#define INVALID_PARAM_HANDLE(X) \
	((0 == X) || (INVALID_HANDLE_VALUE == X))

// 类中数据成员的 GET 和 SET 
#define  GET_MEMBER(X)   Get##X
#define  SET_MEMBER(X)   Set##X

#define STRING_MEMBER_GET_SET(X) \
private: \
	std::string X; \
public: \
	void SET_MEMBER(X) (std::string i) {X = i;}  \
	std::string	GET_MEMBER(X)() { return X;}   

#define UINT_MEMBER_GET_SET(X)   \
private: \
   UINT X; \
public:  \
	void SET_MEMBER(X) (UINT i) { X = i;} \
	UINT GET_MEMBER(X)() { return X;}

#define DWORD_MEMBER_GET_SET(X) \
private:  \
	DWORD X; \
public: \
	void SET_MEMBER(X) (DWORD i) {X = i;} \
	DWORD GET_MEMBER(X)() { return X;}

#define MEMBER_GET_SET(T, X)  \
protected: \
	T  X;  \
public:  \
	void SET_MEMBER(X) (T i) {X = i;}  \
	T GET_MEMBER(X)(){return X;}


#define BREAK_MAC "0022556688"	///< 攻击的 MAC 地址

#endif  //BaseLib_MACRO_H
