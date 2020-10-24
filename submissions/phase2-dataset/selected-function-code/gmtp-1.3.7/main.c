    setFilePaths(argc, argv);

#if GLIB_CHECK_VERSION(2,32,0)
    // only need g_thread_init on versions less than 2.32
#else
    g_thread_init(NULL);
#endif

#if HAVE_GTK3 == 0
    gtk_set_locale();
#endif
    g_set_prgname(PACKAGE_NAME);
    g_set_application_name(PACKAGE_NAME);

    gtk_init(&argc, &argv);
    
#ifdef ENABLE_NLS
    bindtextdomain(PACKAGE_NAME, g_strconcat(applicationpath, "/../share/locale", NULL));
    bind_textdomain_codeset(PACKAGE_NAME, "UTF-8");
    textdomain(PACKAGE_NAME);
#endif

    // Initialise libmtp library
    LIBMTP_Init();

    // Create our main window for the application.
    windowMain = create_windowMain();
    gtk_widget_show(windowMain);

    //Set default state for application
    DeviceMgr.deviceConnected = FALSE;
    statusBarSet(_("No device attached"));
    SetToolbarButtonState(DeviceMgr.deviceConnected);

    setupPreferences();

    // If preference is to auto-connect then attempt to do so.
    if (Preferences.attemptDeviceConnectOnStart == TRUE)
        on_deviceConnect_activate(NULL, NULL);

    // If we do have a connected device, then do a rescan operation to fill in the filelist.
    if (DeviceMgr.deviceConnected == TRUE)
        deviceRescan();

    gtk_main();

    return EXIT_SUCCESS;
} // end main()
