#pragma once
#ifdef _WIN32
    #define HIDDEN_DLL_API
    #ifdef LIB_EXPORT
        #define DLL_API __declspec(dllexport)
    #else 
        #define DLL_API __declspec(dllimport)
    #endif
#else 
    #define DLL_API __attribute__((visibility("default")))
    #define HIDDEN_DLL_API __attribute__((visibility("hidden")))
#endif