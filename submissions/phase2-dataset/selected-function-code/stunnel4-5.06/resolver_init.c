void resolver_init() {
#if defined(USE_WIN32) && !defined(_WIN32_WCE)
    HINSTANCE handle;

        /* IPv6 in Windows XP or higher */
    handle=LoadLibrary(TEXT("ws2_32.dll"));
    if(handle) {
        s_getaddrinfo=(GETADDRINFO)GetProcAddress(handle, "getaddrinfo");
        s_freeaddrinfo=(FREEADDRINFO)GetProcAddress(handle, "freeaddrinfo");
        s_getnameinfo=(GETNAMEINFO)GetProcAddress(handle, "getnameinfo");
        if(s_getaddrinfo && s_freeaddrinfo && s_getnameinfo)
            return; /* IPv6 detected -> OK */
        FreeLibrary(handle);
    }

        /* experimental IPv6 for Windows 2000 */
    handle=LoadLibrary(TEXT("wship6.dll"));
    if(handle) {
        s_getaddrinfo=(GETADDRINFO)GetProcAddress(handle, "getaddrinfo");
        s_freeaddrinfo=(FREEADDRINFO)GetProcAddress(handle, "freeaddrinfo");
        s_getnameinfo=(GETNAMEINFO)GetProcAddress(handle, "getnameinfo");
        if(s_getaddrinfo && s_freeaddrinfo && s_getnameinfo)
            return; /* IPv6 detected -> OK */
        FreeLibrary(handle);
    }

        /* fall back to the built-in emulation */
    s_getaddrinfo=NULL;
    s_freeaddrinfo=NULL;
    s_getnameinfo=NULL;
#endif
}
