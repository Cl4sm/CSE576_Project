BOOL GetSystemOSVersion(LPDWORD dwVersion)
{
    OSVERSIONINFO osvi;
    /*
    Try calling GetVersionEx using the OSVERSIONINFOEX structure.
    If that fails, try using the OSVERSIONINFO structure.
    */
    memset(&osvi, 0, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    if (!GetVersionEx(&osvi)) {
        return FALSE;
    }

    switch (osvi.dwPlatformId)
    {
    case VER_PLATFORM_WIN32_NT:
        if (osvi.dwMajorVersion >= 5)
            *dwVersion = OS_VERSION_WIN2K;
        else
            *dwVersion = OS_VERSION_WINNT;
        break;

    case VER_PLATFORM_WIN32_WINDOWS:
    case VER_PLATFORM_WIN32s:
    default:
        *dwVersion = 0;
        return FALSE;
    }
    return TRUE;
}