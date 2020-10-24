    gchar *foldername = NULL;
    uint32_t tmpFolderID = 0;
    // Get the folder name by displaying a dialog.
    foldername = displayFolderNewDialog();
    if (foldername != NULL) {
        // Let's see if we have anything selected in the folder view, and if not, then we add the
        // folder to the current Folder.
        if (gtk_tree_selection_count_selected_rows(folderSelection) == 0) {
            // Add in folder to MTP device.
            folderAdd(foldername);
        } else {
            // We have selected a folder in the folder view, so let's get it's ID.
            tmpFolderID = currentFolderID;
            currentFolderID = folderListGetSelection();
            folderAdd(foldername);
            currentFolderID = tmpFolderID;
        }
        g_free(foldername);
        deviceRescan();
    }
} // end on_folderNewFolder_activate()
