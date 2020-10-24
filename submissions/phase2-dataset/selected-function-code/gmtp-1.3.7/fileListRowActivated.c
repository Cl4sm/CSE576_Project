void fileListRowActivated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer data) {
    GtkTreeModel *model;
    GtkTreeModel *sortmodel;
    GtkTreeIter iter;

    gchar *filename = NULL;
    gboolean isFolder;
    uint32_t objectID;

    GtkWidget *FileDialog;
    gchar *savepath = NULL;

    // Obtain the iter, and the related objectID.
    sortmodel = gtk_tree_view_get_model(treeview);
    model = gtk_tree_model_sort_get_model(GTK_TREE_MODEL_SORT(sortmodel));
    if (gtk_tree_model_get_iter(model, &iter, gtk_tree_model_sort_convert_path_to_child_path(GTK_TREE_MODEL_SORT(sortmodel), path))) {
        gtk_tree_model_get(GTK_TREE_MODEL(fileList), &iter, COL_ISFOLDER, &isFolder, COL_FILENAME_ACTUAL, &filename, COL_FILEID, &objectID, -1);
        if (isFolder == FALSE) {
            // Now download the actual file from the MTP device.
            displayProgressBar(_("File download"));
            savepath = g_malloc0(8192);
            // Let's confirm our download path.
            if (Preferences.ask_download_path == TRUE) {
                FileDialog = gtk_file_chooser_dialog_new(_("Select Path to Download"),
                        GTK_WINDOW(windowMain), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                        _("_Cancel"), GTK_RESPONSE_CANCEL,
                        _("_Open"), GTK_RESPONSE_ACCEPT,
                        NULL);

                gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(FileDialog), Preferences.fileSystemDownloadPath->str);
                if (gtk_dialog_run(GTK_DIALOG(FileDialog)) == GTK_RESPONSE_ACCEPT) {
                    savepath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(FileDialog));
                    // Save our download path.
                    Preferences.fileSystemDownloadPath = g_string_assign(Preferences.fileSystemDownloadPath, savepath);
                    // We do the deed.
                    displayProgressBar(_("File download"));
                    filesDownload(filename, objectID);
                    destroyProgressBar();
                }
                gtk_widget_destroy(FileDialog);
            } else {
                // We do the deed.
                displayProgressBar(_("File download"));
                filesDownload(filename, objectID);
                destroyProgressBar();
            }
            destroyProgressBar();
            g_free(savepath);

        } else {
            // Maintain the stack of folder IDs and names for alt access mode.
            if (Preferences.use_alt_access_method) {
                if (g_ascii_strcasecmp(filename, "..") == 0) {
                    // going down a level.
                    g_free(g_queue_pop_tail(stackFolderIDs));
                    g_free(g_queue_pop_tail(stackFolderNames));
                } else {
                    // going up a level
                    guint *currentFld = g_malloc(sizeof (guint));
                    *currentFld = currentFolderID;
                    g_queue_push_tail(stackFolderIDs, currentFld);
                    g_queue_push_tail(stackFolderNames, g_strdup(filename));
                }
            }

            // We have a folder so change to it?
            currentFolderID = objectID;
            on_editFindClose_activate(NULL, NULL);
        }
    }
    g_free(filename);
} // end fileListRowActivated()
