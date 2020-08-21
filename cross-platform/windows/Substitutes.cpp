#pragma once
#ifdef _WIN32
#include "cross-platform/windows/Substitutes.hpp"

#include <Shlwapi.h>
#include <Windows.h>
#include <stddef.h>
#include <iostream>

int fnmatch(const char *pattern, const char *string, int flags)
{
    if (flags != 0) 
    {
        std::cerr << __FUNCTION__ << "Warning: flags other than 0 are not supported in Windows.";
    }
    wchar_t pszFile[1024];
    wchar_t pszSpec[1024];

    size_t outsize;
    mbstowcs_s(&outsize, pszFile, string, strlen(string) + 1);
    mbstowcs_s(&outsize, pszSpec, pattern, strlen(pattern) + 1);

    return(!PathMatchSpecW(pszFile, pszSpec));
}

#endif