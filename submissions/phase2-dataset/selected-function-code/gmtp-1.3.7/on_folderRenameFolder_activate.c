void on_folderRenameFolder_activate(GtkMenuItem *menuitem, gpointer user_data) {
    gchar *newfilename = NULL;
    gchar *filename = NULL;
    GtkTreeModel *sortmodel;
    GtkTreeIter iter;
    GtkTreeIter childiter;

    uint32_t objectID;

    // Let's see if we have anything selected in the folder view, and if not let the user know, and return
    if (gtk_tree_selection_count_selected_rows(folderSelection) == 0) {
        // Add in folder to MTP device.
        displayInformation(_("No files/folders selected?"));
        return;
    } else {
        // We have selected a folder in the folder view, so let's get it's ID.
        sortmodel = gtk_tree_view_get_model(GTK_TREE_VIEW(treeviewFiles));
        gtk_tree_selection_get_selected(folderSelection, &sortmodel, &iter);
        gtk_tree_model_sort_convert_iter_to_child_iter(GTK_TREE_MODEL_SORT(sortmodel), &childiter, &iter);
        gtk_tree_model_get(GTK_TREE_MODEL(folderList), &childiter, COL_FOL_ID, &objectID,
                COL_FOL_NAME_HIDDEN, &filename, -1);

        // Get our new folder name.
        newfilename = displayRenameFileDialog(filename);

        // If the user supplied something, then update the name of the device.
        if (newfilename != NULL) {
            filesRename(newfilename, objectID);
            g_free(newfilename);
            deviceRescan();
        }
    }
} // end on_folderRenameFolder_activate()
