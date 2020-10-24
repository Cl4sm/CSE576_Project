gboolean playlist_move_files(gint direction) {
    GList * playlist_files = NULL;
    GtkTreeIter iter;
    gint tracknumber = 1;
    // Get our files...
    playlist_files = playlist_PL_ListGetSelection();
    if (playlist_files == NULL)
        return FALSE;

    // If we are moving files down we need to reverse the rows references...
    if (direction == 1) {
        playlist_files = g_list_reverse(playlist_files);
        g_list_foreach(playlist_files, (GFunc) __playlist_move_files_down, NULL);
    } else {
        g_list_foreach(playlist_files, (GFunc) __playlist_move_files_up, NULL);
    }
    // Now reorder all tracks in this playlist.
    if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(playlist_PL_List), &iter)) {
        gtk_list_store_set(GTK_LIST_STORE(playlist_PL_List), &iter, COL_PL_ORDER_NUM, tracknumber, -1);
        tracknumber++;
        while (gtk_tree_model_iter_next(GTK_TREE_MODEL(playlist_PL_List), &iter)) {
            gtk_list_store_set(GTK_LIST_STORE(playlist_PL_List), &iter, COL_PL_ORDER_NUM, tracknumber, -1);
            tracknumber++;
        }
    }
    return TRUE;
}
