void gconf_callback_func(GConfClient *client, guint cnxn_id, GConfEntry *entry, gpointer user_data) {
    //g_printf("Gconf callback - %s\n", entry->key);
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/promptDownloadPath") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.ask_download_path = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        //g_printf("/apps/gMTP/promptDownloadPath = %d\n", Preferences.ask_download_path );
        if (windowPrefsDialog != NULL) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonDownloadPath), Preferences.ask_download_path);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/autoconnectdevice") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.attemptDeviceConnectOnStart = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        //g_printf("/apps/gMTP/autoconnectdevice = %d\n", Preferences.attemptDeviceConnectOnStart );
        if (windowPrefsDialog != NULL) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonDeviceConnect), Preferences.attemptDeviceConnectOnStart);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/promptOverwriteFile") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.prompt_overwrite_file_op = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        //g_printf("/apps/gMTP/promptOverwriteFile = %d\n", Preferences.prompt_overwrite_file_op );
        if (windowPrefsDialog != NULL) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonConfirmOverWriteFileOp), Preferences.prompt_overwrite_file_op);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/confirmFileDelete") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.confirm_file_delete_op = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        //g_printf("/apps/gMTP/confirmFileDelete = %d\n", Preferences.confirm_file_delete_op );
        if (windowPrefsDialog != NULL) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonConfirmFileOp), Preferences.confirm_file_delete_op);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/DownloadPath") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.fileSystemDownloadPath = g_string_assign(Preferences.fileSystemDownloadPath, gconf_value_to_string((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry)));
        //g_printf("/apps/gMTP/DownloadPath = %s\n", Preferences.fileSystemDownloadPath->str );
        if (windowPrefsDialog != NULL) gtk_entry_set_text(GTK_ENTRY(entryDownloadPath), Preferences.fileSystemDownloadPath->str);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/UploadPath") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.fileSystemUploadPath = g_string_assign(Preferences.fileSystemUploadPath, gconf_value_to_string((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry)));
        //g_printf("/apps/gMTP/UploadPath = %s\n", Preferences.fileSystemUploadPath->str );
        if (windowPrefsDialog != NULL) gtk_entry_set_text(GTK_ENTRY(entryUploadPath), Preferences.fileSystemUploadPath->str);
        return;
    }
    // View menu Options.
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/viewFileSize") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.view_size = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        gtk_tree_view_column_set_visible(column_Size, Preferences.view_size);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_filesize), Preferences.view_size);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewSize), Preferences.view_size);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/viewFileType") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.view_type = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        gtk_tree_view_column_set_visible(column_Type, Preferences.view_type);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_filetype), Preferences.view_type);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewType), Preferences.view_type);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/viewTrackNumber") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.view_track_number = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        gtk_tree_view_column_set_visible(column_Track_Number, Preferences.view_track_number);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_track_number), Preferences.view_track_number);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewTrackNumber), Preferences.view_track_number);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/viewTitle") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.view_title = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        gtk_tree_view_column_set_visible(column_Title, Preferences.view_title);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_title), Preferences.view_title);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewTrackName), Preferences.view_title);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/viewArtist") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.view_artist = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        gtk_tree_view_column_set_visible(column_Artist, Preferences.view_artist);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_artist), Preferences.view_artist);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewArtist), Preferences.view_artist);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/viewAlbum") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.view_album = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        gtk_tree_view_column_set_visible(column_Album, Preferences.view_album);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_album), Preferences.view_album);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewAlbum), Preferences.view_album);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/viewYear") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.view_year = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        gtk_tree_view_column_set_visible(column_Year, Preferences.view_year);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_year), Preferences.view_year);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewYear), Preferences.view_year);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/viewGenre") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.view_genre = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        gtk_tree_view_column_set_visible(column_Genre, Preferences.view_genre);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_genre), Preferences.view_genre);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewGenre), Preferences.view_genre);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/viewDuration") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.view_duration = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        gtk_tree_view_column_set_visible(column_Duration, Preferences.view_duration);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_duration), Preferences.view_duration);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(cViewDuration), Preferences.view_duration);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/viewFolders") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.view_folders = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_view_folders), Preferences.view_folders);

        if (!Preferences.use_alt_access_method) {
            if (Preferences.view_folders == TRUE) {
                gtk_widget_show(scrolledwindowFolders);
            } else {
                gtk_widget_hide(scrolledwindowFolders);
            }
        } else {
            // hide the folder view
            gtk_widget_hide(scrolledwindowFolders);
        }
        gtk_widget_set_sensitive(menu_view_folders, !Preferences.use_alt_access_method);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/autoAddTrackPlaylist") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.auto_add_track_to_playlist = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        if (windowPrefsDialog != NULL) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonAutoAddTrackPlaylist), Preferences.auto_add_track_to_playlist);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/ignorepathinplaylist") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.ignore_path_in_playlist_import = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        if (windowPrefsDialog != NULL) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonIgnorePathInPlaylist), Preferences.ignore_path_in_playlist_import);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/suppressalbumerrors") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.suppress_album_errors = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        if (windowPrefsDialog != NULL) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonSuppressAlbumErrors), Preferences.suppress_album_errors);
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/alternateaccessmethod") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.use_alt_access_method = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        if (windowPrefsDialog != NULL) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonAltAccessMethod), Preferences.use_alt_access_method);
        // if we are connected, then disconnect the device...
        if (DeviceMgr.deviceConnected == TRUE) {
            on_deviceConnect_activate(NULL, NULL);
            displayInformation(_("Disconnected device due to access method change"));
        }
        // Disable the folder view...
        if (Preferences.use_alt_access_method) {
            if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menu_view_folders))) {
                gtk_menu_item_activate(GTK_MENU_ITEM(menu_view_folders));
            }
        }
        gtk_widget_set_sensitive(menu_view_folders, !Preferences.use_alt_access_method);
        if (Preferences.allmediaasfiles == TRUE || Preferences.use_alt_access_method == TRUE){
            // Disable the find track meta data in findToolbar.
            gtk_widget_set_sensitive(GTK_WIDGET(FindToolbar_checkbutton_TrackInformation), FALSE);
        } else {
            gtk_widget_set_sensitive(GTK_WIDGET(FindToolbar_checkbutton_TrackInformation), TRUE);
        }
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/allmediaasfiles") == 0) {
        //set our all media as files preferences
        Preferences.allmediaasfiles = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        if (windowPrefsDialog != NULL) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonAllMediaAsFiles), Preferences.allmediaasfiles);
        if (Preferences.allmediaasfiles == TRUE || Preferences.use_alt_access_method == TRUE){
            // Disable the find track meta data in findToolbar.
            gtk_widget_set_sensitive(GTK_WIDGET(FindToolbar_checkbutton_TrackInformation), FALSE);
        } else {
            gtk_widget_set_sensitive(GTK_WIDGET(FindToolbar_checkbutton_TrackInformation), TRUE);
        }
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/viewtoolbar") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.view_toolbar = (gboolean) gconf_value_get_bool((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry));
        //if (windowPrefsDialog != NULL) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonSuppressAlbumErrors), Preferences.suppress_album_errors);
        if(Preferences.view_toolbar){
            gtk_widget_show(GTK_WIDGET(handlebox1));
        } else {
            gtk_widget_hide(GTK_WIDGET(handlebox1));
        }       
        return;
    }
    if (g_ascii_strcasecmp(entry->key, "/apps/gMTP/toolbarstyle") == 0) {
        //set our promptDownloadPath in Preferences
        Preferences.toolbarStyle = g_string_assign(Preferences.toolbarStyle, gconf_value_to_string((const GConfValue*) gconf_entry_get_value((const GConfEntry*) entry)));
        if(g_ascii_strcasecmp(Preferences.toolbarStyle->str, "icon") == 0){
            gtk_toolbar_set_style(GTK_TOOLBAR(toolbarMain), GTK_TOOLBAR_ICONS);
        } else if(g_ascii_strcasecmp(Preferences.toolbarStyle->str, "text") == 0){
            gtk_toolbar_set_style(GTK_TOOLBAR(toolbarMain), GTK_TOOLBAR_TEXT);
        } else if(g_ascii_strcasecmp(Preferences.toolbarStyle->str, "both") == 0){
            gtk_toolbar_set_style(GTK_TOOLBAR(toolbarMain), GTK_TOOLBAR_BOTH);
        }
        return;
    }
    g_fprintf(stderr, _("WARNING: gconf_callback_func() failed - we got a callback for a key thats not ours?\n"));
}
