    GtkTreePath *path;
    GtkTreeIter iter;

    gboolean isFolder;
    LIBMTP_folder_t *currentFolder = NULL;
    LIBMTP_folder_t *newFolder = NULL;
    uint32_t objectID;
    int error;

    // convert the referenece to a path and retrieve the iterator;
    path = gtk_tree_row_reference_get_path(Row);
    gtk_tree_model_get_iter(GTK_TREE_MODEL(fileList), &iter, path);
    // We have our Iter now.
    // Before we move, is it a folder ?
    gtk_tree_model_get(GTK_TREE_MODEL(fileList), &iter, COL_ISFOLDER, &isFolder, COL_FILEID, &objectID, -1);
    if (isFolder == FALSE) {
        if ((error = setNewParentFolderID(objectID, fileMoveTargetFolder)) != 0) {
            displayError(_("Unable to move the selected file?\n"));
            g_fprintf(stderr, "File Move Error: %d\n", error);
            LIBMTP_Dump_Errorstack(DeviceMgr.device);
            LIBMTP_Clear_Errorstack(DeviceMgr.device);
        }
    } else {
        // Make sure we don't want to move the folder into itself?
        if (objectID == fileMoveTargetFolder) {
            displayError(_("Unable to move the selected folder into itself?\n"));
            g_fprintf(stderr, _("Unable to move the selected folder into itself?\n"));
            return;
        }
        // We have the target folder, so let's check to ensure that we will not create a circular
        // reference by moving a folder underneath it self.
        currentFolder = getCurrentFolderPtr(deviceFolders, objectID);
        if (currentFolder == NULL) {
            // WTF?
            g_fprintf(stderr, "File Move Error: Can't get current folder pointer\n");
            return;
        }
        // Use currentFolder as the starting point, and simply attempt to get the ptr to the new
        // folder based on this point.
        newFolder = getCurrentFolderPtr(currentFolder->child, fileMoveTargetFolder);
        if (newFolder == NULL) {
            // We are alright to proceed.
            if ((error = setNewParentFolderID(objectID, fileMoveTargetFolder)) != 0) {
                displayError(_("Unable to move the selected folder?\n"));
                g_fprintf(stderr, "File Move Error: %d\n", error);
                LIBMTP_Dump_Errorstack(DeviceMgr.device);
                LIBMTP_Clear_Errorstack(DeviceMgr.device);
            }
        } else {
            displayError(_("Unable to move the selected folder underneath itself?\n"));
            g_fprintf(stderr, _("Unable to move the selected folder underneath itself?\n"));
        }
    }
}
