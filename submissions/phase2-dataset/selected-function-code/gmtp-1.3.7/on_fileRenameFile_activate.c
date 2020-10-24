
    GtkTreePath *path;
    GtkTreeIter iter;
    gchar *newfilename = NULL;
    gchar *filename = NULL;
    gboolean isFolder;
    uint32_t ObjectID = 0;

    // Let's check to see if we have anything selected in our treeview?
    if (fileListGetSelection() == NULL) {

        // See if anything is selected in the folder view, if so use that as our source.
        if (folderListGetSelection() != -1) {
            on_folderRenameFolder_activate(menuitem, user_data);
        } else {
            displayInformation(_("No files/folders selected?"));
        }
        return;
    }
    GList *List = fileListGetSelection();

    // We only care about the first entry.
    // convert the referenece to a path and retrieve the iterator;
    path = gtk_tree_row_reference_get_path(List->data);
    gtk_tree_model_get_iter(GTK_TREE_MODEL(fileList), &iter, path);
    // We have our Iter now.
    // Before we download, is it a folder ?
    gtk_tree_model_get(GTK_TREE_MODEL(fileList), &iter, COL_FILENAME_ACTUAL, &filename, COL_ISFOLDER, &isFolder,
            COL_FILEID, &ObjectID, -1);

    // Make sure we are not attempting to edit the parent link folder.
    if (g_ascii_strcasecmp(filename, "..") == 0) {
        g_fprintf(stderr, _("Unable to rename parent folder\n"));
        displayInformation(_("Unable to rename this folder"));
        return;
    }
    // Get our new device name.
    newfilename = displayRenameFileDialog(filename);

    // If the user supplied something, then update the name of the device.
    if (newfilename != NULL) {
        filesRename(newfilename, ObjectID);
        g_free(newfilename);
        deviceRescan();
    }
} // end on_editDeviceName_activate()
