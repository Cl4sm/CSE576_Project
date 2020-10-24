    GtkTreePath *path;
    GtkTreeIter iter;
    gchar *filename = NULL;
    gchar* fullfilename = NULL;

    gboolean isFolder;
    uint32_t objectID;


    fullfilename = g_malloc0(8192);
    // First of all, lets set the download path.

    // convert the referenece to a path and retrieve the iterator;
    path = gtk_tree_row_reference_get_path(Row);
    gtk_tree_model_get_iter(GTK_TREE_MODEL(fileList), &iter, path);
    // We have our Iter now.
    // Before we download, is it a folder ?
    gtk_tree_model_get(GTK_TREE_MODEL(fileList), &iter, COL_ISFOLDER, &isFolder, COL_FILENAME_ACTUAL, &filename, COL_FILEID, &objectID, -1);
    if (isFolder == FALSE) {
        // Our strings are not equal, so we get to download the file.
        g_sprintf(fullfilename, "%s/%s", Preferences.fileSystemDownloadPath->str, filename);
        // Now download the actual file from the MTP device.
        // Check if file exists?
        if (access(fullfilename, F_OK) != -1) {
            // We have that file already?
            if (Preferences.prompt_overwrite_file_op == TRUE) {
                if (fileoverwriteop == MTP_ASK) {
                    fileoverwriteop = displayFileOverwriteDialog(filename);
                }
                switch (fileoverwriteop) {
                    case MTP_ASK:
                        break;
                    case MTP_SKIP:
                        fileoverwriteop = MTP_ASK;
                        break;
                    case MTP_SKIP_ALL:
                        break;
                    case MTP_OVERWRITE:
                        filesDownload(filename, objectID);
                        fileoverwriteop = MTP_ASK;
                        break;
                    case MTP_OVERWRITE_ALL:
                        filesDownload(filename, objectID);
                        break;
                }
            } else {
                filesDownload(filename, objectID);
            }
        } else {
            filesDownload(filename, objectID);
        }
    } else {
        // Overwrite critera performed within this call...
        if (g_ascii_strcasecmp(filename, "..") != 0) {
            folderDownload(filename, objectID, TRUE);
        } else {
            g_fprintf(stderr, _("I don't know how to download a parent folder reference?\n"));
            displayError(_("I don't know how to download a parent folder reference?\n"));
        }
    }
    g_free(filename);
    g_free(fullfilename);
}
