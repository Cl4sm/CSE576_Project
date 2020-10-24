BOOL GetApacheServicesStatus()
{
    TCHAR szKey[MAX_PATH];
    TCHAR achKey[MAX_PATH];
    TCHAR szImagePath[MAX_PATH];
    TCHAR szBuf[MAX_PATH];
    TCHAR szTmp[MAX_PATH];
    HKEY hKey, hSubKey, hKeyRemote;
    DWORD retCode, rv, dwKeyType;
    DWORD dwBufLen = MAX_PATH;
    int i, stPos = 0;
    int computers = 0;

    g_bRescanServices = FALSE;

    am_ClearServicesSt();
    while (g_stComputers[computers].szComputerName != NULL) {
        hKeyRemote = g_stComputers[computers].hRegistry;
        retCode = RegOpenKeyEx(hKeyRemote,
                               _T("System\\CurrentControlSet\\Services\\"),
                               0, KEY_READ, &hKey);
        if (retCode != ERROR_SUCCESS)
        {
            ErrorMessage(NULL, FALSE);
            return FALSE;
        }
        for (i = 0, retCode = ERROR_SUCCESS; retCode == ERROR_SUCCESS; i++)
        {
            retCode = RegEnumKey(hKey, i, achKey, MAX_PATH);
            if (retCode == ERROR_SUCCESS)
            {
                _tcscpy(szKey, _T("System\\CurrentControlSet\\Services\\"));
                _tcscat(szKey, achKey);

                if (RegOpenKeyEx(hKeyRemote, szKey, 0,
                                 KEY_QUERY_VALUE, &hSubKey) == ERROR_SUCCESS)
                {
                    dwBufLen = MAX_PATH;
                    rv = RegQueryValueEx(hSubKey, _T("ImagePath"), NULL,
                                         &dwKeyType, (LPBYTE)szImagePath, &dwBufLen);

                    if (rv == ERROR_SUCCESS
                            && (dwKeyType == REG_SZ
                             || dwKeyType == REG_EXPAND_SZ)
                            && dwBufLen)
                    {
                        _tcscpy(szBuf, szImagePath);
                        CharLower(szBuf);
                        /* the service name could be httpd*.exe or Apache*.exe */
                        if (((_tcsstr(szBuf, _T("\\apache")) != NULL)
                             || (_tcsstr(szBuf, _T("\\httpd")) != NULL))
                                && _tcsstr(szBuf, _T(".exe"))
                                && (_tcsstr(szBuf, _T("--ntservice")) != NULL
                                       || _tcsstr(szBuf, _T("-k ")) != NULL))
                        {
                            g_stServices[stPos].szServiceName = _tcsdup(achKey);
                            g_stServices[stPos].szImagePath = _tcsdup(szImagePath);
                            g_stServices[stPos].szComputerName =
                                _tcsdup(g_stComputers[computers].szComputerName);
                            dwBufLen = MAX_PATH;
                            if (RegQueryValueEx(hSubKey, _T("Description"), NULL,
                                                &dwKeyType, (LPBYTE)szBuf, &dwBufLen)
                                    == ERROR_SUCCESS) {
                                g_stServices[stPos].szDescription = _tcsdup(szBuf);
                            }
                            dwBufLen = MAX_PATH;
                            if (RegQueryValueEx(hSubKey, _T("DisplayName"), NULL,
                                                &dwKeyType, (LPBYTE)szBuf, &dwBufLen)
                                    == ERROR_SUCCESS)
                            {
                                if (_tcscmp(g_stComputers[computers]
                                        .szComputerName, g_szLocalHost) != 0)
                                {
                                    _tcscpy(szTmp, g_stComputers[computers]
                                                      .szComputerName + 2);
                                    _tcscat(szTmp, _T("@"));
                                    _tcscat(szTmp, szBuf);
                                }
                                else {
                                    _tcscpy(szTmp, szBuf);
                                }
                                g_stServices[stPos].szDisplayName = _tcsdup(szTmp);

                            }
                            ++stPos;
                            if (stPos >= MAX_APACHE_SERVICES) {
                                retCode = !ERROR_SUCCESS;
                            }
                        }
                    }
                    RegCloseKey(hSubKey);
                }
            }
        }
        ++computers;
        RegCloseKey(hKey);
    }
    FindRunningServices();
    return TRUE;
}