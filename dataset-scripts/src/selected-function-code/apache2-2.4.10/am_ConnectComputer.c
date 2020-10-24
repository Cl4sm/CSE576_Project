BOOL am_ConnectComputer(LPTSTR szComputerName)
{
    int i = 0;
    HKEY hKeyRemote;
    TCHAR szTmp[MAX_PATH];

    while (g_stComputers[i].szComputerName != NULL) {
        if (_tcscmp(g_stComputers[i].szComputerName, szComputerName) == 0) {
            return FALSE;
        }
        ++i;
    }
    if (i > MAX_APACHE_COMPUTERS - 1) {
        return FALSE;
    }
    if (RegConnectRegistry(szComputerName, HKEY_LOCAL_MACHINE, &hKeyRemote)
            != ERROR_SUCCESS) {
        _sntprintf(szTmp, sizeof(szTmp) / sizeof(TCHAR),
                   g_lpMsg[IDS_MSG_ECONNECT - IDS_MSG_FIRST],
                   szComputerName);
        ErrorMessage(szTmp, FALSE);
        return FALSE;
    }
    else {
        g_stComputers[i].szComputerName = _tcsdup(szComputerName);
        g_stComputers[i].hRegistry = hKeyRemote;
        return TRUE;
    }
}