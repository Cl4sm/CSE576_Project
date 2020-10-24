GtkWidget* create_windowPlaylist(void) {
    GtkWidget *window_playlist;
    GtkWidget *vbox1;
    GtkWidget *hbox1;
    GtkWidget *label_Playlist;

    GtkWidget *button_Add_Playlist;
    GtkWidget *button_Import_Playlist;
    GtkWidget *alignment2;
    GtkWidget *hbox3;
    GtkWidget *image2;
    GtkWidget *label3;

    GtkWidget *alignment1;
    GtkWidget *hbox2;
    GtkWidget *image1;
    GtkWidget *label2;
    GtkWidget *hbox4;
    GtkWidget *scrolledwindow2;

    GtkWidget *vbuttonbox1;

    GtkWidget *alignment6;
    GtkWidget *hbox8;
    GtkWidget *image6;
    GtkWidget *label10;

    GtkWidget *alignment7;
    GtkWidget *hbox9;
    GtkWidget *image7;
    GtkWidget *label11;
    GtkWidget *scrolledwindow3;

    GtkWidget *vbuttonbox2;

    GtkWidget *hbuttonbox1;
    GtkWidget *button_Close;


#if HAVE_GTK3 == 0
    GtkTooltips *tooltips;
    tooltips = gtk_tooltips_new();
#endif

    window_playlist = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gchar * winTitle;
    winTitle = g_strconcat(PACKAGE_NAME, _(" Playlists"), NULL);
    gtk_window_set_title(GTK_WINDOW(window_playlist), winTitle);
    gtk_window_set_modal(GTK_WINDOW(window_playlist), TRUE);
    gtk_window_set_resizable(GTK_WINDOW(window_playlist), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(window_playlist), 760, 400);
    gtk_window_set_transient_for(GTK_WINDOW(window_playlist), GTK_WINDOW(windowMain));
    gtk_window_set_position(GTK_WINDOW(window_playlist), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_skip_taskbar_hint(GTK_WINDOW(window_playlist), TRUE);
    gtk_window_set_type_hint(GTK_WINDOW(window_playlist), GDK_WINDOW_TYPE_HINT_DIALOG);
    g_free(winTitle);
#if HAVE_GTK3 == 0
    vbox1 = gtk_vbox_new(FALSE, 0);
#else
    vbox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
#endif
    gtk_widget_show(vbox1);
    gtk_container_add(GTK_CONTAINER(window_playlist), vbox1);
#if HAVE_GTK3 == 0
    hbox1 = gtk_hbox_new(FALSE, 5);
#else
    hbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
#endif
    gtk_widget_show(hbox1);
    gtk_box_pack_start(GTK_BOX(vbox1), hbox1, FALSE, TRUE, 5);

    label_Playlist = gtk_label_new(_("Current Playlist: "));
    gtk_widget_show(label_Playlist);
    gtk_box_pack_start(GTK_BOX(hbox1), label_Playlist, FALSE, FALSE, 5);
    gtk_misc_set_padding(GTK_MISC(label_Playlist), 5, 0);

#if HAVE_GTK3 == 0
    comboboxentry_playlist = gtk_combo_box_new_text();
#else
    comboboxentry_playlist = gtk_combo_box_text_new();
#endif
    gtk_widget_show(comboboxentry_playlist);
    gtk_box_pack_start(GTK_BOX(hbox1), comboboxentry_playlist, TRUE, TRUE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(comboboxentry_playlist), 5);

    button_Add_Playlist = gtk_button_new();
    gtk_widget_show(button_Add_Playlist);
    gtk_box_pack_start(GTK_BOX(hbox1), button_Add_Playlist, FALSE, FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(button_Add_Playlist), 5);
#if HAVE_GTK3 == 0
    gtk_tooltips_set_tip(tooltips, button_Add_Playlist, _("Add New Playlist"), NULL);
#else
    gtk_widget_set_tooltip_text(button_Add_Playlist, _("Add New Playlist"));
#endif

    alignment2 = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_show(alignment2);
    gtk_container_add(GTK_CONTAINER(button_Add_Playlist), alignment2);
#if HAVE_GTK3 == 0
    hbox3 = gtk_hbox_new(FALSE, 2);
#else
    hbox3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
#endif
    gtk_widget_show(hbox3);
    gtk_container_add(GTK_CONTAINER(alignment2), hbox3);
#if HAVE_GTK3 == 0
    image2 = gtk_image_new_from_stock(GTK_STOCK_ADD, GTK_ICON_SIZE_BUTTON);
#else 
    image2 = gtk_image_new_from_icon_name("list-add", GTK_ICON_SIZE_BUTTON);
#endif
    gtk_widget_show(image2);
    gtk_box_pack_start(GTK_BOX(hbox3), image2, FALSE, FALSE, 0);

    label3 = gtk_label_new_with_mnemonic(_("Add"));
    gtk_widget_show(label3);
    gtk_box_pack_start(GTK_BOX(hbox3), label3, FALSE, FALSE, 0);

    button_Del_Playlist = gtk_button_new();
    gtk_widget_show(button_Del_Playlist);
    gtk_box_pack_start(GTK_BOX(hbox1), button_Del_Playlist, FALSE, FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(button_Del_Playlist), 5);
#if HAVE_GTK3 == 0
    gtk_tooltips_set_tip(tooltips, button_Del_Playlist, _("Remove Current Selected Playlist"), NULL);
#else
    gtk_widget_set_tooltip_text(button_Del_Playlist, _("Remove Current Selected Playlist"));
#endif

    alignment1 = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_show(alignment1);
    gtk_container_add(GTK_CONTAINER(button_Del_Playlist), alignment1);
#if HAVE_GTK3 == 0
    hbox2 = gtk_hbox_new(FALSE, 2);
#else
    hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
#endif

    gtk_widget_show(hbox2);
    gtk_container_add(GTK_CONTAINER(alignment1), hbox2);
#if HAVE_GTK3 == 0
    image1 = gtk_image_new_from_stock(GTK_STOCK_DELETE, GTK_ICON_SIZE_BUTTON);
#else
    image1 = gtk_image_new_from_icon_name("edit-delete", GTK_ICON_SIZE_BUTTON);
#endif
    gtk_widget_show(image1);
    gtk_box_pack_start(GTK_BOX(hbox2), image1, FALSE, FALSE, 0);

    label2 = gtk_label_new_with_mnemonic(_("Del"));
    gtk_widget_show(label2);
    gtk_box_pack_start(GTK_BOX(hbox2), label2, FALSE, FALSE, 0);

    // Import Button
#if HAVE_GTK3 == 0
    button_Import_Playlist = gtk_button_new_from_stock(GTK_STOCK_OPEN);
#else
    button_Import_Playlist = gtk_button_new_with_label(_("Open"));
#endif
    gtk_widget_show(button_Import_Playlist);
    gtk_box_pack_start(GTK_BOX(hbox1), button_Import_Playlist, FALSE, FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(button_Import_Playlist), 5);
#if HAVE_GTK3 == 0
    gtk_tooltips_set_tip(tooltips, button_Import_Playlist, _("Import Playlist"), NULL);
#else
    gtk_widget_set_tooltip_text(button_Import_Playlist, _("Import Playlist"));
#endif

    // Export Button
#if HAVE_GTK3 == 0
    button_Export_Playlist = gtk_button_new_from_stock(GTK_STOCK_SAVE_AS);
#else
    button_Export_Playlist = gtk_button_new_with_label(_("Save"));
#endif
    gtk_widget_show(button_Export_Playlist);
    gtk_box_pack_start(GTK_BOX(hbox1), button_Export_Playlist, FALSE, FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(button_Export_Playlist), 5);
#if HAVE_GTK3 == 0
    gtk_tooltips_set_tip(tooltips, button_Export_Playlist, _("Export Playlist"), NULL);
#else
    gtk_widget_set_tooltip_text(button_Export_Playlist, _("Export Playlist"));
#endif

    // Scrolled Window.
#if HAVE_GTK3 == 0
    hbox4 = gtk_hbox_new(FALSE, 5);
#else
    hbox4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
#endif
    gtk_widget_show(hbox4);
    gtk_box_pack_start(GTK_BOX(vbox1), hbox4, TRUE, TRUE, 0);

    scrolledwindow2 = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_show(scrolledwindow2);
    gtk_box_pack_start(GTK_BOX(hbox4), scrolledwindow2, TRUE, TRUE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(scrolledwindow2), 5);

    treeview_Avail_Files = gtk_tree_view_new();
    gtk_widget_show(treeview_Avail_Files);
    gtk_container_add(GTK_CONTAINER(scrolledwindow2), treeview_Avail_Files);
    gtk_container_set_border_width(GTK_CONTAINER(treeview_Avail_Files), 5);
#if HAVE_GTK3 == 0
    gtk_tooltips_set_tip(tooltips, treeview_Avail_Files, _("Device Audio Tracks"), NULL);
#else
    gtk_widget_set_tooltip_text(treeview_Avail_Files, _("Device Audio Tracks"));
#endif

    playlist_TrackSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_Avail_Files));
    gtk_tree_selection_set_mode(playlist_TrackSelection, GTK_SELECTION_MULTIPLE);

    playlist_TrackList = gtk_list_store_new(NUM_TCOLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_STRING, G_TYPE_STRING);
    setupTrackList(GTK_TREE_VIEW(treeview_Avail_Files));
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_Avail_Files), GTK_TREE_MODEL(playlist_TrackList));
    g_object_unref(playlist_TrackList);
#if HAVE_GTK3 == 0
    vbuttonbox1 = gtk_vbutton_box_new();
#else
    vbuttonbox1 = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
#endif
    gtk_widget_show(vbuttonbox1);
    gtk_box_pack_start(GTK_BOX(hbox4), vbuttonbox1, FALSE, FALSE, 0);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(vbuttonbox1), GTK_BUTTONBOX_SPREAD);

    button_Add_Files = gtk_button_new();
    gtk_widget_show(button_Add_Files);
    gtk_container_add(GTK_CONTAINER(vbuttonbox1), button_Add_Files);
#if HAVE_GTK3 == 0
    gtk_tooltips_set_tip(tooltips, button_Add_Files, _("Add file to playlist"), NULL);
#else
    gtk_widget_set_tooltip_text(button_Add_Files, _("Add file to playlist"));
#endif

    alignment6 = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_show(alignment6);
    gtk_container_add(GTK_CONTAINER(button_Add_Files), alignment6);
#if HAVE_GTK3 == 0
    hbox8 = gtk_hbox_new(FALSE, 2);
#else
    hbox8 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
#endif
    gtk_widget_show(hbox8);
    gtk_container_add(GTK_CONTAINER(alignment6), hbox8);
#if HAVE_GTK3 == 0
    image6 = gtk_image_new_from_stock(GTK_STOCK_GO_FORWARD, GTK_ICON_SIZE_BUTTON);
#else
    image6 = gtk_image_new_from_icon_name("go-next", GTK_ICON_SIZE_BUTTON);
#endif
    gtk_widget_show(image6);
    gtk_box_pack_start(GTK_BOX(hbox8), image6, FALSE, FALSE, 0);

    label10 = gtk_label_new_with_mnemonic(_("Add File"));
    gtk_widget_show(label10);
    gtk_box_pack_start(GTK_BOX(hbox8), label10, FALSE, FALSE, 0);

    button_Del_File = gtk_button_new();
    gtk_widget_show(button_Del_File);
    gtk_container_add(GTK_CONTAINER(vbuttonbox1), button_Del_File);
#if HAVE_GTK3 == 0
    gtk_tooltips_set_tip(tooltips, button_Del_File, _("Remove file from playlist"), NULL);
#else
    gtk_widget_set_tooltip_text(button_Del_File, _("Remove file from playlist"));
#endif

    alignment7 = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_show(alignment7);
    gtk_container_add(GTK_CONTAINER(button_Del_File), alignment7);
#if HAVE_GTK3 == 0
    hbox9 = gtk_hbox_new(FALSE, 2);
#else
    hbox9 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
#endif
    gtk_widget_show(hbox9);
    gtk_container_add(GTK_CONTAINER(alignment7), hbox9);
#if HAVE_GTK3 == 0
    image7 = gtk_image_new_from_stock(GTK_STOCK_GO_BACK, GTK_ICON_SIZE_BUTTON);
#else
    image7 = gtk_image_new_from_icon_name("go-previous", GTK_ICON_SIZE_BUTTON);
#endif
    gtk_widget_show(image7);
    gtk_box_pack_start(GTK_BOX(hbox9), image7, FALSE, FALSE, 0);

    label11 = gtk_label_new_with_mnemonic(_("Del File"));
    gtk_widget_show(label11);
    gtk_box_pack_start(GTK_BOX(hbox9), label11, FALSE, FALSE, 0);

    scrolledwindow3 = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_show(scrolledwindow3);
    gtk_box_pack_start(GTK_BOX(hbox4), scrolledwindow3, TRUE, TRUE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(scrolledwindow3), 5);

    treeview_Playlist_Files = gtk_tree_view_new();
    gtk_widget_show(treeview_Playlist_Files);
    gtk_container_add(GTK_CONTAINER(scrolledwindow3), treeview_Playlist_Files);
    gtk_container_set_border_width(GTK_CONTAINER(treeview_Playlist_Files), 5);
#if HAVE_GTK3 == 0
    gtk_tooltips_set_tip(tooltips, treeview_Playlist_Files, _("Playlist Audio Tracks"), NULL);
#else
    gtk_widget_set_tooltip_text(treeview_Playlist_Files, _("Playlist Audio Tracks"));
#endif

    playlist_PL_Selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_Playlist_Files));
    gtk_tree_selection_set_mode(playlist_PL_Selection, GTK_SELECTION_MULTIPLE);

    playlist_PL_List = gtk_list_store_new(NUM_PL_COLUMNS, G_TYPE_UINT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_STRING, G_TYPE_STRING);
    setup_PL_List(GTK_TREE_VIEW(treeview_Playlist_Files));
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_Playlist_Files), GTK_TREE_MODEL(playlist_PL_List));
    g_object_unref(playlist_PL_List);
#if HAVE_GTK3 == 0
    vbuttonbox2 = gtk_vbutton_box_new();
#else
    vbuttonbox2 = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
#endif
    gtk_widget_show(vbuttonbox2);
    gtk_box_pack_start(GTK_BOX(hbox4), vbuttonbox2, FALSE, FALSE, 5);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(vbuttonbox2), GTK_BUTTONBOX_SPREAD);
#if HAVE_GTK3 == 0
    button_File_Move_Up = gtk_button_new_from_stock(GTK_STOCK_GO_UP);
#else
#if GTK_CHECK_VERSION(3,10,0)
    button_File_Move_Up = gtk_button_new_from_icon_name("go-up", GTK_ICON_SIZE_BUTTON);
#else
    button_File_Move_Up = gtk_button_new_from_stock(GTK_STOCK_GO_UP);
#endif
#endif
    gtk_widget_show(button_File_Move_Up);
    gtk_container_add(GTK_CONTAINER(vbuttonbox2), button_File_Move_Up);
#if HAVE_GTK3 == 0
    gtk_tooltips_set_tip(tooltips, button_File_Move_Up, _("Move selected file up in the playlist"), NULL);
#else
    gtk_widget_set_tooltip_text(button_File_Move_Up, _("Move selected file up in the playlist"));
#endif

#if HAVE_GTK3 == 0
    button_File_Move_Down = gtk_button_new_from_stock(GTK_STOCK_GO_DOWN);
#else 
#if GTK_CHECK_VERSION(3,10,0)
    button_File_Move_Down = gtk_button_new_from_icon_name("go-down", GTK_ICON_SIZE_BUTTON);
#else
    button_File_Move_Down = gtk_button_new_from_stock(GTK_STOCK_GO_DOWN);
#endif
#endif
    gtk_widget_show(button_File_Move_Down);
    gtk_container_add(GTK_CONTAINER(vbuttonbox2), button_File_Move_Down);
#if HAVE_GTK3 == 0
    gtk_tooltips_set_tip(tooltips, button_File_Move_Down, _("Move selected file down in the playlist"), NULL);
#else
    gtk_widget_set_tooltip_text(button_File_Move_Down, _("Move selected file down in the playlist"));
#endif
#if HAVE_GTK3 == 0
    hbuttonbox1 = gtk_hbutton_box_new();
#else
    hbuttonbox1 = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
#endif
    gtk_widget_show(hbuttonbox1);
    gtk_box_pack_start(GTK_BOX(vbox1), hbuttonbox1, FALSE, FALSE, 5);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(hbuttonbox1), GTK_BUTTONBOX_END);
#if HAVE_GTK3 == 0
    button_Close = gtk_button_new_from_stock(GTK_STOCK_CLOSE);
#else
    button_Close = gtk_button_new_with_label(_("Close"));
#endif
    gtk_widget_show(button_Close);
    gtk_container_add(GTK_CONTAINER(hbuttonbox1), button_Close);
    gtk_container_set_border_width(GTK_CONTAINER(button_Close), 5);

    g_signal_connect((gpointer) window_playlist, "destroy",
            G_CALLBACK(on_quitPlaylist_activate),
            NULL);

    g_signal_connect((gpointer) button_Close, "clicked",
            G_CALLBACK(on_quitPlaylist_activate),
            NULL);

    g_signal_connect((gpointer) button_Add_Playlist, "clicked",
            G_CALLBACK(on_Playlist_NewPlaylistButton_activate),
            NULL);

    g_signal_connect((gpointer) button_Import_Playlist, "clicked",
            G_CALLBACK(on_Playlist_ImportPlaylistButton_activate),
            NULL);

    g_signal_connect((gpointer) button_Export_Playlist, "clicked",
            G_CALLBACK(on_Playlist_ExportPlaylistButton_activate),
            NULL);

    g_signal_connect((gpointer) button_Del_Playlist, "clicked",
            G_CALLBACK(on_Playlist_DelPlaylistButton_activate),
            NULL);

    g_signal_connect((gpointer) button_Del_File, "clicked",
            G_CALLBACK(on_Playlist_DelFileButton_activate),
            NULL);

    g_signal_connect((gpointer) button_Add_Files, "clicked",
            G_CALLBACK(on_Playlist_AddFileButton_activate),
            NULL);

    g_signal_connect((gpointer) button_File_Move_Up, "clicked",
            G_CALLBACK(on_Playlist_FileUpButton_activate),
            NULL);

    g_signal_connect((gpointer) button_File_Move_Down, "clicked",
            G_CALLBACK(on_Playlist_FileDownButton_activate),
            NULL);

    g_signal_connect((gpointer) comboboxentry_playlist, "changed",
            G_CALLBACK(on_Playlist_Combobox_activate),
            NULL);

    return window_playlist;
}
