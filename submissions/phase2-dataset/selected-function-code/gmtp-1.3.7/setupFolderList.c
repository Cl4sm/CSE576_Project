GtkTreeViewColumn *setupFolderList(GtkTreeView *treeviewFolders) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkTreeViewColumn *folderColumnInt;

    folderColumnInt = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(folderColumnInt, _("Folder"));

    renderer = gtk_cell_renderer_pixbuf_new();
    gtk_tree_view_column_pack_start(folderColumnInt, renderer, FALSE);
    gtk_tree_view_column_set_attributes(folderColumnInt, renderer, "pixbuf", COL_FOL_ICON, NULL);

    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "weight", PANGO_WEIGHT_BOLD, NULL);
    gtk_tree_view_column_pack_start(folderColumnInt, renderer, TRUE);
    gtk_tree_view_column_set_attributes(folderColumnInt, renderer, "text", COL_FOL_NAME_HIDDEN, NULL);

    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFolders), folderColumnInt);
    gtk_tree_view_column_set_sort_column_id(folderColumnInt, COL_FOL_NAME_HIDDEN);
    gtk_tree_view_column_set_resizable(folderColumnInt, TRUE);
    gtk_tree_view_column_set_spacing(folderColumnInt, 5);

    // Folder column for sorting.
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Folder name Hidden", renderer,
            "text", COL_FOL_NAME_HIDDEN,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFolders), column);
    gtk_tree_view_column_set_visible(column, FALSE);

    return folderColumnInt;
}
