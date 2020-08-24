#pragma once
#ifdef _WIN32

#include <Shlwapi.h>
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
// IGNORE is used in InputErrorAction.hpp, as an enum value, 
// but is defined as a #def in WinBase.h which is included by Shlwapi.h.
// It is not required here, so we can undefine it.
#undef IGNORE
#undef GROUP_NAME
#undef ERROR
#include <iostream>
#include <stddef.h>

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

int fnmatch(const char* pattern, const char* string, int flags)
{
    if (flags != 0) {
        std::cerr << __FUNCTION__ << "Warning: flags other than 0 are not supported in Windows.";
    }
    wchar_t pszFile[1024];
    wchar_t pszSpec[1024];

    size_t outsize;
    mbstowcs_s(&outsize, pszFile, string, strlen(string) + 1);
    mbstowcs_s(&outsize, pszSpec, pattern, strlen(pattern) + 1);

    return (!PathMatchSpecW(pszFile, pszSpec));
}

#endif