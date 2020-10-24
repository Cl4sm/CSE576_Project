void appendServiceMenu(HMENU hMenu, UINT uMenuId,
                       LPTSTR szServiceName, BOOL fRunning)
{
    MENUITEMINFO mii;
    HMENU smh;

    smh = CreatePopupMenu();

    appendMenuItem(smh, IDM_SM_START + uMenuId,
                   g_lpMsg[IDS_MSG_SSTART - IDS_MSG_FIRST], FALSE, !fRunning);
    appendMenuItem(smh, IDM_SM_STOP + uMenuId,
                   g_lpMsg[IDS_MSG_SSTOP - IDS_MSG_FIRST], FALSE, fRunning);
    appendMenuItem(smh, IDM_SM_RESTART + uMenuId,
                   g_lpMsg[IDS_MSG_SRESTART - IDS_MSG_FIRST], FALSE, fRunning);

    memset(&mii, 0, sizeof(MENUITEMINFO));
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_ID | MIIM_TYPE | MIIM_STATE | MIIM_SUBMENU
              | MIIM_CHECKMARKS;
    mii.fType = MFT_STRING;
    mii.wID = uMenuId;
    mii.hbmpChecked = g_hBmpStart;
    mii.hbmpUnchecked = g_hBmpStop;
    mii.dwTypeData = szServiceName;
    mii.hSubMenu = smh;
    mii.fState = fRunning ? MFS_CHECKED : MFS_UNCHECKED;
    InsertMenuItem(hMenu, IDM_SM_SERVICE + uMenuId, FALSE, &mii);
}