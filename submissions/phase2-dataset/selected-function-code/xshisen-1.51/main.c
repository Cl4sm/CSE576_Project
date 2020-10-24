main(int argc, char **argv)
{
    char         *dat_directory;
    char         *scorefile;
    char         *home;
    Display      *disp;
    int          initial_game_state;
    int          num_piece_x, num_piece_y;
    Widget       topform, base;
    XrmDatabase  rdb1, rdb2;

    setlocale(LC_ALL, "");
    XtSetLanguageProc(NULL, NULL, NULL);

    initrandom();
    toplevel = XtVaAppInitialize(&app_context, CLASS_NAME,
                                 options, XtNumber(options),
                                 &argc, argv,
                                 fallback_resources,
                                 XtNminWidth,         MIN_WIN_WID,
                                 XtNminHeight,        MIN_WIN_HEI,
                                 XtNallowShellResize, True,
                                 NULL);
    disp = XtDisplay(toplevel);

    // Override resources with $HOME/.xshisenrc file which is
    // created by Body::WriteRcFile()
    rdb1 = XrmGetDatabase(disp);
    home = getenv("HOME");
    rcfile = new char [strlen(home) + 12];
    sprintf(rcfile, "%s/.xshisenrc", home);
    rdb2 = XrmGetFileDatabase(rcfile);
    XrmMergeDatabases(rdb2, &rdb1);
    XrmSetDatabase(disp, rdb1);
    XtVaGetApplicationResources(toplevel, (XtPointer)&globRes,
                                gres, XtNumber(gres), NULL);

    if (globRes.kanjiConv) {
        kanjiconvert(argv[1]);
        exit(0);
    }
    if (argc != 1)
        usage(argv[0]);

    XtAddEventHandler(toplevel, StructureNotifyMask, False,
                      (XtEventHandler)statewatcher,
                      (Opaque)NULL);
#if USE_EDITRES
    XtAddEventHandler(toplevel, (EventMask)0, True,
                      (XtEventHandler)_XEditResCheckMessages,
                      (Opaque)NULL);
#endif
    XtAppAddActions(app_context, actions, XtNumber(actions));

    globRes.Width  = (int)(globRes.Width  * globRes.magFactor);
    globRes.Height = (int)(globRes.Height * globRes.magFactor);

    initial_game_state = globRes.gameSize;
    if (globRes.trialMode)
        initial_game_state += NUM_GAME;
    else if (globRes.gravityMode)
        initial_game_state += 2*NUM_GAME;
    idletime.tv_sec  = 0;
    idletime.tv_usec = globRes.idleTime;
    max_win_wid = DisplayWidth (disp, XDefaultScreen(disp));
    max_win_hei = DisplayHeight(disp, XDefaultScreen(disp));
    XtVaSetValues(toplevel,
                  XtNmaxWidth,         max_win_wid,
                  XtNmaxHeight,        max_win_hei,
                  NULL);

    if ((dat_directory = getenv("XSHISENDAT")) == NULL)
        dat_directory = globRes.scoreDir;

    sc = new Score(toplevel);
    scorefile = new char[strlen(dat_directory)+strlen(globRes.scoreFile)+1];
    sprintf(scorefile, globRes.scoreFile, dat_directory);
    sc->SetScoreFile(scorefile, globRes.kanjiCode, globRes.personalScoreFile);
    delete[] scorefile;

    if (globRes.scoreOnly) {
#ifndef NO_GLOBAL_HIGHSCORE
        sc->DisplayScore(initial_game_state);
#if USE_MOTIF
        XtAddCallback(*sc, XmNokCallback, (XtCallbackProc)ExitCB, NULL);
#else
        XtAddCallback(XtNameToWidget(*sc, "*ok_button"),
                      XtNcallback, (XtCallbackProc)ExitCB, NULL);
#endif
#else /* NO_GLOBAL_HIGHSCORE */
	fprintf(stderr, "System score files not enabled.\n");
	exit(1);
#endif /* NO_GLOBAL_HIGHSCORE */
    }
    else {
        GetGameSize(initial_game_state, num_piece_x, num_piece_y);
#if USE_MOTIF
        topform = XtVaCreateManagedWidget("form", xmFormWidgetClass, toplevel,
                                          NULL);
        mb = new MenuBar(topform, initial_game_state % NUM_GAME,
                         initial_game_state / NUM_GAME, globRes.imageSet - 1);
        base = topform;
#else
        topform = XtVaCreateManagedWidget("form", formWidgetClass, toplevel,
                                          XtNdefaultDistance, 0,
                                          NULL);
        mb = new MenuBar(topform, initial_game_state % NUM_GAME,
                         initial_game_state / NUM_GAME, globRes.imageSet - 1);
        base = XtVaCreateManagedWidget("sform", formWidgetClass, topform,
                                       XtNfromVert,        (Widget)*mb,
                                       XtNleft,            XawChainLeft,
                                       XtNright,           XawChainRight,
                                       XtNtop,             XawChainTop,
                                       XtNbottom,          XawChainBottom,
                                       XtNresizable,       True,
                                       XtNborderWidth,     0,
                                       XtNdefaultDistance, 0,
                                       NULL);
#endif
        tm = new TimerW(base, *mb,
                        globRes.displayFormat1, "000",
                        globRes.displayFormat2, "00:00:00");
        bd = new Body(initial_game_state, num_piece_x, num_piece_y, base, *tm);
        workproc_id = XtAppAddWorkProc(app_context, (XtWorkProc)refreshtimer, NULL);
        XtRealizeWidget(toplevel);
        // InitGlobalMahjong must be called after realized because it
        // requires the real Window.
        InitPicture();
        seticonwindow();
        bd->SetGC();
        GetBoardSizeFromGameSize(num_piece_x, num_piece_y,
                                 globRes.Width, globRes.Height);
        bd->SetGeometry(globRes.Width, globRes.Height);
        SetGameStart();
    }

    XtAppMainLoop(app_context);
}
