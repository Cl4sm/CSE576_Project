    GtkWidget *FileDialog;
    gchar *savepath = NULL;
    //savepath = g_malloc0(8192);

    // Let's confirm our download path.
    if (Preferences.ask_download_path == TRUE) {
        FileDialog = gtk_file_chooser_dialog_new(_("Select Path to Download"),
                GTK_WINDOW(windowMain), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                _("_Cancel"), GTK_RESPONSE_CANCEL,
                _("_Open"), GTK_RESPONSE_ACCEPT,
                NULL);

        gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(FileDialog), Preferences.fileSystemDownloadPath->str);
        if (gtk_dialog_run(GTK_DIALOG(FileDialog)) == GTK_RESPONSE_ACCEPT) {
            gtk_widget_hide(FileDialog);
            savepath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(FileDialog));
            // Save our download path.
            Preferences.fileSystemDownloadPath = g_string_assign(Preferences.fileSystemDownloadPath, savepath);
            // We do the deed.
            displayProgressBar(_("File download"));
            folderDownload(foldername, folderid, TRUE);
            destroyProgressBar();
            fileoverwriteop = MTP_ASK;
        }
        gtk_widget_destroy(FileDialog);
    } else {
        // We do the deed.
        displayProgressBar(_("File download"));
        folderDownload(foldername, folderid, TRUE);
        destroyProgressBar();
        fileoverwriteop = MTP_ASK;
    }
    if (savepath != NULL)
        g_free(savepath);
    return TRUE;
}
