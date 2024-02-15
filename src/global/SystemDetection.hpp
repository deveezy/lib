#pragma once

#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
  #define K_OS_APPLE
#elif !defined(SAG_COM) && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP) && \
    (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
  #define K_OS_WIN32
  #define K_OS_WIN64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
  #define K_OS_WIN32
#elif defined(__linux__) || defined(__linux)
  #define K_OS_LINUX
#else
  #error "Lib has not been ported to this OS"
#endif

#if defined(K_OS_WIN32) || defined(K_OS_WIN64)
  #define K_OS_WINDOWS
  #define K_OS_WIN
#endif
