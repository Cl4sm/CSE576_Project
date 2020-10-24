    GtkWidget *dialog, *hbox, *label, *buttonNewPlaylist;
    LIBMTP_playlist_t* tmpplaylist = NULL;
    gint selectedPlaylist = 0;

#if HAVE_GTK3 == 0
    GtkTooltips *tooltips;
    tooltips = gtk_tooltips_new();
#endif

    dialog = gtk_dialog_new_with_buttons(_("Playlists"), GTK_WINDOW(windowMain),
            (GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
            _("_OK"), GTK_RESPONSE_OK,
            _("_Cancel"), GTK_RESPONSE_CANCEL,
            NULL);

    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
#if HAVE_GTK3 == 0
    hbox = gtk_hbox_new(FALSE, 5);
#else 
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
#endif

    gtk_widget_show(hbox);

#if HAVE_GTK3 == 0
    gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), hbox);
#else
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), hbox);
#endif

    // Add in the label
    label = gtk_label_new(_("Playlist Name:"));
    gtk_widget_show(label);
    gtk_container_add(GTK_CONTAINER(hbox), label);
    gtk_misc_set_padding(GTK_MISC(label), 5, 0);

    // Add in the combobox
#if HAVE_GTK3 == 0
    combobox_AddTrackPlaylist = gtk_combo_box_new_text();
#else
    combobox_AddTrackPlaylist = gtk_combo_box_text_new();
#endif
    gtk_widget_show(combobox_AddTrackPlaylist);
    gtk_box_pack_start(GTK_BOX(hbox), combobox_AddTrackPlaylist, TRUE, TRUE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(combobox_AddTrackPlaylist), 5);

    // Add in the new playlist button.
#if HAVE_GTK3 == 0
    buttonNewPlaylist = gtk_button_new_from_stock(GTK_STOCK_ADD);
#else 
    buttonNewPlaylist = gtk_button_new_with_label(_("Add"));
#endif
    if (showNew == TRUE) {
        gtk_widget_show(buttonNewPlaylist);
        gtk_container_add(GTK_CONTAINER(hbox), buttonNewPlaylist);
        gtk_container_set_border_width(GTK_CONTAINER(buttonNewPlaylist), 5);
    }

#if HAVE_GTK3 == 0
    gtk_tooltips_set_tip(tooltips, buttonNewPlaylist, _("Add New Playlist"), NULL);
#else
    gtk_widget_set_tooltip_text(buttonNewPlaylist, _("Add New Playlist"));
#endif

    // Assign the callback for the new playlist button.
    g_signal_connect((gpointer) buttonNewPlaylist, "clicked",
            G_CALLBACK(on_TrackPlaylist_NewPlaylistButton_activate),
            NULL);

    // Populate the combobox with the current playlists.

    // We need to remove all entries in the combo box before starting.
    // This is a little bit of a hack - but does work.
    gtk_list_store_clear(GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(combobox_AddTrackPlaylist))));

    if (devicePlayLists != NULL) {
        // Populate the playlist dropdown box;
        //comboboxentry_playlist;
        tmpplaylist = devicePlayLists;
        while (tmpplaylist != NULL) {
            if (tmpplaylist->storage_id == DeviceMgr.devicestorage->id) {
#if HAVE_GTK3 == 0
                gtk_combo_box_append_text(GTK_COMBO_BOX(combobox_AddTrackPlaylist), g_strdup(tmpplaylist->name));
#else
                gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combobox_AddTrackPlaylist), g_strdup(tmpplaylist->name));
#endif
            }
            tmpplaylist = tmpplaylist->next;
        }
    }
    if (devicePlayLists != NULL) {
        // Set our playlist to the first one.
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_AddTrackPlaylist), 0);
    }

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        // Get our playlist ID.
        selectedPlaylist = gtk_combo_box_get_active(GTK_COMBO_BOX(combobox_AddTrackPlaylist));
        // Now cycle through the playlists to get the correct one.
        tmpplaylist = devicePlayLists;

        if (selectedPlaylist > 0) {
            while (selectedPlaylist--)
                if (tmpplaylist->next != NULL)
                    tmpplaylist = tmpplaylist->next;
        }
        gtk_widget_destroy(dialog);
        return tmpplaylist->playlist_id;
    } else {
        gtk_widget_destroy(dialog);
        return GMTP_NO_PLAYLIST;
    }
}
