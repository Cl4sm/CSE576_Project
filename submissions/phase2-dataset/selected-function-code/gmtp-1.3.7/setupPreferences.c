    // We setup default Preferences.
    Preferences.ask_download_path = TRUE;
    Preferences.attemptDeviceConnectOnStart = FALSE;
    Preferences.suppress_album_errors = FALSE;
    Preferences.view_toolbar = TRUE;
    Preferences.toolbarStyle = g_string_new(g_getenv("both"));
#ifdef WIN32
    Preferences.fileSystemDownloadPath = g_string_new(g_getenv("HOMEPATH"));
    Preferences.fileSystemUploadPath = g_string_new(g_getenv("HOMEPATH"));
#else
    Preferences.fileSystemDownloadPath = g_string_new(g_getenv("HOME"));
    Preferences.fileSystemUploadPath = g_string_new(g_getenv("HOME"));
#endif
    // Now setup our gconf/gsettings callbacks;
#if HAVE_GTK3 == 0
    if (gconfconnect == NULL)
        gconfconnect = gconf_client_get_default();
    if (gconf_client_dir_exists(gconfconnect, "/apps/gMTP", NULL) == TRUE) {
        gconf_client_add_dir(gconfconnect, "/apps/gMTP", GCONF_CLIENT_PRELOAD_ONELEVEL, NULL);
        gconf_callback_id = gconf_client_notify_add(gconfconnect, "/apps/gMTP", (GConfClientNotifyFunc) gconf_callback_func, NULL, NULL, NULL);
    }
#else
    gsettings_connect = g_settings_new(GMTP_GSETTINGS_SCHEMA);
    g_signal_connect((gpointer) gsettings_connect, "changed",
            G_CALLBACK(gsettings_callback_func),
            NULL);
#endif
    // Now attempt to read the config file from the user config folder.
    loadPreferences();
}
