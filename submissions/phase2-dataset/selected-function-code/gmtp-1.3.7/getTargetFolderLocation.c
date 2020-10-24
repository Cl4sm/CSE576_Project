    GtkWidget *dialog;
    GtkWidget *treeviewFoldersDialog;
    GtkTreeStore *folderListDialog;
    GtkTreeSelection *folderSelectionDialog;
    GtkWidget *scrolledwindowFoldersDialog;
    GtkTreeModel *folderListModelDialog;

    GtkTreeModel *sortmodel;
    GtkTreeIter iter;
    GtkTreeIter childiter;
    uint32_t objectID = 0;

    dialog = gtk_dialog_new_with_buttons(_("Move To..."), GTK_WINDOW(windowMain),
            (GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
            _("_OK"), GTK_RESPONSE_OK,
            _("_Cancel"), GTK_RESPONSE_CANCEL,
            NULL);

    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
    gtk_window_set_resizable(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 240, 400);

    // Actual folder list.
    scrolledwindowFoldersDialog = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_show(scrolledwindowFoldersDialog);
#if HAVE_GTK3 == 0
    gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), scrolledwindowFoldersDialog);
#else
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), scrolledwindowFoldersDialog);
    gtk_widget_set_vexpand(scrolledwindowFoldersDialog, TRUE);
#endif

    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindowFoldersDialog), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    treeviewFoldersDialog = gtk_tree_view_new();
    gtk_widget_show(treeviewFoldersDialog);
    gtk_container_add(GTK_CONTAINER(scrolledwindowFoldersDialog), treeviewFoldersDialog);
    gtk_container_set_border_width(GTK_CONTAINER(treeviewFoldersDialog), 5);
    folderSelectionDialog = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeviewFoldersDialog));
    gtk_tree_selection_set_mode(folderSelectionDialog, GTK_SELECTION_SINGLE);

    folderListDialog = gtk_tree_store_new(NUM_FOL_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_UINT, GDK_TYPE_PIXBUF);
    setupFolderList(GTK_TREE_VIEW(treeviewFoldersDialog));

    folderListModelDialog = gtk_tree_model_sort_new_with_model(GTK_TREE_MODEL(folderListDialog));
    gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(folderListModelDialog),
            COL_FOL_NAME_HIDDEN, GTK_SORT_ASCENDING);

    gtk_tree_view_set_model(GTK_TREE_VIEW(treeviewFoldersDialog), GTK_TREE_MODEL(folderListModelDialog));

    folderListAddDialog(deviceFolders, NULL, folderListDialog);

    gtk_tree_view_expand_all(GTK_TREE_VIEW(treeviewFoldersDialog));
    g_object_unref(folderListDialog);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {

        // Get our selected row.
        sortmodel = gtk_tree_view_get_model(GTK_TREE_VIEW(treeviewFoldersDialog));

        if (gtk_tree_selection_get_selected(folderSelectionDialog, &sortmodel, &iter)) {
            gtk_tree_model_sort_convert_iter_to_child_iter(GTK_TREE_MODEL_SORT(sortmodel), &childiter, &iter);
            gtk_tree_model_get(GTK_TREE_MODEL(folderListDialog), &childiter, COL_FOL_ID, &objectID, -1);
            gtk_widget_destroy(dialog);
            return objectID;
        }
    }
    gtk_widget_destroy(dialog);
    return -1;
}
