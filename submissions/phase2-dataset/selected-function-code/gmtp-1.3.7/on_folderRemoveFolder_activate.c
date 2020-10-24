    GtkWidget *dialog;
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
        gtk_tree_model_get(GTK_TREE_MODEL(folderList), &childiter, COL_FOL_ID, &objectID, -1);

        // Now we prompt to confirm delete?
        if (Preferences.confirm_file_delete_op == FALSE) {
            // Now download the actual file from the MTP device.
            folderDelete(getCurrentFolderPtr(deviceFolders, objectID), 0);
        } else {
            dialog = gtk_message_dialog_new(GTK_WINDOW(windowMain),
                    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_MESSAGE_WARNING,
                    GTK_BUTTONS_YES_NO,
                    _("Are you sure you want to delete this folder (and all contents)?"));
            gtk_window_set_title(GTK_WINDOW(dialog), _("Confirm Delete"));
            gint result = gtk_dialog_run(GTK_DIALOG(dialog));
            if (result == GTK_RESPONSE_YES)
                folderDelete(getCurrentFolderPtr(deviceFolders, objectID), 0);
            gtk_widget_destroy(dialog);
        }

        //folderDelete(getCurrentFolderPtr(deviceFolders, objectID), 0);
    }
    deviceRescan();
} // end on_folderRemoveFolder_activate()
