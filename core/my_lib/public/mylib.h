#ifndef __MYLIB_H__
#define __MYLIB_H__

#if defined(_MSC_VER)
    #if defined(MYLIB_EXPORTS)
        #define MYLIB_API __declspec(dllexport)
    #else
        #define MYLIB_API __declspec(dllimport)
    #endif
#else
    #define MYLIB_API __attribute__((visibility("default")))
#endif
#ifdef __cplusplus
extern "C" {
#endif

    // -----------------------------------------------------------

   MYLIB_API int Sum(const int pLhs, const int pRhs);

    // -----------------------------------------------------------

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // __MYLIB_H__
