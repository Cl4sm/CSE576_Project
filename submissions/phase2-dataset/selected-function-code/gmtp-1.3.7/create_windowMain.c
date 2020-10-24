    GtkWidget *vbox1;
    GtkWidget *menubarMain;
    GtkWidget *menuitem1;
    GtkWidget *menuitem1_menu;
    GtkWidget *menuseparator1;
    GtkWidget *menuseparator2;
    GtkWidget *menuseparator3;
    GtkWidget *menuseparator4;
    GtkWidget *menuseparator5;
    GtkWidget *menuseparator6;
    GtkWidget *menuseparator7;
    GtkWidget *menuseparator8;
    GtkWidget *menuseparator9;
    GtkWidget *menuseparator10;
    GtkWidget *quit1;
    GtkWidget *menuitem2;
    GtkWidget *menuitem2_menu;
    GtkWidget *preferences1;
    GtkWidget *menuView;
    GtkWidget *menuView_menu;

    GtkWidget *menuitem4;
    GtkWidget *menuitem4_menu;
    GtkWidget *about1;

    gint tmp_toolbar_icon_size;
    GtkWidget *tmp_image;
    GtkWidget *toolbuttonPreferences;
    GtkWidget *hpanel;

    GtkWidget *toolbarSeparator;
    GtkWidget *toolbarSeparator2;
    //GtkWidget *toolbuttonQuit;

    GtkWidget *FindToolbar_hbox_FindToolbar;
    GtkWidget *FindToolbar_label_FindLabel;
    GtkWidget *FindToolbar_FindButton;
    GtkWidget *FindToolbar_CloseButton;

    GtkAccelGroup *accel_group;

    accel_group = gtk_accel_group_new();

    windowMain = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    setWindowTitle(NULL);
    gtk_window_set_default_size(GTK_WINDOW(windowMain), 880, 400);
    gtk_window_set_icon_from_file(GTK_WINDOW(windowMain), file_icon48_png, NULL);
#if HAVE_GTK3 == 0
    vbox1 = gtk_vbox_new(FALSE, 0);
#else 
    vbox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
#endif
    gtk_widget_show(vbox1);
    gtk_container_add(GTK_CONTAINER(windowMain), vbox1);

    menubarMain = gtk_menu_bar_new();
    gtk_widget_show(menubarMain);
    gtk_box_pack_start(GTK_BOX(vbox1), menubarMain, FALSE, FALSE, 0);

    menuitem1 = gtk_menu_item_new_with_mnemonic(_("_File"));
    gtk_widget_show(menuitem1);
    gtk_container_add(GTK_CONTAINER(menubarMain), menuitem1);

    menuitem1_menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem1), menuitem1_menu);

    fileConnect = gtk_menu_item_new_with_mnemonic(_("_Connect Device"));
    gtk_widget_show(fileConnect);
    gtk_container_add(GTK_CONTAINER(menuitem1_menu), fileConnect);

    menuseparator4 = gtk_separator_menu_item_new();
    gtk_widget_show(menuseparator4);
    gtk_container_add(GTK_CONTAINER(menuitem1_menu), menuseparator4);

    fileAdd = gtk_menu_item_new_with_label(_("Add Files"));
    gtk_widget_show(fileAdd);
    gtk_container_add(GTK_CONTAINER(menuitem1_menu), fileAdd);

    fileRemove = gtk_menu_item_new_with_label(_("Delete Files"));
    gtk_widget_show(fileRemove);
    gtk_container_add(GTK_CONTAINER(menuitem1_menu), fileRemove);

    fileRename = gtk_menu_item_new_with_label(_("Rename Files"));
    gtk_widget_show(fileRename);
    gtk_container_add(GTK_CONTAINER(menuitem1_menu), fileRename);

    fileMove = gtk_menu_item_new_with_label(_("Move To..."));
    gtk_widget_show(fileMove);
    gtk_container_add(GTK_CONTAINER(menuitem1_menu), fileMove);

    fileDownload = gtk_menu_item_new_with_label(_("Download Files"));
    gtk_widget_show(fileDownload);
    gtk_container_add(GTK_CONTAINER(menuitem1_menu), fileDownload);

    menuseparator1 = gtk_separator_menu_item_new();
    gtk_widget_show(menuseparator1);
    gtk_container_add(GTK_CONTAINER(menuitem1_menu), menuseparator1);

    fileNewFolder = gtk_menu_item_new_with_label(_("Create Folder"));
    gtk_widget_show(fileNewFolder);
    gtk_container_add(GTK_CONTAINER(menuitem1_menu), fileNewFolder);

    fileRemoveFolder = gtk_menu_item_new_with_label(_("Delete Folder"));
    gtk_widget_show(fileRemoveFolder);
    gtk_container_add(GTK_CONTAINER(menuitem1_menu), fileRemoveFolder);

    menuseparator2 = gtk_separator_menu_item_new();
    gtk_widget_show(menuseparator2);
    gtk_container_add(GTK_CONTAINER(menuitem1_menu), menuseparator2);

    fileRescan = gtk_menu_item_new_with_label(_("Refresh Folder"));
    //menuText = gtk_bin_get_child(GTK_BIN(fileRescan));
    //gtk_label_set_text(GTK_LABEL(menuText), _("Refresh Device"));
    gtk_widget_show(fileRescan);
    gtk_container_add(GTK_CONTAINER(menuitem1_menu), fileRescan);

    properties1 = gtk_menu_item_new_with_label(_("Device Properties"));
    //menuText = gtk_bin_get_child(GTK_BIN(properties1));
    //gtk_label_set_text(GTK_LABEL(menuText), _("Device Properties"));
    gtk_widget_show(properties1);
    gtk_container_add(GTK_CONTAINER(menuitem1_menu), properties1);

    menuseparator3 = gtk_separator_menu_item_new();
    gtk_widget_show(menuseparator3);
    gtk_container_add(GTK_CONTAINER(menuitem1_menu), menuseparator3);

    quit1 = gtk_menu_item_new_with_mnemonic(_("_Quit"));
    gtk_widget_show(quit1);
    gtk_container_add(GTK_CONTAINER(menuitem1_menu), quit1);

    menuitem2 = gtk_menu_item_new_with_mnemonic(_("_Edit"));
    gtk_widget_show(menuitem2);
    gtk_container_add(GTK_CONTAINER(menubarMain), menuitem2);

    menuitem2_menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem2), menuitem2_menu);

    editSelectAll = gtk_menu_item_new_with_label(_("Select All"));
    gtk_widget_show(editSelectAll);
    gtk_container_add(GTK_CONTAINER(menuitem2_menu), editSelectAll);

    menuseparator6 = gtk_separator_menu_item_new();
    gtk_widget_show(menuseparator6);
    gtk_container_add(GTK_CONTAINER(menuitem2_menu), menuseparator6);

    editFind = gtk_menu_item_new_with_label(_("Find"));
    gtk_widget_show(editFind);
    gtk_container_add(GTK_CONTAINER(menuitem2_menu), editFind);

    menuseparator9 = gtk_separator_menu_item_new();
    gtk_widget_show(menuseparator9);
    gtk_container_add(GTK_CONTAINER(menuitem2_menu), menuseparator9);

    editDeviceName = gtk_menu_item_new_with_label(_("Change Device Name"));
    gtk_widget_show(editDeviceName);
    gtk_container_add(GTK_CONTAINER(menuitem2_menu), editDeviceName);

    editFormatDevice = gtk_menu_item_new_with_label(_("Format Device"));
    gtk_widget_show(editFormatDevice);
    gtk_container_add(GTK_CONTAINER(menuitem2_menu), editFormatDevice);

    menuseparator7 = gtk_separator_menu_item_new();
    gtk_widget_show(menuseparator7);
    gtk_container_add(GTK_CONTAINER(menuitem2_menu), menuseparator7);

    editAddAlbumArt = gtk_menu_item_new_with_label(_("Album Art"));
    gtk_widget_show(editAddAlbumArt);
    gtk_container_add(GTK_CONTAINER(menuitem2_menu), editAddAlbumArt);

    editPlaylist = gtk_menu_item_new_with_label(_("Edit Playlist(s)"));
    gtk_widget_show(editPlaylist);
    gtk_container_add(GTK_CONTAINER(menuitem2_menu), editPlaylist);

    menuseparator5 = gtk_separator_menu_item_new();
    gtk_widget_show(menuseparator5);
    gtk_container_add(GTK_CONTAINER(menuitem2_menu), menuseparator5);

    preferences1 = gtk_menu_item_new_with_mnemonic(_("_Preferences"));
    gtk_widget_show(preferences1);
    gtk_container_add(GTK_CONTAINER(menuitem2_menu), preferences1);

    menuView = gtk_menu_item_new_with_mnemonic(_("_View"));
    gtk_widget_show(menuView);
    gtk_container_add(GTK_CONTAINER(menubarMain), menuView);


    menuView_menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuView), menuView_menu);

    menu_view_toolbar = gtk_check_menu_item_new_with_label(_("Toolbar"));
    gtk_widget_show(menu_view_toolbar);
    gtk_container_add(GTK_CONTAINER(menuView_menu), menu_view_toolbar);

    menuseparator10 = gtk_separator_menu_item_new();
    gtk_widget_show(menuseparator10);
    gtk_container_add(GTK_CONTAINER(menuView_menu), menuseparator10);

    menu_view_folders = gtk_check_menu_item_new_with_label(_("Folders"));
    gtk_widget_show(menu_view_folders);
    gtk_container_add(GTK_CONTAINER(menuView_menu), menu_view_folders);

    menuseparator8 = gtk_separator_menu_item_new();
    gtk_widget_show(menuseparator8);
    gtk_container_add(GTK_CONTAINER(menuView_menu), menuseparator8);

    menu_view_filesize = gtk_check_menu_item_new_with_label(_("File Size"));
    gtk_widget_show(menu_view_filesize);
    gtk_container_add(GTK_CONTAINER(menuView_menu), menu_view_filesize);

    menu_view_filetype = gtk_check_menu_item_new_with_label(_("File Type"));
    gtk_widget_show(menu_view_filetype);
    gtk_container_add(GTK_CONTAINER(menuView_menu), menu_view_filetype);

    menu_view_track_number = gtk_check_menu_item_new_with_label(_("Track Number"));
    gtk_widget_show(menu_view_track_number);
    gtk_container_add(GTK_CONTAINER(menuView_menu), menu_view_track_number);

    menu_view_title = gtk_check_menu_item_new_with_label(_("Track Name"));
    gtk_widget_show(menu_view_title);
    gtk_container_add(GTK_CONTAINER(menuView_menu), menu_view_title);

    menu_view_artist = gtk_check_menu_item_new_with_label(_("Artist"));
    gtk_widget_show(menu_view_artist);
    gtk_container_add(GTK_CONTAINER(menuView_menu), menu_view_artist);

    menu_view_album = gtk_check_menu_item_new_with_label(_("Album"));
    gtk_widget_show(menu_view_album);
    gtk_container_add(GTK_CONTAINER(menuView_menu), menu_view_album);

    menu_view_year = gtk_check_menu_item_new_with_label(_("Year"));
    gtk_widget_show(menu_view_year);
    gtk_container_add(GTK_CONTAINER(menuView_menu), menu_view_year);

    menu_view_genre = gtk_check_menu_item_new_with_label(_("Genre"));
    gtk_widget_show(menu_view_genre);
    gtk_container_add(GTK_CONTAINER(menuView_menu), menu_view_genre);

    menu_view_duration = gtk_check_menu_item_new_with_label(_("Duration"));
    gtk_widget_show(menu_view_duration);
    gtk_container_add(GTK_CONTAINER(menuView_menu), menu_view_duration);

    menuitem4 = gtk_menu_item_new_with_mnemonic(_("_Help"));
    gtk_widget_show(menuitem4);
    gtk_container_add(GTK_CONTAINER(menubarMain), menuitem4);

    menuitem4_menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem4), menuitem4_menu);

    about1 = gtk_menu_item_new_with_mnemonic(_("_About"));
    gtk_widget_show(about1);
    gtk_container_add(GTK_CONTAINER(menuitem4_menu), about1);

#if HAVE_GTK3 == 0
    handlebox1 = gtk_handle_box_new();
    gtk_handle_box_set_shadow_type(GTK_HANDLE_BOX(handlebox1), GTK_SHADOW_ETCHED_OUT);
#else 
    handlebox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
#endif
    gtk_widget_show(handlebox1);
    gtk_box_pack_start(GTK_BOX(vbox1), handlebox1, FALSE, FALSE, 0);

#if HAVE_GTK3 == 0
    tooltipsToolbar = gtk_tooltips_new();
#endif

    toolbarMain = gtk_toolbar_new();
    gtk_widget_show(toolbarMain);
#if HAVE_GTK3 == 0
    gtk_container_add(GTK_CONTAINER(handlebox1), toolbarMain);
#else
    gtk_box_pack_start(GTK_BOX(handlebox1), toolbarMain, TRUE, TRUE, 0);
#endif
    if(Preferences.toolbarStyle != NULL){
        if(g_ascii_strcasecmp(Preferences.toolbarStyle->str, "icon") == 0){
            gtk_toolbar_set_style(GTK_TOOLBAR(toolbarMain), GTK_TOOLBAR_ICONS);
        } else if(g_ascii_strcasecmp(Preferences.toolbarStyle->str, "text") == 0){
            gtk_toolbar_set_style(GTK_TOOLBAR(toolbarMain), GTK_TOOLBAR_TEXT);
        } else if(g_ascii_strcasecmp(Preferences.toolbarStyle->str, "both") == 0){
            gtk_toolbar_set_style(GTK_TOOLBAR(toolbarMain), GTK_TOOLBAR_BOTH);
        }
    } else {
        gtk_toolbar_set_style(GTK_TOOLBAR(toolbarMain), GTK_TOOLBAR_BOTH);
    }
    tmp_toolbar_icon_size = gtk_toolbar_get_icon_size(GTK_TOOLBAR(toolbarMain));

#if HAVE_GTK3 == 0
    gtk_toolbar_set_tooltips(GTK_TOOLBAR(toolbarMain), TRUE);
#else
    g_object_set(gtk_settings_get_default(), "gtk-enable-tooltips", TRUE, NULL);
#endif

#if HAVE_GTK3 == 0
    tmp_image = gtk_image_new_from_stock(GTK_STOCK_NETWORK, tmp_toolbar_icon_size);
#else
    tmp_image = gtk_image_new_from_icon_name("network-workgroup", tmp_toolbar_icon_size);
#endif
    gtk_widget_show(tmp_image);
    toolbuttonConnect = (GtkWidget*) gtk_tool_button_new(tmp_image, _("Connect"));
    gtk_widget_show(toolbuttonConnect);
    gtk_container_add(GTK_CONTAINER(toolbarMain), toolbuttonConnect);
#if HAVE_GTK3 == 0
    gtk_tool_item_set_tooltip(GTK_TOOL_ITEM(toolbuttonConnect), GTK_TOOLTIPS(tooltipsToolbar), _("Connect/Disconnect to your device."), _("Connect/Disconnect to your device."));
#else
    gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(toolbuttonConnect), _("Connect/Disconnect to your device."));
#endif

    toolbarSeparator = (GtkWidget*) gtk_separator_tool_item_new();
    gtk_separator_tool_item_set_draw(GTK_SEPARATOR_TOOL_ITEM(toolbarSeparator), TRUE);
    gtk_widget_show(toolbarSeparator);
    gtk_container_add(GTK_CONTAINER(toolbarMain), toolbarSeparator);

#if HAVE_GTK3 == 0
    tmp_image = gtk_image_new_from_stock(GTK_STOCK_ADD, tmp_toolbar_icon_size);
#else
    tmp_image = gtk_image_new_from_icon_name("list-add", tmp_toolbar_icon_size);
#endif
    gtk_widget_show(tmp_image);
    toolbuttonAddFile = (GtkWidget*) gtk_tool_button_new(tmp_image, _("Add"));
    gtk_widget_show(toolbuttonAddFile);
    gtk_container_add(GTK_CONTAINER(toolbarMain), toolbuttonAddFile);
#if HAVE_GTK3 == 0
    gtk_tool_item_set_tooltip(GTK_TOOL_ITEM(toolbuttonAddFile), GTK_TOOLTIPS(tooltipsToolbar), _("Add Files to your device."), _("Add a varity of Files to your device in the current folder."));
#else
    gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(toolbuttonAddFile), _("Add Files to your device."));
#endif
#if HAVE_GTK3 == 0
    tmp_image = gtk_image_new_from_stock(GTK_STOCK_REMOVE, tmp_toolbar_icon_size);
#else
    tmp_image = gtk_image_new_from_icon_name("list-remove", tmp_toolbar_icon_size);
#endif
    gtk_widget_show(tmp_image);
    toolbuttonRemoveFile = (GtkWidget*) gtk_tool_button_new(tmp_image, _("Delete"));
    gtk_widget_show(toolbuttonRemoveFile);
    gtk_container_add(GTK_CONTAINER(toolbarMain), toolbuttonRemoveFile);
#if HAVE_GTK3 == 0
    gtk_tool_item_set_tooltip(GTK_TOOL_ITEM(toolbuttonRemoveFile), GTK_TOOLTIPS(tooltipsToolbar), _("Delete Files/Folders from your device."), _("Permanently remove files/folders from your device. Note: Albums are stored as *.alb files."));
#else
    gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(toolbuttonRemoveFile), _("Delete Files/Folders from your device."));
#endif
#if HAVE_GTK3 == 0
    tmp_image = gtk_image_new_from_stock(GTK_STOCK_GOTO_BOTTOM, tmp_toolbar_icon_size);
#else
    tmp_image = gtk_image_new_from_icon_name("go-bottom", tmp_toolbar_icon_size);
#endif
    gtk_widget_show(tmp_image);
    toolbuttonRetrieve = (GtkWidget*) gtk_tool_button_new(tmp_image, _("Download"));
    gtk_widget_show(toolbuttonRetrieve);
    gtk_container_add(GTK_CONTAINER(toolbarMain), toolbuttonRetrieve);
#if HAVE_GTK3 == 0
    gtk_tool_item_set_tooltip(GTK_TOOL_ITEM(toolbuttonRetrieve), GTK_TOOLTIPS(tooltipsToolbar), _("Download Files from your device to your Host PC."), _("Download files from your device to your PC. Default Download path is set in the preferences dialog."));
#else
    gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(toolbuttonRetrieve), _("Download Files from your device to your Host PC."));
#endif

    toolbarSeparator2 = (GtkWidget*) gtk_separator_tool_item_new();
    gtk_separator_tool_item_set_draw(GTK_SEPARATOR_TOOL_ITEM(toolbarSeparator2), TRUE);
    gtk_widget_show(toolbarSeparator2);
    gtk_container_add(GTK_CONTAINER(toolbarMain), toolbarSeparator2);

#if HAVE_GTK3 == 0
    tmp_image = gtk_image_new_from_stock(GTK_STOCK_CDROM, tmp_toolbar_icon_size);
#else
    tmp_image = gtk_image_new_from_icon_name("media-optical", tmp_toolbar_icon_size);
#endif
    gtk_widget_show(tmp_image);
    toolbuttonAlbumArt = (GtkWidget*) gtk_tool_button_new(tmp_image, _("Album Art"));
    gtk_widget_show(toolbuttonAlbumArt);
    gtk_container_add(GTK_CONTAINER(toolbarMain), toolbuttonAlbumArt);
#if HAVE_GTK3 == 0
    gtk_tool_item_set_tooltip(GTK_TOOL_ITEM(toolbuttonAlbumArt), GTK_TOOLTIPS(tooltipsToolbar), _("Upload an image file as Album Art."), _("Upload a JPG file and assign it as Album Art."));
#else
    gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(toolbuttonAlbumArt), _("Upload an image file as Album Art."));
#endif
#if HAVE_GTK3 == 0
    tmp_image = gtk_image_new_from_stock(GTK_STOCK_DND_MULTIPLE, tmp_toolbar_icon_size);
#else
    tmp_image = gtk_image_new_from_icon_name("emblem-documents", tmp_toolbar_icon_size);
#endif
    gtk_widget_show(tmp_image);
    toolbuttonPlaylist = (GtkWidget*) gtk_tool_button_new(tmp_image, _("Playlists"));
    gtk_widget_show(toolbuttonPlaylist);
    gtk_container_add(GTK_CONTAINER(toolbarMain), toolbuttonPlaylist);
#if HAVE_GTK3 == 0
    gtk_tool_item_set_tooltip(GTK_TOOL_ITEM(toolbuttonPlaylist), GTK_TOOLTIPS(tooltipsToolbar), _("Add and Modify Playlists."), _("Add and Modify Playlists."));
#else
    gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(toolbuttonPlaylist), _("Add and Modify Playlists."));
#endif
#if HAVE_GTK3 == 0
    tmp_image = gtk_image_new_from_stock(GTK_STOCK_REFRESH, tmp_toolbar_icon_size);
#else
    tmp_image = gtk_image_new_from_icon_name("view-refresh", tmp_toolbar_icon_size);
#endif
    gtk_widget_show(tmp_image);
    toolbuttonRescan = (GtkWidget*) gtk_tool_button_new(tmp_image, _("Refresh"));
    gtk_widget_show(toolbuttonRescan);
    gtk_container_add(GTK_CONTAINER(toolbarMain), toolbuttonRescan);
#if HAVE_GTK3 == 0
    gtk_tool_item_set_tooltip(GTK_TOOL_ITEM(toolbuttonRescan), GTK_TOOLTIPS(tooltipsToolbar), _("Refresh File/Folder listing."), _("Refresh File/Folder listing."));
#else
    gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(toolbuttonRescan), _("Refresh File/Folder listing."));
#endif

    toolbarSeparator = (GtkWidget*) gtk_separator_tool_item_new();
    gtk_separator_tool_item_set_draw(GTK_SEPARATOR_TOOL_ITEM(toolbarSeparator), TRUE);
    gtk_widget_show(toolbarSeparator);
    gtk_container_add(GTK_CONTAINER(toolbarMain), toolbarSeparator);

#if HAVE_GTK3 == 0
    tmp_image = gtk_image_new_from_stock(GTK_STOCK_PROPERTIES, tmp_toolbar_icon_size);
#else
    tmp_image = gtk_image_new_from_icon_name("document-properties", tmp_toolbar_icon_size);
#endif
    gtk_widget_show(tmp_image);
    toolbuttonProperties = (GtkWidget*) gtk_tool_button_new(tmp_image, _("Properties"));
    gtk_widget_show(toolbuttonProperties);
    gtk_container_add(GTK_CONTAINER(toolbarMain), toolbuttonProperties);
#if HAVE_GTK3 == 0
    gtk_tool_item_set_tooltip(GTK_TOOL_ITEM(toolbuttonProperties), GTK_TOOLTIPS(tooltipsToolbar), _("View Device Properties."), _("View Device Properties."));
#else
    gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(toolbuttonProperties), _("View Device Properties."));
#endif
#if HAVE_GTK3 == 0
    tmp_image = gtk_image_new_from_stock(GTK_STOCK_PREFERENCES, tmp_toolbar_icon_size);
#else
    tmp_image = gtk_image_new_from_icon_name("preferences-system", tmp_toolbar_icon_size);
#endif
    gtk_widget_show(tmp_image);
    toolbuttonPreferences = (GtkWidget*) gtk_tool_button_new(tmp_image, _("Preferences"));
    gtk_widget_show(toolbuttonPreferences);
    gtk_container_add(GTK_CONTAINER(toolbarMain), toolbuttonPreferences);
#if HAVE_GTK3 == 0
    gtk_tool_item_set_tooltip(GTK_TOOL_ITEM(toolbuttonPreferences), GTK_TOOLTIPS(tooltipsToolbar), _("View/Change gMTP Preferences."), _("View/Change gMTP Preferences."));
#else
    gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(toolbuttonPreferences), _("View/Change gMTP Preferences."));
#endif

/*    toolbarSeparator = (GtkWidget*) gtk_separator_tool_item_new();
    gtk_separator_tool_item_set_draw(GTK_SEPARATOR_TOOL_ITEM(toolbarSeparator), TRUE);
    gtk_widget_show(toolbarSeparator);
    gtk_container_add(GTK_CONTAINER(toolbarMain), toolbarSeparator);
#if HAVE_GTK3 == 0
    tmp_image = gtk_image_new_from_stock(GTK_STOCK_QUIT, tmp_toolbar_icon_size);
#else
    tmp_image = gtk_image_new_from_icon_name("application-exit", tmp_toolbar_icon_size);
#endif
    gtk_widget_show(tmp_image);
    toolbuttonQuit = (GtkWidget*) gtk_tool_button_new(tmp_image, _("Quit"));
    gtk_widget_show(toolbuttonQuit);
    gtk_container_add(GTK_CONTAINER(toolbarMain), toolbuttonQuit);
#if HAVE_GTK3 == 0
    gtk_tool_item_set_tooltip(GTK_TOOL_ITEM(toolbuttonQuit), GTK_TOOLTIPS(tooltipsToolbar), _("Quit gMTP."), _("Quit"));
#else
    gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(toolbuttonQuit), _("Quit gMTP."));
#endif
*/
#if HAVE_GTK3 == 0
    gtk_tooltips_enable(tooltipsToolbar);
#endif

    // Find toolbar;
#if HAVE_GTK3 == 0
    findToolbar = gtk_handle_box_new();
    //gtk_widget_show(findToolbar); // Only show when the user selects the menu option.
    gtk_handle_box_set_shadow_type(GTK_HANDLE_BOX(findToolbar), GTK_SHADOW_ETCHED_OUT);
#else 
    findToolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
#endif
    gtk_box_pack_start(GTK_BOX(vbox1), findToolbar, FALSE, FALSE, 0);

#if HAVE_GTK3 == 0
    FindToolbar_hbox_FindToolbar = gtk_hbox_new(FALSE, 5);
#else 
    FindToolbar_hbox_FindToolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
#endif
    gtk_widget_show(FindToolbar_hbox_FindToolbar);
    gtk_container_add(GTK_CONTAINER(findToolbar), FindToolbar_hbox_FindToolbar);
    gtk_container_set_border_width(GTK_CONTAINER(FindToolbar_hbox_FindToolbar), 2);

    FindToolbar_label_FindLabel = gtk_label_new(_("Find:"));
    gtk_widget_show(FindToolbar_label_FindLabel);
    gtk_box_pack_start(GTK_BOX(FindToolbar_hbox_FindToolbar), FindToolbar_label_FindLabel, FALSE, FALSE, 0);
    gtk_label_set_justify(GTK_LABEL(FindToolbar_label_FindLabel), GTK_JUSTIFY_RIGHT);

    FindToolbar_entry_FindText = gtk_entry_new();
    gtk_widget_show(FindToolbar_entry_FindText);
    gtk_box_pack_start(GTK_BOX(FindToolbar_hbox_FindToolbar), FindToolbar_entry_FindText, TRUE, TRUE, 0);
    gtk_entry_set_max_length(GTK_ENTRY(FindToolbar_entry_FindText), 256);
#if HAVE_GTK3 == 0
    FindToolbar_FindButton = gtk_button_new_from_stock(GTK_STOCK_FIND);
#else
#if GTK_CHECK_VERSION(3,10,0)
    FindToolbar_FindButton = gtk_button_new_from_icon_name("edit-find", tmp_toolbar_icon_size);
#else
    FindToolbar_FindButton = gtk_button_new_from_stock(GTK_STOCK_FIND);
#endif
#endif
    gtk_widget_show(FindToolbar_FindButton);
    gtk_box_pack_start(GTK_BOX(FindToolbar_hbox_FindToolbar), FindToolbar_FindButton, FALSE, FALSE, 5);
    //gtk_container_set_border_width(GTK_CONTAINER(FindToolbar_FindButton), 5);

    FindToolbar_checkbutton_FindFiles = gtk_check_button_new_with_mnemonic(_("Filenames"));
    gtk_widget_show(FindToolbar_checkbutton_FindFiles);
    gtk_box_pack_start(GTK_BOX(FindToolbar_hbox_FindToolbar), FindToolbar_checkbutton_FindFiles, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(FindToolbar_checkbutton_FindFiles), TRUE);

    FindToolbar_checkbutton_TrackInformation = gtk_check_button_new_with_mnemonic(_("Track Information"));
    gtk_widget_show(FindToolbar_checkbutton_TrackInformation);
    gtk_box_pack_start(GTK_BOX(FindToolbar_hbox_FindToolbar), FindToolbar_checkbutton_TrackInformation, FALSE, FALSE, 0);
#if HAVE_GTK3 == 0
    tmp_image = gtk_image_new_from_stock(GTK_STOCK_CLOSE, tmp_toolbar_icon_size);
#else
    tmp_image = gtk_image_new_from_icon_name("window-close", tmp_toolbar_icon_size);
#endif

    gtk_widget_show(tmp_image);
    FindToolbar_CloseButton = (GtkWidget*) gtk_button_new();
    gtk_container_add(GTK_CONTAINER(FindToolbar_CloseButton), tmp_image);
    gtk_widget_show(FindToolbar_CloseButton);
    gtk_box_pack_start(GTK_BOX(FindToolbar_hbox_FindToolbar), FindToolbar_CloseButton, FALSE, FALSE, 5);
    gtk_button_set_relief(GTK_BUTTON(FindToolbar_CloseButton), GTK_RELIEF_NONE);

    // Main Window.

    // Hpane for showing both folders and main window.

#if HAVE_GTK3 == 0
    hpanel = gtk_hpaned_new();
#else
    hpanel = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
#endif
    gtk_widget_show(hpanel);
    gtk_box_pack_start(GTK_BOX(vbox1), hpanel, TRUE, TRUE, 0);
    gtk_paned_set_position(GTK_PANED(hpanel), 150);


    // Folder Window

    scrolledwindowFolders = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_hide(scrolledwindowFolders);
    gtk_paned_pack1(GTK_PANED(hpanel), scrolledwindowFolders, TRUE, TRUE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindowFolders), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    treeviewFolders = gtk_tree_view_new();
    gtk_widget_show(treeviewFolders);
    gtk_container_add(GTK_CONTAINER(scrolledwindowFolders), treeviewFolders);
    gtk_container_set_border_width(GTK_CONTAINER(treeviewFolders), 5);
    folderSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeviewFolders));
    gtk_tree_selection_set_mode(folderSelection, GTK_SELECTION_SINGLE);

    folderList = gtk_tree_store_new(NUM_FOL_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_UINT, GDK_TYPE_PIXBUF);
    folderColumn = setupFolderList(treeviewFolders);

    folderListModel = gtk_tree_model_sort_new_with_model(GTK_TREE_MODEL(folderList));
    gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(folderListModel),
            COL_FOL_NAME_HIDDEN, GTK_SORT_ASCENDING);

    gtk_tree_view_set_model(GTK_TREE_VIEW(treeviewFolders), GTK_TREE_MODEL(folderListModel));
    gtk_tree_view_expand_all(GTK_TREE_VIEW(treeviewFolders));
    g_object_unref(folderList);

    // Main file window

    scrolledwindowMain = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_show(scrolledwindowMain);
    gtk_paned_pack2(GTK_PANED(hpanel), scrolledwindowMain, TRUE, TRUE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindowMain), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    treeviewFiles = gtk_tree_view_new();
    gtk_widget_show(treeviewFiles);
    gtk_container_add(GTK_CONTAINER(scrolledwindowMain), treeviewFiles);
    gtk_container_set_border_width(GTK_CONTAINER(treeviewFiles), 5);
    fileSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeviewFiles));
    gtk_tree_selection_set_mode(fileSelection, GTK_SELECTION_MULTIPLE);

    fileList = gtk_list_store_new(NUM_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
            G_TYPE_UINT, G_TYPE_BOOLEAN, G_TYPE_UINT64, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_STRING,
            G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_UINT, GDK_TYPE_PIXBUF,
            G_TYPE_STRING);
    setupFileList(treeviewFiles);

    fileListModel = gtk_tree_model_sort_new_with_model(GTK_TREE_MODEL(fileList));
    gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(fileListModel),
            COL_FILENAME_HIDDEN, GTK_SORT_ASCENDING);

    gtk_tree_view_set_model(GTK_TREE_VIEW(treeviewFiles), GTK_TREE_MODEL(fileListModel));
    g_object_unref(fileList);

    windowStatusBar = gtk_statusbar_new();
    gtk_widget_show(windowStatusBar);
    gtk_box_pack_start(GTK_BOX(vbox1), windowStatusBar, FALSE, FALSE, 0);

    // Build our right-click context menu;
    contextMenu = create_windowMainContextMenu();
    contextMenuColumn = create_windowMainColumnContextMenu();
    contextMenuFolder = create_windowFolderContextMenu();

    // DnD functions

    g_signal_connect((gpointer) scrolledwindowMain, "drag-data-received",
            G_CALLBACK(gmtp_drag_data_received), NULL);

    g_signal_connect((gpointer) treeviewFolders, "drag-data-received",
            G_CALLBACK(gmtpfolders_drag_data_received), NULL);

    g_signal_connect((gpointer) treeviewFolders, "drag-motion",
            G_CALLBACK(gmtpfolders_drag_motion_received), NULL);

    // End Dnd functions

    g_signal_connect((gpointer) windowMain, "destroy",
            G_CALLBACK(on_quit1_activate),
            NULL);

    g_signal_connect((gpointer) properties1, "activate",
            G_CALLBACK(on_deviceProperties_activate),
            NULL);
    g_signal_connect((gpointer) toolbuttonProperties, "clicked",
            G_CALLBACK(on_deviceProperties_activate),
            NULL);

    g_signal_connect((gpointer) quit1, "activate",
            G_CALLBACK(on_quit1_activate),
            NULL);

    g_signal_connect((gpointer) preferences1, "activate",
            G_CALLBACK(on_preferences1_activate),
            NULL);

    g_signal_connect((gpointer) editFind, "activate",
            G_CALLBACK(on_editFind_activate),
            NULL);

    g_signal_connect((gpointer) editSelectAll, "activate",
            G_CALLBACK(on_editSelectAll_activate),
            NULL);

    g_signal_connect((gpointer) editDeviceName, "activate",
            G_CALLBACK(on_editDeviceName_activate),
            NULL);

    g_signal_connect((gpointer) editFormatDevice, "activate",
            G_CALLBACK(on_editFormatDevice_activate),
            NULL);

    g_signal_connect((gpointer) editAddAlbumArt, "activate",
            G_CALLBACK(on_editAddAlbumArt_activate),
            NULL);

    g_signal_connect((gpointer) editPlaylist, "activate",
            G_CALLBACK(on_editPlaylist_activate),
            NULL);

    g_signal_connect((gpointer) fileAdd, "activate",
            G_CALLBACK(on_filesAdd_activate),
            NULL);

    g_signal_connect((gpointer) fileDownload, "activate",
            G_CALLBACK(on_filesDownload_activate),
            NULL);

    g_signal_connect((gpointer) fileRemove, "activate",
            G_CALLBACK(on_filesDelete_activate),
            NULL);

    g_signal_connect((gpointer) fileRename, "activate",
            G_CALLBACK(on_fileRenameFile_activate),
            NULL);

    g_signal_connect((gpointer) fileMove, "activate",
            G_CALLBACK(on_fileMoveFile_activate),
            NULL);

    g_signal_connect((gpointer) fileConnect, "activate",
            G_CALLBACK(on_deviceConnect_activate),
            NULL);

    g_signal_connect((gpointer) fileNewFolder, "activate",
            G_CALLBACK(on_fileNewFolder_activate),
            NULL);

    g_signal_connect((gpointer) fileRemoveFolder, "activate",
            G_CALLBACK(on_fileRemoveFolder_activate),
            NULL);

    g_signal_connect((gpointer) fileRescan, "activate",
            G_CALLBACK(on_deviceRescan_activate),
            NULL);

    g_signal_connect((gpointer) about1, "activate",
            G_CALLBACK(on_about1_activate),
            NULL);

/*    g_signal_connect((gpointer) toolbuttonQuit, "clicked",
            G_CALLBACK(on_quit1_activate),
            NULL);*/

    g_signal_connect((gpointer) toolbuttonRescan, "clicked",
            G_CALLBACK(on_deviceRescan_activate),
            NULL);

    g_signal_connect((gpointer) toolbuttonAddFile, "clicked",
            G_CALLBACK(on_filesAdd_activate),
            NULL);

    g_signal_connect((gpointer) toolbuttonRemoveFile, "clicked",
            G_CALLBACK(on_filesDelete_activate),
            NULL);

    g_signal_connect((gpointer) toolbuttonRetrieve, "clicked",
            G_CALLBACK(on_filesDownload_activate),
            NULL);

    g_signal_connect((gpointer) toolbuttonAlbumArt, "clicked",
            G_CALLBACK(on_editAddAlbumArt_activate),
            NULL);

    g_signal_connect((gpointer) toolbuttonPlaylist, "clicked",
            G_CALLBACK(on_editPlaylist_activate),
            NULL);

    g_signal_connect((gpointer) toolbuttonConnect, "clicked",
            G_CALLBACK(on_deviceConnect_activate),
            NULL);
    g_signal_connect((gpointer) toolbuttonPreferences, "clicked",
            G_CALLBACK(on_preferences1_activate),
            NULL);

    g_signal_connect((gpointer) treeviewFiles, "row-activated",
            G_CALLBACK(fileListRowActivated),
            NULL);

    g_signal_connect((gpointer) treeviewFolders, "row-activated",
            G_CALLBACK(folderListRowActivated),
            NULL);

    g_signal_connect_swapped(treeviewFiles, "button_press_event",
            G_CALLBACK(on_windowMainContextMenu_activate), contextMenu);

    g_signal_connect_swapped(treeviewFolders, "button_press_event",
            G_CALLBACK(on_windowMainContextMenu_activate), contextMenuFolder);

    g_signal_connect((gpointer) menu_view_filesize, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) menu_view_filetype, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) menu_view_track_number, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) menu_view_title, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) menu_view_artist, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) menu_view_album, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) menu_view_year, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) menu_view_genre, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) menu_view_duration, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) menu_view_folders, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);
            
    g_signal_connect((gpointer) menu_view_toolbar, "toggled",
            G_CALLBACK(on_view_activate),
            NULL);

    g_signal_connect((gpointer) FindToolbar_CloseButton, "clicked",
            G_CALLBACK(on_editFindClose_activate),
            NULL);

    g_signal_connect((gpointer) FindToolbar_FindButton, "clicked",
            G_CALLBACK(on_editFindSearch_activate),
            NULL);

    g_signal_connect((gpointer) FindToolbar_entry_FindText, "activate",
            G_CALLBACK(on_editFindSearch_activate),
            NULL);

    folderSelectHandler = g_signal_connect_after((gpointer) folderSelection, "changed",
            G_CALLBACK(on_treeviewFolders_rowactivated),
            NULL);

    fileSelectHandler = g_signal_connect_after((gpointer) fileSelection, "changed",
            G_CALLBACK(on_treeviewFolders_rowactivated),
            NULL);

    gtk_window_add_accel_group(GTK_WINDOW(windowMain), accel_group);
    gtk_menu_set_accel_group(GTK_MENU(menuitem1_menu), accel_group);
    gtk_widget_add_accelerator(fileRemove, "activate", accel_group, GDK_Delete, 0, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(fileRename, "activate", accel_group, GDK_F2, 0, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(fileConnect, "activate", accel_group, GDK_F3, 0, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(fileRescan, "activate", accel_group, GDK_F5, 0, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(fileNewFolder, "activate", accel_group, GDK_N, GDK_CONTROL_MASK + GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(fileAdd, "activate", accel_group, GDK_O, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(fileDownload, "activate", accel_group, GDK_D, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(editPlaylist, "activate", accel_group, GDK_P, GDK_CONTROL_MASK + GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(editAddAlbumArt, "activate", accel_group, GDK_A, GDK_CONTROL_MASK + GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(editSelectAll, "activate", accel_group, GDK_A, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(editFind, "activate", accel_group, GDK_F, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    return windowMain;
}
