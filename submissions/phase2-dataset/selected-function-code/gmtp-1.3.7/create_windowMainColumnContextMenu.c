    GtkWidget* menu;
    menu = gtk_menu_new();

    cViewSize = gtk_check_menu_item_new_with_label(_("File Size"));
    gtk_widget_show(cViewSize);
    gtk_container_add(GTK_CONTAINER(menu), cViewSize);

    cViewType = gtk_check_menu_item_new_with_label(_("File Type"));
    gtk_widget_show(cViewType);
    gtk_container_add(GTK_CONTAINER(menu), cViewType);

    cViewTrackNumber = gtk_check_menu_item_new_with_label(_("Track Number"));
    gtk_widget_show(cViewTrackNumber);
    gtk_container_add(GTK_CONTAINER(menu), cViewTrackNumber);

    cViewTrackName = gtk_check_menu_item_new_with_label(_("Track Name"));
    gtk_widget_show(cViewTrackName);
    gtk_container_add(GTK_CONTAINER(menu), cViewTrackName);

    cViewArtist = gtk_check_menu_item_new_with_label(_("Artist"));
    gtk_widget_show(cViewArtist);
    gtk_container_add(GTK_CONTAINER(menu), cViewArtist);

    cViewAlbum = gtk_check_menu_item_new_with_label(_("Album"));
    gtk_widget_show(cViewAlbum);
    gtk_container_add(GTK_CONTAINER(menu), cViewAlbum);

    cViewYear = gtk_check_menu_item_new_with_label(_("Year"));
    gtk_widget_show(cViewYear);
    gtk_container_add(GTK_CONTAINER(menu), cViewYear);

    cViewGenre = gtk_check_menu_item_new_with_label(_("Genre"));
    gtk_widget_show(cViewGenre);
    gtk_container_add(GTK_CONTAINER(menu), cViewGenre);

    cViewDuration = gtk_check_menu_item_new_with_label(_("Duration"));
    gtk_widget_show(cViewDuration);
    gtk_container_add(GTK_CONTAINER(menu), cViewDuration);


    // Now our call backs.
    g_signal_connect((gpointer) cViewSize, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) cViewTrackNumber, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) cViewTrackName, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) cViewType, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) cViewArtist, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) cViewAlbum, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) cViewYear, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) cViewGenre, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) cViewDuration, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    return menu;

}
