void __playlist_TrackList_Add(GtkTreeRowReference *Row) {
    GtkTreePath *path = NULL;
    GtkTreeIter iter;
    GtkTreeIter PL_rowIter;
    gchar* artist = NULL;
    gchar* album = NULL;
    gchar* title = NULL;
    gint item_id = 0;
    gchar * duration = NULL;

    // convert the referenece to a path and retrieve the iterator;
    path = gtk_tree_row_reference_get_path(Row);
    gtk_tree_model_get_iter(GTK_TREE_MODEL(playlist_TrackList), &iter, path);
    // We have our Iter now, so get the required information from the track treeview.
    gtk_tree_model_get(GTK_TREE_MODEL(playlist_TrackList), &iter, COL_ARTIST, &artist, COL_ALBUM, &album,
            COL_TRACKID, &item_id, COL_TRACKNAME, &title, COL_TRACKDURATION, & duration, -1);
    // Now store our information in the playlist treeview.
    playlist_track_count++;
    gtk_list_store_append(GTK_LIST_STORE(playlist_PL_List), &PL_rowIter);
    gtk_list_store_set(GTK_LIST_STORE(playlist_PL_List), &PL_rowIter, COL_PL_ORDER_NUM, playlist_track_count, COL_PL_ARTIST, artist,
            COL_PL_ALBUM, album, COL_PL_TRACKID, item_id, COL_PL_TRACKNAME, title, COL_PL_TRACKDURATION, duration, -1);

    //Need to free our string values
    g_free(artist);
    g_free(album);
    g_free(title);
    g_free(duration);
}
