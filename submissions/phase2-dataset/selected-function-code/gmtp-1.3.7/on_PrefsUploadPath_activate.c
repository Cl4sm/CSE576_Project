    // What we do here is display a find folder dialog, and save the resulting folder into the text wigdet and preferences item.
    gchar *savepath = NULL;
    GtkWidget *FileDialog;
    // First of all, lets set the upload path.
    FileDialog = gtk_file_chooser_dialog_new(_("Select Path to Upload From"),
            GTK_WINDOW(windowPrefsDialog), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
            _("_Cancel"), GTK_RESPONSE_CANCEL,
            _("_Open"), GTK_RESPONSE_ACCEPT,
            NULL);
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(FileDialog), Preferences.fileSystemUploadPath->str);
    if (gtk_dialog_run(GTK_DIALOG(FileDialog)) == GTK_RESPONSE_ACCEPT) {
        savepath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(FileDialog));

        // Save our download path.
#if HAVE_GTK3 == 0
        if (gconfconnect != NULL)
            gconf_client_set_string(gconfconnect, "/apps/gMTP/UploadPath", savepath, NULL);
#else
        if (gsettings_connect != NULL)
            g_settings_set_string(gsettings_connect, "uploadpath", savepath);
        g_settings_sync();
#endif
        g_free(savepath);
    }
    gtk_widget_destroy(FileDialog);
} //on_PrefsUploadPath_activate()
