BOOL ApacheManageService(LPCTSTR szServiceName, LPCTSTR szImagePath,
                         LPTSTR szComputerName, DWORD dwCommand)
{
    TCHAR szMsg[MAX_PATH];
    BOOL retValue;
    SC_HANDLE schService;
    SC_HANDLE schSCManager;
    SERVICE_STATUS schSStatus;
    int ticks;

    schSCManager = OpenSCManager(szComputerName, NULL,
                                 SC_MANAGER_CONNECT);
    if (!schSCManager) {
        ErrorMessage(g_lpMsg[IDS_MSG_SRVFAILED - IDS_MSG_FIRST],
                     FALSE);
        return FALSE;
    }

    schService = OpenService(schSCManager, szServiceName,
                             SERVICE_QUERY_STATUS | SERVICE_START |
                             SERVICE_STOP | SERVICE_USER_DEFINED_CONTROL);
    if (schService == NULL)
    {
        /* Avoid recursion of ImagePath NULL (from this Respawn) */
        if (szImagePath) {
            am_RespawnAsUserAdmin(g_hwndMain, dwCommand,
                                  szServiceName, szComputerName);
        }
        else {
            ErrorMessage(g_lpMsg[IDS_MSG_SRVFAILED - IDS_MSG_FIRST],
                         FALSE);
        }
        CloseServiceHandle(schSCManager);
        return FALSE;
    }
    else
    {
        retValue = FALSE;
        g_bConsoleRun = TRUE;
        SetCursor(g_hCursorHourglass);
        switch (dwCommand)
        {
          case SERVICE_CONTROL_STOP:
            _sntprintf(szMsg, sizeof(szMsg) / sizeof(TCHAR),
                       g_lpMsg[IDS_MSG_SRVSTOP - IDS_MSG_FIRST],
                       szServiceName);
            addListBoxString(g_hwndStdoutList, szMsg);
            if (ControlService(schService, SERVICE_CONTROL_STOP,
                               &schSStatus)) {
                Sleep(1000);
                while (QueryServiceStatus(schService, &schSStatus))
                {
                    if (schSStatus.dwCurrentState == SERVICE_STOP_PENDING)
                    {
                        Sleep(1000);
                    }
                    else {
                        break;
                    }
                }
            }
            if (QueryServiceStatus(schService, &schSStatus))
            {
                if (schSStatus.dwCurrentState == SERVICE_STOPPED)
                {
                    retValue = TRUE;
                    _sntprintf(szMsg, sizeof(szMsg) / sizeof(TCHAR),
                               g_lpMsg[IDS_MSG_SRVSTOPPED - IDS_MSG_FIRST],
                               szServiceName);
                    addListBoxString(g_hwndStdoutList, szMsg);
                }
            }
            break;

          case SERVICE_CONTROL_CONTINUE:
            _sntprintf(szMsg, sizeof(szMsg) / sizeof(TCHAR),
                       g_lpMsg[IDS_MSG_SRVSTART - IDS_MSG_FIRST],
                       szServiceName);
            addListBoxString(g_hwndStdoutList, szMsg);

            if (StartService(schService, 0, NULL))
            {
                Sleep(1000);
                while (QueryServiceStatus(schService, &schSStatus))
                {
                    if (schSStatus.dwCurrentState == SERVICE_START_PENDING)
                    {
                        Sleep(1000);
                    }
                    else {
                        break;
                    }
                }
            }
            if (QueryServiceStatus(schService, &schSStatus))
            {
                if (schSStatus.dwCurrentState == SERVICE_RUNNING)
                {
                    retValue = TRUE;
                    _sntprintf(szMsg, sizeof(szMsg) / sizeof(TCHAR),
                               g_lpMsg[IDS_MSG_SRVSTARTED - IDS_MSG_FIRST],
                               szServiceName);
                    addListBoxString(g_hwndStdoutList, szMsg);
                }
            }
            break;

          case SERVICE_APACHE_RESTART:
            _sntprintf(szMsg, sizeof(szMsg) / sizeof(TCHAR),
                       g_lpMsg[IDS_MSG_SRVRESTART - IDS_MSG_FIRST],
                       szServiceName);
            addListBoxString(g_hwndStdoutList, szMsg);
            if (ControlService(schService, SERVICE_APACHE_RESTART,
                               &schSStatus))
            {
                ticks = 60;
                while (schSStatus.dwCurrentState == SERVICE_START_PENDING)
                {
                    Sleep(1000);
                    if (!QueryServiceStatus(schService, &schSStatus))
                    {
                        CloseServiceHandle(schService);
                        CloseServiceHandle(schSCManager);
                        g_bConsoleRun = FALSE;
                        SetCursor(g_hCursorArrow);
                        return FALSE;
                    }
                    if (!--ticks) {
                        break;
                    }
                }
            }
            if (schSStatus.dwCurrentState == SERVICE_RUNNING)
            {
                retValue = TRUE;
                _sntprintf(szMsg, sizeof(szMsg) / sizeof(TCHAR),
                           g_lpMsg[IDS_MSG_SRVRESTARTED - IDS_MSG_FIRST],
                           szServiceName);
                addListBoxString(g_hwndStdoutList, szMsg);
            }
            break;
        }
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        if (!retValue) {
            ErrorMessage(g_lpMsg[IDS_MSG_SRVFAILED - IDS_MSG_FIRST],
                         FALSE);
        }
        g_bConsoleRun = FALSE;
        SetCursor(g_hCursorArrow);
        return retValue;
    }
    return FALSE;
}