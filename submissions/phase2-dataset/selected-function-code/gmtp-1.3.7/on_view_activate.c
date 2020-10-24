#if HAVE_GTK3 == 0
    gchar *gconf_path = NULL;
    gboolean state = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menuitem));
    // Main menu.
    if ((void *) menuitem == (void *) menu_view_filesize) gconf_path = g_strdup("/apps/gMTP/viewFileSize");
    if ((void *) menuitem == (void *) menu_view_filetype) gconf_path = g_strdup("/apps/gMTP/viewFileType");
    if ((void *) menuitem == (void *) menu_view_track_number) gconf_path = g_strdup("/apps/gMTP/viewTrackNumber");
    if ((void *) menuitem == (void *) menu_view_title) gconf_path = g_strdup("/apps/gMTP/viewTitle");
    if ((void *) menuitem == (void *) menu_view_artist) gconf_path = g_strdup("/apps/gMTP/viewArtist");
    if ((void *) menuitem == (void *) menu_view_album) gconf_path = g_strdup("/apps/gMTP/viewAlbum");
    if ((void *) menuitem == (void *) menu_view_year) gconf_path = g_strdup("/apps/gMTP/viewYear");
    if ((void *) menuitem == (void *) menu_view_genre) gconf_path = g_strdup("/apps/gMTP/viewGenre");
    if ((void *) menuitem == (void *) menu_view_duration) gconf_path = g_strdup("/apps/gMTP/viewDuration");
    if ((void *) menuitem == (void *) menu_view_folders) gconf_path = g_strdup("/apps/gMTP/viewFolders");
    if ((void *) menuitem == (void *) menu_view_toolbar) gconf_path = g_strdup("/apps/gMTP/viewtoolbar");
    // context menu
    if ((void *) menuitem == (void *) cViewSize) gconf_path = g_strdup("/apps/gMTP/viewFileSize");
    if ((void *) menuitem == (void *) cViewType) gconf_path = g_strdup("/apps/gMTP/viewFileType");
    if ((void *) menuitem == (void *) cViewTrackNumber) gconf_path = g_strdup("/apps/gMTP/viewTrackNumber");
    if ((void *) menuitem == (void *) cViewTrackName) gconf_path = g_strdup("/apps/gMTP/viewTitle");
    if ((void *) menuitem == (void *) cViewArtist) gconf_path = g_strdup("/apps/gMTP/viewArtist");
    if ((void *) menuitem == (void *) cViewAlbum) gconf_path = g_strdup("/apps/gMTP/viewAlbum");
    if ((void *) menuitem == (void *) cViewYear) gconf_path = g_strdup("/apps/gMTP/viewYear");
    if ((void *) menuitem == (void *) cViewGenre) gconf_path = g_strdup("/apps/gMTP/viewGenre");
    if ((void *) menuitem == (void *) cViewDuration) gconf_path = g_strdup("/apps/gMTP/viewDuration");

    if ((gconfconnect != NULL) && (gconf_path != NULL)) {
        gconf_client_set_bool(gconfconnect, gconf_path, state, NULL);
        g_free(gconf_path);
    }
#else
    gchar *gsetting_path = NULL;
    gboolean state = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menuitem));
    // main menu
    if ((void *) menuitem == (void *) menu_view_filesize) gsetting_path = g_strdup("viewfilesize");
    if ((void *) menuitem == (void *) menu_view_filetype) gsetting_path = g_strdup("viewfiletype");
    if ((void *) menuitem == (void *) menu_view_track_number) gsetting_path = g_strdup("viewtracknumber");
    if ((void *) menuitem == (void *) menu_view_title) gsetting_path = g_strdup("viewtitle");
    if ((void *) menuitem == (void *) menu_view_artist) gsetting_path = g_strdup("viewartist");
    if ((void *) menuitem == (void *) menu_view_album) gsetting_path = g_strdup("viewalbum");
    if ((void *) menuitem == (void *) menu_view_year) gsetting_path = g_strdup("viewyear");
    if ((void *) menuitem == (void *) menu_view_genre) gsetting_path = g_strdup("viewgenre");
    if ((void *) menuitem == (void *) menu_view_duration) gsetting_path = g_strdup("viewduration");
    if ((void *) menuitem == (void *) menu_view_folders) gsetting_path = g_strdup("viewfolders");
    if ((void *) menuitem == (void *) menu_view_toolbar) gsetting_path = g_strdup("viewtoolbar");
    //context menu.
    if ((void *) menuitem == (void *) cViewSize) gsetting_path = g_strdup("viewfilesize");
    if ((void *) menuitem == (void *) cViewType) gsetting_path = g_strdup("viewfiletype");
    if ((void *) menuitem == (void *) cViewTrackNumber) gsetting_path = g_strdup("viewtracknumber");
    if ((void *) menuitem == (void *) cViewTrackName) gsetting_path = g_strdup("viewtitle");
    if ((void *) menuitem == (void *) cViewArtist) gsetting_path = g_strdup("viewartist");
    if ((void *) menuitem == (void *) cViewAlbum) gsetting_path = g_strdup("viewalbum");
    if ((void *) menuitem == (void *) cViewYear) gsetting_path = g_strdup("viewyear");
    if ((void *) menuitem == (void *) cViewGenre) gsetting_path = g_strdup("viewgenre");
    if ((void *) menuitem == (void *) cViewDuration) gsetting_path = g_strdup("viewduration");

    if ((gsettings_connect != NULL) && (gsetting_path != NULL)) {
        g_settings_set_boolean(gsettings_connect, gsetting_path, state);
        g_settings_sync();
        g_free(gsetting_path);
    }
#endif
} // end on_view_activate()
