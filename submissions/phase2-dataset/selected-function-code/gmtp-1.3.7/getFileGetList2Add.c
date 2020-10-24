    GSList* files = NULL;
    GtkWidget *FileDialog;
    gchar *savepath = NULL;

    savepath = g_malloc0(8192);
    FileDialog = gtk_file_chooser_dialog_new(_("Select Files to Add"),
            GTK_WINDOW(windowMain), GTK_FILE_CHOOSER_ACTION_OPEN,
            _("_Cancel"), GTK_RESPONSE_CANCEL,
            _("_Open"), GTK_RESPONSE_ACCEPT,
            NULL);

    gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(FileDialog), TRUE);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(FileDialog), Preferences.fileSystemUploadPath->str);
    if (gtk_dialog_run(GTK_DIALOG(FileDialog)) == GTK_RESPONSE_ACCEPT) {
        savepath = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(FileDialog));
        // Save our upload path.
        Preferences.fileSystemUploadPath = g_string_assign(Preferences.fileSystemUploadPath, savepath);
        files = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(FileDialog));
    }

    gtk_widget_hide(FileDialog);
    gtk_widget_destroy(FileDialog);
    g_free(savepath);
    return files;
}
