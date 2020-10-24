void appendMenuItem(HMENU hMenu, UINT uMenuId, LPTSTR szName,
                    BOOL fDefault, BOOL fEnabled)
{
    MENUITEMINFO mii;

    memset(&mii, 0, sizeof(MENUITEMINFO));
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_ID | MIIM_TYPE | MIIM_STATE;
    if (_tcslen(szName))
    {
        mii.fType = MFT_STRING;
        mii.wID = uMenuId;
        if (fDefault) {
            mii.fState = MFS_DEFAULT;
        }
        if (!fEnabled) {
            mii.fState |= MFS_DISABLED;
        }
        mii.dwTypeData = szName;
    }
    else {
        mii.fType = MFT_SEPARATOR;
    }
    InsertMenuItem(hMenu, uMenuId, FALSE, &mii);
}