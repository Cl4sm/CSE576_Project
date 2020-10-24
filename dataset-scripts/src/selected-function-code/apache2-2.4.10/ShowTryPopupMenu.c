void ShowTryPopupMenu(HWND hWnd)
{
    /* create popup menu */
    HMENU hMenu = CreatePopupMenu();
    POINT pt;

    if (hMenu)
    {
        appendMenuItem(hMenu, IDM_RESTORE,
                       g_lpMsg[IDS_MSG_MNUSHOW - IDS_MSG_FIRST],
                       TRUE, TRUE);
        appendMenuItem(hMenu, IDC_SMANAGER,
                       g_lpMsg[IDS_MSG_MNUSERVICES - IDS_MSG_FIRST],
                       FALSE, TRUE);
        appendMenuItem(hMenu, 0, _T(""), FALSE, TRUE);
        appendMenuItem(hMenu, IDM_EXIT,
                       g_lpMsg[IDS_MSG_MNUEXIT - IDS_MSG_FIRST],
                       FALSE, TRUE);

        if (!SetForegroundWindow(hWnd)) {
            SetForegroundWindow(NULL);
        }
        GetCursorPos(&pt);
        TrackPopupMenu(hMenu, TPM_LEFTALIGN|TPM_RIGHTBUTTON,
                       pt.x, pt.y, 0, hWnd, NULL);
        DestroyMenu(hMenu);
    }
}