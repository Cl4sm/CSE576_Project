    GtkTreeModel *sortmodel;
    GtkTreeIter iter;
    GtkTreeIter childiter;
    int64_t targetfolder = 0;
    uint32_t objectID;

    LIBMTP_folder_t *currentFolder = NULL;
    LIBMTP_folder_t *newFolder = NULL;
    int error;

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
        gtk_tree_model_get(GTK_TREE_MODEL(folderList), &childiter, COL_FOL_ID, &objectID, -1);

        // Prompt for the target folder location.
        targetfolder = getTargetFolderLocation();
        if (targetfolder == -1) {
            // If the user didn't select a folder, or the target folder is the current selected folder
            // then do nothing.
            return;
        }
        fileMoveTargetFolder = targetfolder;
        gtk_tree_selection_unselect_all(folderSelection);

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
    deviceRescan();
} // end on_folderMoveFolder_activate()
