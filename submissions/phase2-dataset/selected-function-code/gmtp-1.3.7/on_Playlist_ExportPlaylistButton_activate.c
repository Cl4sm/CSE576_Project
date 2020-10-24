void on_Playlist_ExportPlaylistButton_activate(GtkMenuItem *menuitem, gpointer user_data) {
    gchar *playlistfilename = NULL;
    GtkWidget *FileDialog;

    // Save our current selected playlist!
    if (devicePlayLists != NULL)
        playlist_SavePlaylist(playlist_number);

    gint PlayListID = gtk_combo_box_get_active(GTK_COMBO_BOX(comboboxentry_playlist));

    if (PlayListID != -1) {
        // We have something selected so lets do the dance.
        LIBMTP_playlist_t* tmpplaylist = devicePlayLists;
        if (PlayListID > 0) {
            while (PlayListID--)
                if (tmpplaylist->next != NULL)
                    tmpplaylist = tmpplaylist->next;
        }
        // We should be in the correct playlist LIBMTP structure.

        playlistfilename = g_strdup_printf("%s.%s", tmpplaylist->name, "m3u");

        FileDialog = gtk_file_chooser_dialog_new(_("Save as..."),
                GTK_WINDOW(windowMain), GTK_FILE_CHOOSER_ACTION_SAVE,
                _("_Cancel"), GTK_RESPONSE_CANCEL,
                _("_Open"), GTK_RESPONSE_ACCEPT,
                NULL);
        gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(FileDialog), playlistfilename);

        if (gtk_dialog_run(GTK_DIALOG(FileDialog)) == GTK_RESPONSE_ACCEPT) {
            playlistfilename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(FileDialog));
        }

        gtk_widget_hide(FileDialog);
        gtk_widget_destroy(FileDialog);

        if (playlistfilename != NULL) {
            playlistExport(playlistfilename, tmpplaylist);
            g_free(playlistfilename);
        }
    }
} // end on_Playlist_ExportPlaylistButton_activate()
