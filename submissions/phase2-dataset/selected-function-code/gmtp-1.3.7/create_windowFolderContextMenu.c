GtkWidget* create_windowFolderContextMenu(void) {
    GtkWidget* menu;
    GtkWidget* menuseparator1;
    menu = gtk_menu_new();

    cfFolderAdd = gtk_menu_item_new_with_label(_("Create Folder"));
    gtk_widget_show(cfFolderAdd);
    gtk_container_add(GTK_CONTAINER(menu), cfFolderAdd);

    cfFolderRename = gtk_menu_item_new_with_label(_("Rename Folder"));
    gtk_widget_show(cfFolderRename);
    gtk_container_add(GTK_CONTAINER(menu), cfFolderRename);

    cfFolderDelete = gtk_menu_item_new_with_label(_("Delete Folder"));
    gtk_widget_show(cfFolderDelete);
    gtk_container_add(GTK_CONTAINER(menu), cfFolderDelete);

    cfFolderMove = gtk_menu_item_new_with_label(_("Move To..."));
    gtk_widget_show(cfFolderMove);
    gtk_container_add(GTK_CONTAINER(menu), cfFolderMove);

    menuseparator1 = gtk_separator_menu_item_new();
    gtk_widget_show(menuseparator1);
    gtk_container_add(GTK_CONTAINER(menu), menuseparator1);

    cfFolderRefresh = gtk_menu_item_new_with_label(_("Refresh Device"));
    gtk_widget_show(cfFolderRefresh);
    gtk_container_add(GTK_CONTAINER(menu), cfFolderRefresh);


    // Now our call backs.
    g_signal_connect((gpointer) cfFolderRefresh, "activate",
            G_CALLBACK(on_deviceRescan_activate),
            NULL);

    g_signal_connect((gpointer) cfFolderRename, "activate",
            G_CALLBACK(on_folderRenameFolder_activate),
            NULL);

    g_signal_connect((gpointer) cfFolderDelete, "activate",
            G_CALLBACK(on_folderRemoveFolder_activate),
            NULL);

    g_signal_connect((gpointer) cfFolderMove, "activate",
            G_CALLBACK(on_folderMoveFolder_activate),
            NULL);

    g_signal_connect((gpointer) cfFolderAdd, "activate",
            G_CALLBACK(on_folderNewFolder_activate),
            NULL);


    return menu;

}
