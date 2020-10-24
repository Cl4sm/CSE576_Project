void on_Playlist_ImportPlaylistButton_activate(GtkMenuItem *menuitem, gpointer user_data) {
    //g_printf("Clicked on new playlist button\n");
    gchar *playlistfilename = NULL;
    gchar *playlistname = NULL;
    GtkWidget *FileDialog;
    GtkFileFilter *OpenFormFilter, *OpenFormFilter2;

    // Save our current selected playlist!
    if (devicePlayLists != NULL)
        playlist_SavePlaylist(playlist_number);

    // Get our filename...

    FileDialog = gtk_file_chooser_dialog_new(_("Select Playlist to Import"),
            GTK_WINDOW(windowMain), GTK_FILE_CHOOSER_ACTION_OPEN,
            _("_Cancel"), GTK_RESPONSE_CANCEL,
            _("_Open"), GTK_RESPONSE_ACCEPT,
            NULL);
    gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(FileDialog), FALSE);
    OpenFormFilter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(OpenFormFilter, "*.m3u");
    gtk_file_filter_set_name(OpenFormFilter, "m3u Playlists");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(FileDialog), OpenFormFilter);
    OpenFormFilter2 = gtk_file_filter_new();
    gtk_file_filter_add_pattern(OpenFormFilter2, "*");
    gtk_file_filter_set_name(OpenFormFilter2, "All Files");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(FileDialog), OpenFormFilter2);

    if (gtk_dialog_run(GTK_DIALOG(FileDialog)) == GTK_RESPONSE_ACCEPT) {
        playlistfilename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(FileDialog));
    }

    gtk_widget_hide(FileDialog);
    gtk_widget_destroy(FileDialog);

    if (playlistfilename != NULL) {
        // Add in playlist to MTP device.
        playlistname = playlistImport(playlistfilename);

        // If our name is NULL, then the import failed...
        if (playlistname != NULL) {
            // Refresh our playlist information.
            devicePlayLists = getPlaylists();
            gtk_list_store_clear(GTK_LIST_STORE(playlist_PL_List));
            // Add it to our combobox

#if HAVE_GTK3 == 0
            gtk_combo_box_append_text(GTK_COMBO_BOX(comboboxentry_playlist), g_strdup(playlistname));
#else
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboboxentry_playlist), g_strdup(playlistname));
#endif

            // Set the active combobox item.
            comboboxentry_playlist_entries++;
            playlist_number = comboboxentry_playlist_entries - 1;
            gtk_combo_box_set_active(GTK_COMBO_BOX(comboboxentry_playlist), comboboxentry_playlist_entries - 1);
            SetPlaylistButtonState(TRUE);
            setPlaylistField(playlist_number);

            // Clean up fields.
            g_free(playlistname);
        } else {
            // Let the user know the import failed.
            g_fprintf(stderr, _("The playlist failed to import correctly.\n"));
            displayError(_("The playlist failed to import correctly.\n"));
        }
        // Clean up fields.
        g_free(playlistfilename);
    }
} // end on_Playlist_ImportPlaylistButton_activate()
