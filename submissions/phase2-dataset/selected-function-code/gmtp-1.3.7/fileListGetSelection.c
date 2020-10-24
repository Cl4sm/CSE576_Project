GList* fileListGetSelection() {
    GList *selectedFiles, *ptr;
    GtkTreeRowReference *ref;
    GtkTreeModel *sortmodel;
    // Lets clear up the old list.
    g_list_free(fileSelection_RowReferences);
    fileSelection_RowReferences = NULL;

    if (gtk_tree_selection_count_selected_rows(fileSelection) == 0) {
        // We have no rows.
        return NULL;
    }
    // So now we must convert each selection to a row reference and store it in a new GList variable
    // which we will return below.
    sortmodel = gtk_tree_view_get_model(GTK_TREE_VIEW(treeviewFiles));
    selectedFiles = gtk_tree_selection_get_selected_rows(fileSelection, &sortmodel);
    ptr = selectedFiles;
    while (ptr != NULL) {
        // Add our row into the GSList so it can be parsed by the respective operations, including
        // handling changing from the sort model to the real underlying model.
        ref = gtk_tree_row_reference_new(GTK_TREE_MODEL(fileList),
                gtk_tree_model_sort_convert_path_to_child_path(GTK_TREE_MODEL_SORT(sortmodel), (GtkTreePath*) ptr->data));
        fileSelection_RowReferences = g_list_prepend(fileSelection_RowReferences, gtk_tree_row_reference_copy(ref));
        gtk_tree_row_reference_free(ref);
        ptr = ptr->next;
    }
    g_list_foreach(selectedFiles, (GFunc) gtk_tree_path_free, NULL);
    g_list_free(selectedFiles);
    return fileSelection_RowReferences;
}
