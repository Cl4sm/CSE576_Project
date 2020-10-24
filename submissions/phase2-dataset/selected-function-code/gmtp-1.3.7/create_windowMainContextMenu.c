GtkWidget* create_windowMainContextMenu(void) {
    GtkWidget* menu;
    //GtkWidget* cfileAdd;
    GtkWidget* cfileRename;
    GtkWidget* cfileRemove;
    GtkWidget* cfileDownload;
    GtkWidget* cfileMove;
    //GtkWidget* cfileNewFolder;
    GtkWidget* cfileRemoveFolder;
    GtkWidget* cfileRescan;
    GtkWidget* cfileAddToPlaylist;
    GtkWidget* cfileRemoveFromPlaylist;
    GtkWidget* menuseparator1;
    GtkWidget* menuseparator2;
    GtkWidget* menuseparator3;

    menu = gtk_menu_new();

    cfileAdd = gtk_menu_item_new_with_label(_("Add Files"));
    gtk_widget_show(cfileAdd);
    gtk_container_add(GTK_CONTAINER(menu), cfileAdd);

    cfileRemove = gtk_menu_item_new_with_label(_("Delete Files"));
    gtk_widget_show(cfileRemove);
    gtk_container_add(GTK_CONTAINER(menu), cfileRemove);

    cfileDownload = gtk_menu_item_new_with_label(_("Download Files"));
    gtk_widget_show(cfileDownload);
    gtk_container_add(GTK_CONTAINER(menu), cfileDownload);

    cfileRename = gtk_menu_item_new_with_label(_("Rename File"));
    gtk_widget_show(cfileRename);
    gtk_container_add(GTK_CONTAINER(menu), cfileRename);

    cfileMove = gtk_menu_item_new_with_label(_("Move To..."));
    gtk_widget_show(cfileMove);
    gtk_container_add(GTK_CONTAINER(menu), cfileMove);

    menuseparator3 = gtk_separator_menu_item_new();
    gtk_widget_show(menuseparator3);
    gtk_container_add(GTK_CONTAINER(menu), menuseparator3);

    cfileAddToPlaylist = gtk_menu_item_new_with_label(_("Add To Playlist"));
    gtk_widget_show(cfileAddToPlaylist);
    gtk_container_add(GTK_CONTAINER(menu), cfileAddToPlaylist);

    cfileRemoveFromPlaylist = gtk_menu_item_new_with_label(_("Remove From Playlist"));
    gtk_widget_show(cfileRemoveFromPlaylist);
    gtk_container_add(GTK_CONTAINER(menu), cfileRemoveFromPlaylist);

    menuseparator1 = gtk_separator_menu_item_new();
    gtk_widget_show(menuseparator1);
    gtk_container_add(GTK_CONTAINER(menu), menuseparator1);

    cfileNewFolder = gtk_menu_item_new_with_label(_("Create Folder"));
    gtk_widget_show(cfileNewFolder);
    gtk_container_add(GTK_CONTAINER(menu), cfileNewFolder);

    cfileRemoveFolder = gtk_menu_item_new_with_label(_("Delete Folder"));
    gtk_widget_show(cfileRemoveFolder);
    gtk_container_add(GTK_CONTAINER(menu), cfileRemoveFolder);

    menuseparator2 = gtk_separator_menu_item_new();
    gtk_widget_show(menuseparator2);
    gtk_container_add(GTK_CONTAINER(menu), menuseparator2);

    cfileRescan = gtk_menu_item_new_with_label(_("Refresh Device"));
    gtk_widget_show(cfileRescan);
    gtk_container_add(GTK_CONTAINER(menu), cfileRescan);

    // Now our call backs.
    g_signal_connect((gpointer) cfileAdd, "activate",
            G_CALLBACK(on_filesAdd_activate),
            NULL);

    g_signal_connect((gpointer) cfileDownload, "activate",
            G_CALLBACK(on_filesDownload_activate),
            NULL);

    g_signal_connect((gpointer) cfileRename, "activate",
            G_CALLBACK(on_fileRenameFile_activate),
            NULL);

    g_signal_connect((gpointer) cfileMove, "activate",
            G_CALLBACK(on_fileMoveFile_activate),
            NULL);

    g_signal_connect((gpointer) cfileRemove, "activate",
            G_CALLBACK(on_filesDelete_activate),
            NULL);

    g_signal_connect((gpointer) cfileNewFolder, "activate",
            G_CALLBACK(on_fileNewFolder_activate),
            NULL);

    g_signal_connect((gpointer) cfileRemoveFolder, "activate",
            G_CALLBACK(on_fileRemoveFolder_activate),
            NULL);

    g_signal_connect((gpointer) cfileRescan, "activate",
            G_CALLBACK(on_deviceRescan_activate),
            NULL);

    g_signal_connect((gpointer) cfileAddToPlaylist, "activate",
            G_CALLBACK(on_fileAddToPlaylist_activate),
            NULL);

    g_signal_connect((gpointer) cfileRemoveFromPlaylist, "activate",
            G_CALLBACK(on_fileRemoveFromPlaylist_activate),
            NULL);

    return menu;

}
