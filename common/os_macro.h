#ifndef CXXDEV_OS_MACRO_H
#define CXXDEV_OS_MACRO_H

//OS环境判断
#if defined (_WIN32)       //windows环境
    #define _MACRO_OS_WIN32
#elif defined (__linux__)    //Linux环境
    #define _MACRO_OS_LINUX
#endif

//编译器类型判断
#if defined (__GNUC__)     //gcc编译器
    #define _MACRO_CC_GCC
    #ifdef __LP64__
        #define _MACRO_BIT_64
    #else
        #define _MACRO_BIT_32
    #endif
#elif defined (_MSC_VER)   //VC编译器
    #define _MACRO_CC_MSVC
    #ifdef _WIN64
        #define _MACRO_BIT_64
    #else
        #define _MACRO_BIT_32
    #endif
#endif

#endif
