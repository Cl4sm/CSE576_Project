gboolean savePreferences() {
#if HAVE_GTK3 == 0
    if (gconf_client_dir_exists(gconfconnect, "/apps/gMTP", NULL) == TRUE) {
        gconf_client_preload(gconfconnect, "/apps/gMTP", GCONF_CLIENT_PRELOAD_ONELEVEL, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/promptDownloadPath", Preferences.ask_download_path, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/autoconnectdevice", Preferences.attemptDeviceConnectOnStart, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/promptOverwriteFile", Preferences.prompt_overwrite_file_op, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/confirmFileDelete", Preferences.confirm_file_delete_op, NULL);
        gconf_client_set_string(gconfconnect, "/apps/gMTP/DownloadPath", Preferences.fileSystemDownloadPath->str, NULL);
        gconf_client_set_string(gconfconnect, "/apps/gMTP/UploadPath", Preferences.fileSystemUploadPath->str, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/autoAddTrackPlaylist", Preferences.auto_add_track_to_playlist, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/ignorepathinplaylist", Preferences.ignore_path_in_playlist_import, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/suppressalbumerrors", Preferences.suppress_album_errors, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/alternateaccessmethod", Preferences.use_alt_access_method, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/allmediaasfiles", Preferences.allmediaasfiles, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/viewFileSize", Preferences.view_size, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/viewFileType", Preferences.view_type, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/viewTrackNumber", Preferences.view_track_number, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/viewTitle", Preferences.view_title, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/viewArtist", Preferences.view_artist, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/viewAlbum", Preferences.view_album, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/viewYear", Preferences.view_year, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/viewGenre", Preferences.view_genre, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/viewDuration", Preferences.view_duration, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/viewFolders", Preferences.view_folders, NULL);
        gconf_client_set_bool(gconfconnect, "/apps/gMTP/viewtoolbar", Preferences.view_toolbar, NULL);
        gconf_client_set_string(gconfconnect, "/apps/gMTP/toolbarstyle", Preferences.toolbarStyle->str, NULL);
    } else {
        g_fprintf(stderr, _("WARNING: gconf schema invalid, unable to save! Please ensure schema is loaded in gconf database.\n"));
    }
    gconf_client_suggest_sync(gconfconnect, NULL);
    gconf_client_clear_cache(gconfconnect);
#else
    if (gsettings_connect != NULL) {
        g_settings_set_boolean(gsettings_connect, "promptdownloadpath", Preferences.ask_download_path);
        g_settings_set_boolean(gsettings_connect, "autoconnectdevice", Preferences.attemptDeviceConnectOnStart);
        g_settings_set_boolean(gsettings_connect, "promptoverwritefile", Preferences.prompt_overwrite_file_op);
        g_settings_set_boolean(gsettings_connect, "confirmfiledelete", Preferences.confirm_file_delete_op);
        g_settings_set_string(gsettings_connect, "downloadpath", Preferences.fileSystemDownloadPath->str);
        g_settings_set_string(gsettings_connect, "uploadpath", Preferences.fileSystemUploadPath->str);
        g_settings_set_boolean(gsettings_connect, "autoaddtrackplaylist", Preferences.auto_add_track_to_playlist);
        g_settings_set_boolean(gsettings_connect, "ignorepathinplaylist", Preferences.ignore_path_in_playlist_import);
        g_settings_set_boolean(gsettings_connect, "suppressalbumerrors", Preferences.suppress_album_errors);
        g_settings_set_boolean(gsettings_connect, "alternateaccessmethod", Preferences.use_alt_access_method);
        g_settings_set_boolean(gsettings_connect, "allmediaasfiles", Preferences.allmediaasfiles);
        g_settings_set_boolean(gsettings_connect, "viewfilesize", Preferences.view_size);
        g_settings_set_boolean(gsettings_connect, "viewfiletype", Preferences.view_type);
        g_settings_set_boolean(gsettings_connect, "viewtracknumber", Preferences.view_track_number);
        g_settings_set_boolean(gsettings_connect, "viewtitle", Preferences.view_title);
        g_settings_set_boolean(gsettings_connect, "viewartist", Preferences.view_artist);
        g_settings_set_boolean(gsettings_connect, "viewalbum", Preferences.view_album);
        g_settings_set_boolean(gsettings_connect, "viewyear", Preferences.view_year);
        g_settings_set_boolean(gsettings_connect, "viewgenre", Preferences.view_genre);
        g_settings_set_boolean(gsettings_connect, "viewduration", Preferences.view_duration);
        g_settings_set_boolean(gsettings_connect, "viewfolders", Preferences.view_folders);
        g_settings_set_boolean(gsettings_connect, "viewtoolbar", Preferences.view_toolbar);
        g_settings_set_string(gsettings_connect, "toolbarstyle", Preferences.toolbarStyle->str);
    }
    g_settings_sync();
#endif
    return TRUE;
}
