#ifndef __HTTP_FILE_TRANSFER_H__
#define __HTTP_FILE_TRANSFER_H__


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

 /**
 * Calculates the sum of two integers.
 *
 * @param pLhs The first integer to be summed.
 * @param pRhs The second integer to be summed.
 *
 * @return The sum of the two integers.
 *
 * @throws None
 */
   MYLIB_API int Sum(const int pLhs, const int pRhs);
   /**
 * Downloads a file synchronously from the specified URL and saves it locally.
 *
 * @param url The URL of the file to download.
 * @param local_filename The local filename to save the downloaded file.
 */
   MYLIB_API void sync_file_download(const char* url,const char* local_filename);
   /**
 * Downloads a file asynchronously from the specified URL and saves it locally.
 *
 * @param url The URL of the file to download.
 * @param local_filename The local filename to save the downloaded file.
 *
 * @throws None
 */
   MYLIB_API void async_file_download(const char* url,const char* local_filename);

#ifdef __cplusplus
}
#endif //__cplusplus


#endif // __HTTP_FILE_TRANSFER_H__