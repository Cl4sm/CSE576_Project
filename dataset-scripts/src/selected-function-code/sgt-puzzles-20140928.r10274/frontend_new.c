static frontend *frontend_new(HINSTANCE inst)
{
    frontend *fe;
    const char *nogame = "Puzzles (no game selected)";

    fe = snew(frontend);

    fe->inst = inst;

    fe->game = NULL;
    fe->me = NULL;

    fe->timer = 0;
    fe->hwnd = NULL;

    fe->help_running = FALSE;

    fe->drawstatus = NOTHING;
    fe->dr = NULL;
    fe->fontstart = 0;

    fe->fonts = NULL;
    fe->nfonts = fe->fontsize = 0;

    fe->colours = NULL;
    fe->brushes = NULL;
    fe->pens = NULL;

    fe->puzz_scale = 1.0;

    #ifdef _WIN32_WCE
    MultiByteToWideChar (CP_ACP, 0, nogame, -1, wGameName, 256);
    fe->hwnd = CreateWindowEx(0, wClassName, wGameName,
			      WS_VISIBLE,
			      CW_USEDEFAULT, CW_USEDEFAULT,
			      CW_USEDEFAULT, CW_USEDEFAULT,
			      NULL, NULL, inst, NULL);

    {
	SHMENUBARINFO mbi;
	RECT rc, rcBar, rcTB, rcClient;

	memset (&mbi, 0, sizeof(SHMENUBARINFO));
	mbi.cbSize     = sizeof(SHMENUBARINFO);
	mbi.hwndParent = fe->hwnd;
	mbi.nToolBarId = IDR_MENUBAR1;
	mbi.hInstRes   = inst;

	SHCreateMenuBar(&mbi);

	GetWindowRect(fe->hwnd, &rc);
	GetWindowRect(mbi.hwndMB, &rcBar);
	rc.bottom -= rcBar.bottom - rcBar.top;
	MoveWindow(fe->hwnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, FALSE);

        fe->numpad = NULL;
    }
#else
    fe->hwnd = CreateWindowEx(0, CLASSNAME, nogame,
			      WS_OVERLAPPEDWINDOW &~
			      (WS_MAXIMIZEBOX),
			      CW_USEDEFAULT, CW_USEDEFAULT,
			      CW_USEDEFAULT, CW_USEDEFAULT,
			      NULL, NULL, inst, NULL);
    if (!fe->hwnd) {
        DWORD lerr = GetLastError();
        printf("no window: 0x%x\n", lerr);
    }
#endif

    fe->gamemenu = NULL;
    fe->presets = NULL;

    fe->statusbar = NULL;
    fe->bitmap = NULL;

    SetWindowLong(fe->hwnd, GWL_USERDATA, (LONG)fe);

    return fe;
}