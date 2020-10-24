char *geterrstr(void)
{
    LPVOID lpMsgBuf;
    DWORD dw = GetLastError();
    char *ret;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    ret = dupstr(lpMsgBuf);

    LocalFree(lpMsgBuf);

    return ret;
}