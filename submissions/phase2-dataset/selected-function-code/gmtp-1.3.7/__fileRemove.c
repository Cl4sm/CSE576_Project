void __fileRemove(GtkTreeRowReference *Row) {
    GtkTreePath *path;
    GtkTreeIter iter;
    gchar* filename = NULL;
    uint32_t objectID;
    gboolean isFolder;
    // convert the referenece to a path and retrieve the iterator;
    path = gtk_tree_row_reference_get_path(Row);
    gtk_tree_model_get_iter(GTK_TREE_MODEL(fileList), &iter, path);
    // We have our Iter now.
    gtk_tree_model_get(GTK_TREE_MODEL(fileList), &iter, COL_ISFOLDER, &isFolder, COL_FILENAME_ACTUAL, &filename,
            COL_FILEID, &objectID, -1);
    if (isFolder == FALSE) {
        gtk_tree_model_get_iter(GTK_TREE_MODEL(fileList), &iter, path);
        gtk_list_store_remove(GTK_LIST_STORE(fileList), &iter);
        // Now get rid of the actual file from the MTP device.
        filesDelete(filename, objectID);
    } else {
        // Our file is really a folder, so perform a folder remove operation.
        __folderRemove(Row);
    }
    g_free(filename);
}
