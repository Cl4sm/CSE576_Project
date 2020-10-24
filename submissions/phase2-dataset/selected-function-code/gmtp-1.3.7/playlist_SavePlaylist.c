void playlist_SavePlaylist(gint PlayListID) {
    LIBMTP_playlist_t* tmpplaylist = devicePlayLists;
    gint tmpplaylistID = PlayListID;
    gint item_id = 0;
    GtkTreeIter iter;
    uint32_t *tmp = NULL;

    if (PlayListID > 0) {
        while (tmpplaylistID--)
            if (tmpplaylist->next != NULL)
                tmpplaylist = tmpplaylist->next;
    }
    // tmpplaylist points to our playlist;
    // So all we need to do is - update our current structure with the new details

    if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(playlist_PL_List), &iter)) {
        gtk_tree_model_get(GTK_TREE_MODEL(playlist_PL_List), &iter, COL_PL_TRACKID, &item_id, -1);
        tmpplaylist->no_tracks = 1;

        // item_id = our track num... so append to tmpplaylist->tracks
        if ((tmp = g_realloc(tmpplaylist->tracks, sizeof (uint32_t) * (tmpplaylist->no_tracks))) == NULL) {
            g_fprintf(stderr, _("realloc in savePlayList failed\n"));
            displayError(_("Updating playlist failed? 'realloc in savePlayList'\n"));
            return;
        }
        tmpplaylist->tracks = tmp;
        tmpplaylist->tracks[(tmpplaylist->no_tracks - 1)] = item_id;
        //tmpplaylist->no_tracks++;
        while (gtk_tree_model_iter_next(GTK_TREE_MODEL(playlist_PL_List), &iter)) {
            gtk_tree_model_get(GTK_TREE_MODEL(playlist_PL_List), &iter, COL_PL_TRACKID, &item_id, -1);
            tmpplaylist->no_tracks++;
            // item_id = our track num... so append to tmpplaylist->tracks
            if ((tmp = g_realloc(tmpplaylist->tracks, sizeof (uint32_t) * (tmpplaylist->no_tracks))) == NULL) {
                g_fprintf(stderr, _("realloc in savePlayList failed\n"));
                displayError(_("Updating playlist failed? 'realloc in savePlayList'\n"));
                return;
            }
            tmpplaylist->tracks = tmp;
            tmpplaylist->tracks[(tmpplaylist->no_tracks - 1)] = item_id;
            //tmpplaylist->no_tracks++;

        }
    }
    // get libmtp to save it.
    playlistUpdate(tmpplaylist);
    // Update our own metadata.
    devicePlayLists = getPlaylists();
}
