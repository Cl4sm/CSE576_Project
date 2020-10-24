void ShowTryServicesMenu(HWND hWnd)
{
    /* create services list popup menu and submenus */
    HMENU hMenu = CreatePopupMenu();
    POINT pt;
    int i = 0;

    if (hMenu)
    {
        while (g_stServices[i].szServiceName != NULL)
        {
            appendServiceMenu(hMenu, i, g_stServices[i].szDisplayName,
                              g_stServices[i].dwPid != 0);
            ++i;
        }
        if (i)
        {
            if (!SetForegroundWindow(hWnd)) {
                SetForegroundWindow(NULL);
            }
            GetCursorPos(&pt);
            TrackPopupMenu(hMenu, TPM_LEFTALIGN|TPM_RIGHTBUTTON,
                           pt.x, pt.y, 0, hWnd, NULL);
            DestroyMenu(hMenu);
        }
    }
}