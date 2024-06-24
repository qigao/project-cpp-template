#ifndef __MYLIB_H__
#define __MYLIB_H__

#if defined(_MSC_VER)
    #if defined(MYLIB_EXPORTS)
        #define MVIEW_API __declspec(dllexport)
    #else
        #define MVIEW_API __declspec(dllimport)
    #endif
#else
    #define MVIEW_API __attribute__((visibility("default")))
#endif
#ifdef __cplusplus
extern "C" {
#endif

    // -----------------------------------------------------------

    MVIEW_API int Sum(const int pLhs, const int pRhs);

    // -----------------------------------------------------------

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // __MYLIB_H__
