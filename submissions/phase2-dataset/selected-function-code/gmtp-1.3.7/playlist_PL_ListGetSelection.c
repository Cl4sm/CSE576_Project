    GList *selectedFiles, *ptr;
    GtkTreeRowReference *ref;
    GtkTreeModel *model;
    // Lets clear up the old list.
    g_list_free(playlist_Selection_PL_RowReferences);
    playlist_Selection_PL_RowReferences = NULL;

    if (gtk_tree_selection_count_selected_rows(playlist_PL_Selection) == 0) {
        // We have no rows.
        return NULL;
    }
    // So now we must convert each selection to a row reference and store it in a new GList variable
    // which we will return below.
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview_Playlist_Files));
    selectedFiles = gtk_tree_selection_get_selected_rows(playlist_PL_Selection, &model);
    ptr = selectedFiles;
    while (ptr != NULL) {
        ref = gtk_tree_row_reference_new(GTK_TREE_MODEL(playlist_PL_List), (GtkTreePath*) ptr->data);
        playlist_Selection_PL_RowReferences = g_list_append(playlist_Selection_PL_RowReferences, gtk_tree_row_reference_copy(ref));
        gtk_tree_row_reference_free(ref);
        ptr = ptr->next;
    }
    g_list_foreach(selectedFiles, (GFunc) gtk_tree_path_free, NULL);
    g_list_free(selectedFiles);
    return playlist_Selection_PL_RowReferences;
}
