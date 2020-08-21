#pragma once

#define FNM_PATHNAME (1 << 0) /* No wildcard can ever match '/'.  */
#define FNM_NOESCAPE (1 << 1) /* Backslashes don't quote special chars.  */
#define FNM_PERIOD (1 << 2) /* Leading '.' is matched only explicitly.  */

int fnmatch(const char* pattern, const char* string, int flags);