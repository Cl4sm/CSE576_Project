BOOL IsServiceRunning(LPCTSTR szServiceName, LPCTSTR szComputerName,
                      LPDWORD lpdwPid)
{
    DWORD dwPid;
    SC_HANDLE schService;
    SC_HANDLE schSCManager;
    SERVICE_STATUS schSStatus;

    dwPid = 0;
    schSCManager = OpenSCManager(szComputerName, NULL,
                                SC_MANAGER_CONNECT);
    if (!schSCManager) {
        return FALSE;
    }

    schService = OpenService(schSCManager, szServiceName,
                             SERVICE_QUERY_STATUS);
    if (schService != NULL)
    {
        if (QueryServiceStatus(schService, &schSStatus))
        {
            dwPid = schSStatus.dwCurrentState;
            if (lpdwPid) {
                *lpdwPid = 1;
            }
        }
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return dwPid == SERVICE_RUNNING ? TRUE : FALSE;
    }
    else {
        g_bRescanServices = TRUE;
    }
    CloseServiceHandle(schSCManager);
    return FALSE;
}