static int fe_set_midend(frontend *fe, midend *me)
{
    int x, y;
    RECT r;

    if (fe->me) midend_free(fe->me);
    fe->me = me;
    fe->game = midend_which_game(fe->me);

    {
	int i, ncolours;
        float *colours;

        colours = midend_colours(fe->me, &ncolours);

        if (fe->colours) sfree(fe->colours);
        if (fe->brushes) sfree(fe->brushes);
        if (fe->pens) sfree(fe->pens);

	fe->colours = snewn(ncolours, COLORREF);
	fe->brushes = snewn(ncolours, HBRUSH);
	fe->pens = snewn(ncolours, HPEN);

	for (i = 0; i < ncolours; i++) {
	    fe->colours[i] = RGB(255 * colours[i*3+0],
				 255 * colours[i*3+1],
				 255 * colours[i*3+2]);
	    fe->brushes[i] = CreateSolidBrush(fe->colours[i]);
	    fe->pens[i] = CreatePen(PS_SOLID, 1, fe->colours[i]);
	}
        sfree(colours);
    }

    if (fe->statusbar)
        DestroyWindow(fe->statusbar);
    if (midend_wants_statusbar(fe->me)) {
	fe->statusbar = CreateWindowEx(0, STATUSCLASSNAME, TEXT("ooh"),
				       WS_CHILD | WS_VISIBLE,
				       0, 0, 0, 0, /* status bar does these */
				       NULL, NULL, fe->inst, NULL);
    } else
        fe->statusbar = NULL;

    get_max_puzzle_size(fe, &x, &y);
    midend_size(fe->me, &x, &y, FALSE);

    r.left = r.top = 0;
    r.right = x;
    r.bottom = y;
    AdjustWindowRectEx(&r, WINFLAGS, TRUE, 0);

#ifdef _WIN32_WCE
    if (fe->numpad)
        DestroyWindow(fe->numpad);
    if (fe->game->flags & REQUIRE_NUMPAD)
    {
        fe->numpad = CreateToolbarEx (fe->hwnd,
                                      WS_VISIBLE | WS_CHILD | CCS_NOPARENTALIGN | TBSTYLE_FLAT,
                                      0, 10, fe->inst, IDR_PADTOOLBAR,
                                      tbNumpadButtons, sizeof (tbNumpadButtons) / sizeof (TBBUTTON),
                                      0, 0, 14, 15, sizeof (TBBUTTON));
        GetWindowRect(fe->numpad, &rcTB);
        GetClientRect(fe->hwnd, &rcClient);
        MoveWindow(fe->numpad, 
                   0, 
                   rcClient.bottom - (rcTB.bottom - rcTB.top) - 1,
                   rcClient.right,
                   rcTB.bottom - rcTB.top,
                   FALSE);
        SendMessage(fe->numpad, TB_SETINDENT, (rcClient.right - (10 * 21)) / 2, 0);
    }
    else {
	fe->numpad = NULL;
    }
    MultiByteToWideChar (CP_ACP, 0, fe->game->name, -1, wGameName, 256);
    SetWindowText(fe->hwnd, wGameName);
#else
    SetWindowText(fe->hwnd, fe->game->name);
#endif

    if (fe->statusbar)
        DestroyWindow(fe->statusbar);
    if (midend_wants_statusbar(fe->me)) {
	RECT sr;
	fe->statusbar = CreateWindowEx(0, STATUSCLASSNAME, TEXT("ooh"),
				       WS_CHILD | WS_VISIBLE,
				       0, 0, 0, 0, /* status bar does these */
				       fe->hwnd, NULL, fe->inst, NULL);
#ifdef _WIN32_WCE
	/* Flat status bar looks better on the Pocket PC */
	SendMessage(fe->statusbar, SB_SIMPLE, (WPARAM) TRUE, 0);
	SendMessage(fe->statusbar, SB_SETTEXT,
				(WPARAM) 255 | SBT_NOBORDERS,
				(LPARAM) L"");
#endif

	/*
	 * Now resize the window to take account of the status bar.
	 */
	GetWindowRect(fe->statusbar, &sr);
	GetWindowRect(fe->hwnd, &r);
#ifndef _WIN32_WCE
	SetWindowPos(fe->hwnd, NULL, 0, 0, r.right - r.left,
		     r.bottom - r.top + sr.bottom - sr.top,
		     SWP_NOMOVE | SWP_NOZORDER);
#endif
    } else {
	fe->statusbar = NULL;
    }

    {
        HMENU oldmenu = GetMenu(fe->hwnd);

#ifndef _WIN32_WCE
	HMENU bar = CreateMenu();
	HMENU menu = CreateMenu();
        RECT menusize;

	AppendMenu(bar, MF_ENABLED|MF_POPUP, (UINT)menu, "&Game");
#else
	HMENU menu = SHGetSubMenu(SHFindMenuBar(fe->hwnd), ID_GAME);
	DeleteMenu(menu, 0, MF_BYPOSITION);
#endif
	fe->gamemenu = menu;
	AppendMenu(menu, MF_ENABLED, IDM_NEW, TEXT("&New"));
	AppendMenu(menu, MF_ENABLED, IDM_RESTART, TEXT("&Restart"));
#ifndef _WIN32_WCE
        /* ...here I run out of sensible accelerator characters. */
	AppendMenu(menu, MF_ENABLED, IDM_DESC, TEXT("Speci&fic..."));
	AppendMenu(menu, MF_ENABLED, IDM_SEED, TEXT("Rando&m Seed..."));
#endif

        if (fe->presets)
            sfree(fe->presets);
	if ((fe->npresets = midend_num_presets(fe->me)) > 0 ||
	    fe->game->can_configure) {
	    int i;
#ifndef _WIN32_WCE
	    HMENU sub = CreateMenu();

	    AppendMenu(bar, MF_ENABLED|MF_POPUP, (UINT)sub, "&Type");
#else
	    HMENU sub = SHGetSubMenu(SHFindMenuBar(fe->hwnd), ID_TYPE);
	    DeleteMenu(sub, 0, MF_BYPOSITION);
#endif
	    fe->presets = snewn(fe->npresets, game_params *);

	    for (i = 0; i < fe->npresets; i++) {
		char *name;
#ifdef _WIN32_WCE
		TCHAR wName[255];
#endif

		midend_fetch_preset(fe->me, i, &name, &fe->presets[i]);

		/*
		 * FIXME: we ought to go through and do something
		 * with ampersands here.
		 */

#ifndef _WIN32_WCE
		AppendMenu(sub, MF_ENABLED, IDM_PRESETS + 0x10 * i, name);
#else
		MultiByteToWideChar (CP_ACP, 0, name, -1, wName, 255);
		AppendMenu(sub, MF_ENABLED, IDM_PRESETS + 0x10 * i, wName);
#endif
	    }
	    if (fe->game->can_configure) {
		AppendMenu(sub, MF_ENABLED, IDM_CONFIG, TEXT("&Custom..."));
	    }

	    fe->typemenu = sub;
	} else {
	    fe->typemenu = INVALID_HANDLE_VALUE;
            fe->presets = NULL;
        }

#ifdef COMBINED
#ifdef _WIN32_WCE
#error Windows CE does not support COMBINED build.
#endif
        {
            HMENU games = CreateMenu();
            int i;

            AppendMenu(menu, MF_SEPARATOR, 0, 0);
            AppendMenu(menu, MF_ENABLED|MF_POPUP, (UINT)games, "&Other");
            for (i = 0; i < gamecount; i++) {
                if (strcmp(gamelist[i]->name, fe->game->name) != 0) {
                    /* only include those games that aren't the same as the
                     * game we're currently playing. */
                    AppendMenu(games, MF_ENABLED, IDM_GAMES + i, gamelist[i]->name);
                }
            }
        }
#endif

	AppendMenu(menu, MF_SEPARATOR, 0, 0);
#ifndef _WIN32_WCE
	AppendMenu(menu, MF_ENABLED, IDM_LOAD, TEXT("&Load..."));
	AppendMenu(menu, MF_ENABLED, IDM_SAVE, TEXT("&Save..."));
	AppendMenu(menu, MF_SEPARATOR, 0, 0);
	if (fe->game->can_print) {
	    AppendMenu(menu, MF_ENABLED, IDM_PRINT, TEXT("&Print..."));
	    AppendMenu(menu, MF_SEPARATOR, 0, 0);
	}
#endif
	AppendMenu(menu, MF_ENABLED, IDM_UNDO, TEXT("Undo"));
	AppendMenu(menu, MF_ENABLED, IDM_REDO, TEXT("Redo"));
#ifndef _WIN32_WCE
	if (fe->game->can_format_as_text_ever) {
	    AppendMenu(menu, MF_SEPARATOR, 0, 0);
	    AppendMenu(menu, MF_ENABLED, IDM_COPY, TEXT("&Copy"));
	}
#endif
	if (fe->game->can_solve) {
	    AppendMenu(menu, MF_SEPARATOR, 0, 0);
	    AppendMenu(menu, MF_ENABLED, IDM_SOLVE, TEXT("Sol&ve"));
	}
	AppendMenu(menu, MF_SEPARATOR, 0, 0);
#ifndef _WIN32_WCE
	AppendMenu(menu, MF_ENABLED, IDM_QUIT, TEXT("E&xit"));
	menu = CreateMenu();
	AppendMenu(bar, MF_ENABLED|MF_POPUP, (UINT)menu, TEXT("&Help"));
#endif
	AppendMenu(menu, MF_ENABLED, IDM_ABOUT, TEXT("&About"));
#ifndef _WIN32_WCE
        if (help_type != NONE) {
            char *item;
            AppendMenu(menu, MF_SEPARATOR, 0, 0);
            AppendMenu(menu, MF_ENABLED, IDM_HELPC, TEXT("&Contents"));
            assert(fe->game->name);
            item = snewn(10+strlen(fe->game->name), char); /*ick*/
            sprintf(item, "&Help on %s", fe->game->name);
            AppendMenu(menu, MF_ENABLED, IDM_GAMEHELP, item);
            sfree(item);
        }
        DestroyMenu(oldmenu);
	SetMenu(fe->hwnd, bar);
        get_menu_size(fe->hwnd, &menusize);
        fe->xmin = (menusize.right - menusize.left) + 25;
#endif
    }

    if (fe->bitmap) DeleteObject(fe->bitmap);
    fe->bitmap = NULL;
    new_game_size(fe, fe->puzz_scale); /* initialises fe->bitmap */

    return 0;
}