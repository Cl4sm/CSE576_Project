int am_RespawnAsUserAdmin(HWND hwnd, DWORD op, LPCTSTR szService,
                          LPCTSTR szComputerName)
{
    TCHAR args[MAX_PATH + MAX_COMPUTERNAME_LENGTH + 12];

    if (g_dwOSVersion < OS_VERSION_WIN2K) {
        ErrorMessage(g_lpMsg[IDS_MSG_SRVFAILED - IDS_MSG_FIRST], FALSE);
        return 0;
    }

    _sntprintf(args, sizeof(args) / sizeof(TCHAR),
               _T("%d \"%s\" \"%s\""), op, szService,
               szComputerName ? szComputerName : _T(""));
    if (!ShellExecute(hwnd, _T("runas"), __targv[0], args, NULL, SW_NORMAL)) {
        ErrorMessage(g_lpMsg[IDS_MSG_SRVFAILED - IDS_MSG_FIRST],
                     FALSE);
        return 0;
    }

    return 1;
}