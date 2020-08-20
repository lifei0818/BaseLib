#ifndef CXXDEV_OS_MACRO_H
#define CXXDEV_OS_MACRO_H

//OS�����ж�
#if defined (_WIN32)       //windows����
    #define _MACRO_OS_WIN32
#elif defined (__linux__)    //Linux����
    #define _MACRO_OS_LINUX
#endif

//�����������ж�
#if defined (__GNUC__)     //gcc������
    #define _MACRO_CC_GCC
    #ifdef __LP64__
        #define _MACRO_BIT_64
    #else
        #define _MACRO_BIT_32
    #endif
#elif defined (_MSC_VER)   //VC������
    #define _MACRO_CC_MSVC
    #ifdef _WIN64
        #define _MACRO_BIT_64
    #else
        #define _MACRO_BIT_32
    #endif
#endif

#endif
