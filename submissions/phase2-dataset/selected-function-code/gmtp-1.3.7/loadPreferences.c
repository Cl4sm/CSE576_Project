gboolean loadPreferences() {
#if HAVE_GTK3 == 0
    if (gconf_client_dir_exists(gconfconnect, "/apps/gMTP", NULL) == TRUE) {
        gconf_client_preload(gconfconnect, "/apps/gMTP", GCONF_CLIENT_PRELOAD_ONELEVEL, NULL);
        Preferences.ask_download_path = gconf_client_get_bool(gconfconnect, "/apps/gMTP/promptDownloadPath", NULL);
        Preferences.confirm_file_delete_op = gconf_client_get_bool(gconfconnect, "/apps/gMTP/confirmFileDelete", NULL);
        Preferences.prompt_overwrite_file_op = gconf_client_get_bool(gconfconnect, "/apps/gMTP/promptOverwriteFile", NULL);
        Preferences.attemptDeviceConnectOnStart = gconf_client_get_bool(gconfconnect, "/apps/gMTP/autoconnectdevice", NULL);
        Preferences.fileSystemDownloadPath = g_string_new(gconf_client_get_string(gconfconnect, "/apps/gMTP/DownloadPath", NULL));
        Preferences.fileSystemUploadPath = g_string_new(gconf_client_get_string(gconfconnect, "/apps/gMTP/UploadPath", NULL));
        Preferences.auto_add_track_to_playlist = gconf_client_get_bool(gconfconnect, "/apps/gMTP/autoAddTrackPlaylist", NULL);
        Preferences.ignore_path_in_playlist_import = gconf_client_get_bool(gconfconnect, "/apps/gMTP/ignorepathinplaylist", NULL);
        Preferences.suppress_album_errors = gconf_client_get_bool(gconfconnect, "/apps/gMTP/suppressalbumerrors", NULL);
        Preferences.use_alt_access_method = gconf_client_get_bool(gconfconnect, "/apps/gMTP/alternateaccessmethod", NULL);
        Preferences.allmediaasfiles = gconf_client_get_bool(gconfconnect, "/apps/gMTP/allmediaasfiles", NULL);
        Preferences.view_size = gconf_client_get_bool(gconfconnect, "/apps/gMTP/viewFileSize", NULL);
        Preferences.view_type = gconf_client_get_bool(gconfconnect, "/apps/gMTP/viewFileType", NULL);
        Preferences.view_track_number = gconf_client_get_bool(gconfconnect, "/apps/gMTP/viewTrackNumber", NULL);
        Preferences.view_title = gconf_client_get_bool(gconfconnect, "/apps/gMTP/viewTitle", NULL);
        Preferences.view_artist = gconf_client_get_bool(gconfconnect, "/apps/gMTP/viewArtist", NULL);
        Preferences.view_album = gconf_client_get_bool(gconfconnect, "/apps/gMTP/viewAlbum", NULL);
        Preferences.view_year = gconf_client_get_bool(gconfconnect, "/apps/gMTP/viewYear", NULL);
        Preferences.view_genre = gconf_client_get_bool(gconfconnect, "/apps/gMTP/viewGenre", NULL);
        Preferences.view_duration = gconf_client_get_bool(gconfconnect, "/apps/gMTP/viewDuration", NULL);
        Preferences.view_folders = gconf_client_get_bool(gconfconnect, "/apps/gMTP/viewFolders", NULL);
        Preferences.view_toolbar = gconf_client_get_bool(gconfconnect, "/apps/gMTP/viewtoolbar", NULL);
        Preferences.toolbarStyle = g_string_new(gconf_client_get_string(gconfconnect, "/apps/gMTP/toolbarstyle", NULL));
    } else {
        g_fprintf(stderr, _("WARNING: gconf schema invalid, reverting to defaults. Please ensure schema is loaded in gconf database.\n"));
    }
    gconf_client_clear_cache(gconfconnect);
#else
    if (gsettings_connect != NULL) {
        Preferences.ask_download_path = g_settings_get_boolean(gsettings_connect, "promptdownloadpath");
        Preferences.confirm_file_delete_op = g_settings_get_boolean(gsettings_connect, "confirmfiledelete");
        Preferences.prompt_overwrite_file_op = g_settings_get_boolean(gsettings_connect, "promptoverwritefile");
        Preferences.attemptDeviceConnectOnStart = g_settings_get_boolean(gsettings_connect, "autoconnectdevice");
        Preferences.fileSystemDownloadPath = g_string_new(g_settings_get_string(gsettings_connect, "downloadpath"));
        Preferences.fileSystemUploadPath = g_string_new(g_settings_get_string(gsettings_connect, "uploadpath"));
        Preferences.auto_add_track_to_playlist = g_settings_get_boolean(gsettings_connect, "autoaddtrackplaylist");
        Preferences.ignore_path_in_playlist_import = g_settings_get_boolean(gsettings_connect, "ignorepathinplaylist");
        Preferences.suppress_album_errors = g_settings_get_boolean(gsettings_connect, "suppressalbumerrors");
        Preferences.use_alt_access_method = g_settings_get_boolean(gsettings_connect, "alternateaccessmethod");
        Preferences.allmediaasfiles = g_settings_get_boolean(gsettings_connect, "allmediaasfiles");
        Preferences.view_size = g_settings_get_boolean(gsettings_connect, "viewfilesize");
        Preferences.view_type = g_settings_get_boolean(gsettings_connect, "viewfiletype");
        Preferences.view_track_number = g_settings_get_boolean(gsettings_connect, "viewtracknumber");
        Preferences.view_title = g_settings_get_boolean(gsettings_connect, "viewtitle");
        Preferences.view_artist = g_settings_get_boolean(gsettings_connect, "viewartist");
        Preferences.view_album = g_settings_get_boolean(gsettings_connect, "viewalbum");
        Preferences.view_year = g_settings_get_boolean(gsettings_connect, "viewyear");
        Preferences.view_genre = g_settings_get_boolean(gsettings_connect, "viewgenre");
        Preferences.view_duration = g_settings_get_boolean(gsettings_connect, "viewduration");
        Preferences.view_folders = g_settings_get_boolean(gsettings_connect, "viewfolders");
        Preferences.view_toolbar = g_settings_get_boolean(gsettings_connect, "viewtoolbar");
        Preferences.toolbarStyle = g_string_new(g_settings_get_string(gsettings_connect, "toolbarstyle"));
    }
#endif
    // Set some menu options and view states.
    gtk_tree_view_column_set_visible(column_Size, Preferences.view_size);
    gtk_tree_view_column_set_visible(column_Type, Preferences.view_type);
    gtk_tree_view_column_set_visible(column_Track_Number, Preferences.view_track_number);
    gtk_tree_view_column_set_visible(column_Title, Preferences.view_title);
    gtk_tree_view_column_set_visible(column_Artist, Preferences.view_artist);
    gtk_tree_view_column_set_visible(column_Album, Preferences.view_album);
    gtk_tree_view_column_set_visible(column_Year, Preferences.view_year);
    gtk_tree_view_column_set_visible(column_Genre, Preferences.view_genre);
    gtk_tree_view_column_set_visible(column_Duration, Preferences.view_duration);

    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_filesize), Preferences.view_size);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_filetype), Preferences.view_type);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_track_number), Preferences.view_track_number);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_title), Preferences.view_title);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_artist), Preferences.view_artist);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_album), Preferences.view_album);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_year), Preferences.view_year);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_genre), Preferences.view_genre);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_duration), Preferences.view_duration);
    
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_folders), Preferences.view_folders);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_toolbar), Preferences.view_toolbar);

    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewSize), Preferences.view_size);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewType), Preferences.view_type);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewTrackNumber), Preferences.view_track_number);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewTrackName), Preferences.view_title);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewArtist), Preferences.view_artist);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewAlbum), Preferences.view_album);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewYear), Preferences.view_year);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewGenre), Preferences.view_genre);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewDuration), Preferences.view_duration);

    // Disable the folder view if in alt access mode...
    if (Preferences.use_alt_access_method) {
        if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menu_view_folders))) {
            gtk_menu_item_activate(GTK_MENU_ITEM(menu_view_folders));
        }
        gtk_widget_hide(scrolledwindowFolders);
        gtk_widget_set_sensitive(menu_view_folders, !Preferences.use_alt_access_method);
    }
    
    // Disable the use track information in the find field.
    if (Preferences.allmediaasfiles == TRUE || Preferences.use_alt_access_method == TRUE){
        // Disable the find track meta data in findToolbar.
        gtk_widget_set_sensitive(GTK_WIDGET(FindToolbar_checkbutton_TrackInformation), FALSE);
    } else {
        gtk_widget_set_sensitive(GTK_WIDGET(FindToolbar_checkbutton_TrackInformation), TRUE);
    }
    
    // Show/hide the toolbar, and set the style.
    if(handlebox1 != NULL){
        if(Preferences.view_toolbar){
            gtk_widget_show(GTK_WIDGET(handlebox1));
        } else {
            gtk_widget_hide(GTK_WIDGET(handlebox1));
        }
    }
    if(toolbarMain != NULL){
        if(g_ascii_strcasecmp(Preferences.toolbarStyle->str, "icon") == 0){
            gtk_toolbar_set_style(GTK_TOOLBAR(toolbarMain), GTK_TOOLBAR_ICONS);
        } else if(g_ascii_strcasecmp(Preferences.toolbarStyle->str, "text") == 0){
            gtk_toolbar_set_style(GTK_TOOLBAR(toolbarMain), GTK_TOOLBAR_TEXT);
        } else if(g_ascii_strcasecmp(Preferences.toolbarStyle->str, "both") == 0){
            gtk_toolbar_set_style(GTK_TOOLBAR(toolbarMain), GTK_TOOLBAR_BOTH);
        }
    }
    return TRUE;
}
