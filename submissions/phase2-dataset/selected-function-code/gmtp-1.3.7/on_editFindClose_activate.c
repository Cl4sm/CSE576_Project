void on_editFindClose_activate(GtkMenuItem *menuitem, gpointer user_data) {
    gchar* tmp_string;

    gtk_widget_hide(findToolbar);
    if (Preferences.view_folders == TRUE) {
        gtk_widget_show(scrolledwindowFolders);
    }
    fileListClear();
    inFindMode = FALSE;
    gtk_tree_view_column_set_visible(column_Location, FALSE);

    // First we clear the file and folder list...
    fileListClear();
    folderListClear();
    // Refresh the file listings.

    // If using alternate access method, then get our next list of files for the current folder id.
    if (Preferences.use_alt_access_method) {
        filesUpateFileList();
    }

    fileListAdd();
    folderListAdd(deviceFolders, NULL);

    // Update the status bar.
    if (DeviceMgr.storagedeviceID == MTP_DEVICE_SINGLE_STORAGE) {
        tmp_string = g_strdup_printf(_("Connected to %s - %d MB free"), DeviceMgr.devicename->str,
                (int) (DeviceMgr.devicestorage->FreeSpaceInBytes / MEGABYTE));
    } else {
        if (DeviceMgr.devicestorage->StorageDescription != NULL) {
            tmp_string = g_strdup_printf(_("Connected to %s (%s) - %d MB free"), DeviceMgr.devicename->str,
                    DeviceMgr.devicestorage->StorageDescription,
                    (int) (DeviceMgr.devicestorage->FreeSpaceInBytes / MEGABYTE));
        } else {
            tmp_string = g_strdup_printf(_("Connected to %s - %d MB free"), DeviceMgr.devicename->str,
                    (int) (DeviceMgr.devicestorage->FreeSpaceInBytes / MEGABYTE));
        }
    }
    statusBarSet(tmp_string);
    g_free(tmp_string);

    // Now clear the Search GList;
    if (searchList != NULL) {
        g_slist_foreach(searchList, (GFunc) g_free_search, NULL);
        g_slist_free(searchList);
        searchList = NULL;
    }
    //Enable some of the menu options, while in search mode.
    gtk_widget_set_sensitive(GTK_WIDGET(cfileAdd), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(cfileNewFolder), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(fileAdd), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(fileNewFolder), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(toolbuttonAddFile), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(menu_view_folders), !Preferences.use_alt_access_method);

} // end on_editFindClose_activate()
