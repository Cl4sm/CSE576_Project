void __fileRemoveFromPlaylist(GtkTreeRowReference *Row, LIBMTP_playlist_t **playlist) {
    GtkTreePath *path;
    GtkTreeIter iter;
    uint32_t objectID;
    gboolean isFolder;
    LIBMTP_track_t *tracks = deviceTracks;
    LIBMTP_track_t *node = NULL;
    // convert the referenece to a path and retrieve the iterator;
    path = gtk_tree_row_reference_get_path(Row);
    gtk_tree_model_get_iter(GTK_TREE_MODEL(fileList), &iter, path);
    // We have our Iter now.
    gtk_tree_model_get(GTK_TREE_MODEL(fileList), &iter, COL_ISFOLDER, &isFolder, COL_FILEID, &objectID, -1);
    if (isFolder == FALSE) {
        // Now add the file to the playlist.
        // We need the playlist pointer, and the **track** pointer;
        while (tracks != NULL) {
            if (tracks->item_id == objectID) {
                node = tracks;
                tracks = NULL;
            } else {
                tracks = tracks->next;
            }
        }
        if (node != NULL) {
            playlistRemoveTrack(*(playlist), node, MTP_PLAYLIST_FIRST_INSTANCE);
        }
    }
}
