    GtkWidget *windowDialog;
    GtkWidget *vbox1;
    GtkWidget *frame1;
    GtkWidget *alignment1;

    GtkWidget *frame3;
    GtkWidget *alignment3;
    GtkWidget *alignment4;
    GtkWidget *alignment7;
    GtkWidget *labelPlaylist;
    GtkWidget *frame4;
    GtkWidget *alignment5;
    GtkWidget *alignment6;
    GtkWidget *vbox4;
    GtkWidget *vbox2;
    GtkWidget *vbox5;
    GtkWidget *alignment8;
    GtkWidget *alignment9;

    GtkWidget *labelDevice;
    GtkWidget *frame2;
    GtkWidget *alignment2;
    GtkWidget *vbox3;
    
    GtkWidget *frame5;
    GtkWidget *alignment10;
    GtkWidget *labelApplication;
    GtkWidget *hbox2;
    GtkWidget *labelToolbarStyle;

    GtkWidget *table1;
    GtkWidget *labelDownloadPath;
    GtkWidget *labelUploadPath;

    GtkWidget *buttonDownloadPath;
    GtkWidget *buttonUploadPath;
    GtkWidget *labelFilePath;
    GtkWidget *hbox1;
    GtkWidget *buttonClose;

    windowDialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gchar * winTitle;
    winTitle = g_strconcat(PACKAGE_NAME, _(" Preferences"), NULL);
    gtk_window_set_title(GTK_WINDOW(windowDialog), winTitle);
    gtk_window_set_modal(GTK_WINDOW(windowDialog), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(windowDialog), GTK_WINDOW(windowMain));
    gtk_window_set_position(GTK_WINDOW(windowDialog), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_resizable(GTK_WINDOW(windowDialog), FALSE);
    gtk_window_set_type_hint(GTK_WINDOW(windowDialog), GDK_WINDOW_TYPE_HINT_DIALOG);
    g_free(winTitle);
#if HAVE_GTK3 == 0
    vbox1 = gtk_vbox_new(FALSE, 5);
#else 
    vbox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
#endif
    gtk_widget_show(vbox1);
    gtk_container_add(GTK_CONTAINER(windowDialog), vbox1);
    gtk_container_set_border_width(GTK_CONTAINER(vbox1), 5);

    // Application frame.

    frame5 = gtk_frame_new(NULL);
    gtk_widget_show(frame5);
    gtk_box_pack_start(GTK_BOX(vbox1), frame5, TRUE, TRUE, 0);
    gtk_frame_set_shadow_type(GTK_FRAME(frame5), GTK_SHADOW_NONE);
    
    labelApplication = gtk_label_new(_("<b>Application</b>"));
    gtk_widget_show(labelApplication);
    gtk_frame_set_label_widget(GTK_FRAME(frame5), labelApplication);
    gtk_label_set_use_markup(GTK_LABEL(labelApplication), TRUE);
    
    alignment10 = gtk_alignment_new(0.5, 0.5, 1, 1);
    gtk_widget_show(alignment10);
    gtk_container_add(GTK_CONTAINER(frame5), alignment10);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment10), 0, 0, 12, 0);
#if HAVE_GTK3 == 0
    hbox2 = gtk_hbox_new(FALSE, 0);
#else 
    hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
#endif
    gtk_widget_show(hbox2);
    gtk_container_add(GTK_CONTAINER(alignment10), hbox2);
    
    labelToolbarStyle = gtk_label_new(_("Toolbar Style:"));
    gtk_widget_show(labelToolbarStyle);
    gtk_box_pack_start(GTK_BOX(hbox2), labelToolbarStyle, FALSE, FALSE, 5);
    
#if HAVE_GTK3 == 0
    comboboxToolbarStyle = gtk_combo_box_new_text();
    gtk_widget_show(comboboxToolbarStyle);
    gtk_box_pack_end(GTK_BOX(hbox2), comboboxToolbarStyle, TRUE, TRUE, 5 );
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboboxToolbarStyle), _("both"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboboxToolbarStyle), _("icon"));
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboboxToolbarStyle), _("text"));
#else 
    comboboxToolbarStyle = gtk_combo_box_text_new();
    gtk_widget_show(comboboxToolbarStyle);
    gtk_box_pack_end(GTK_BOX(hbox2), comboboxToolbarStyle, TRUE, TRUE, 5 );
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboboxToolbarStyle), _("both"));
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboboxToolbarStyle), _("icon"));
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboboxToolbarStyle), _("text"));
#endif
    if(Preferences.toolbarStyle != NULL){
        if(g_ascii_strcasecmp(Preferences.toolbarStyle->str, ("both")) == 0){
            gtk_combo_box_set_active(GTK_COMBO_BOX(comboboxToolbarStyle), 0);
        } else if(g_ascii_strcasecmp(Preferences.toolbarStyle->str, ("icon")) == 0){
            gtk_combo_box_set_active(GTK_COMBO_BOX(comboboxToolbarStyle), 1);
        } else if(g_ascii_strcasecmp(Preferences.toolbarStyle->str, ("text")) == 0){
            gtk_combo_box_set_active(GTK_COMBO_BOX(comboboxToolbarStyle), 2);
        } else {
            gtk_combo_box_set_active(GTK_COMBO_BOX(comboboxToolbarStyle), 0);
        }
    }

    // Device Frame.

    frame1 = gtk_frame_new(NULL);
    gtk_widget_show(frame1);
    gtk_box_pack_start(GTK_BOX(vbox1), frame1, TRUE, TRUE, 0);
    gtk_frame_set_shadow_type(GTK_FRAME(frame1), GTK_SHADOW_NONE);


#if HAVE_GTK3 == 0
    vbox5 = gtk_vbox_new(FALSE, 5);
#else 
    vbox5 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
#endif
    gtk_widget_show(vbox5);
    gtk_container_add(GTK_CONTAINER(frame1), vbox5);

    alignment1 = gtk_alignment_new(0.5, 0.5, 1, 1);
    gtk_widget_show(alignment1);
    gtk_container_add(GTK_CONTAINER(vbox5), alignment1);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment1), 0, 0, 12, 0);

    alignment8 = gtk_alignment_new(0.5, 0.5, 1, 1);
    gtk_widget_show(alignment8);
    gtk_container_add(GTK_CONTAINER(vbox5), alignment8);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment8), 0, 0, 12, 0);

    checkbuttonDeviceConnect = gtk_check_button_new_with_mnemonic(_("Attempt to connect to Device on startup"));
    gtk_widget_show(checkbuttonDeviceConnect);
    gtk_container_add(GTK_CONTAINER(alignment1), checkbuttonDeviceConnect);

    checkbuttonAltAccessMethod = gtk_check_button_new_with_mnemonic(_("Utilize alternate access method"));
    gtk_widget_show(checkbuttonAltAccessMethod);
    gtk_container_add(GTK_CONTAINER(alignment8), checkbuttonAltAccessMethod);

    labelDevice = gtk_label_new(_("<b>Device</b>"));
    gtk_widget_show(labelDevice);
    gtk_frame_set_label_widget(GTK_FRAME(frame1), labelDevice);
    gtk_label_set_use_markup(GTK_LABEL(labelDevice), TRUE);

    frame3 = gtk_frame_new(NULL);
    gtk_widget_show(frame3);
    gtk_box_pack_start(GTK_BOX(vbox1), frame3, TRUE, TRUE, 0);
    gtk_frame_set_shadow_type(GTK_FRAME(frame3), GTK_SHADOW_NONE);
#if HAVE_GTK3 == 0
    vbox2 = gtk_vbox_new(FALSE, 5);
#else
    vbox2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
#endif
    gtk_widget_show(vbox2);
    gtk_container_add(GTK_CONTAINER(frame3), vbox2);

    alignment3 = gtk_alignment_new(0.5, 0.5, 1, 1);
    gtk_widget_show(alignment3);
    gtk_container_add(GTK_CONTAINER(vbox2), alignment3);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment3), 0, 0, 12, 0);

    alignment4 = gtk_alignment_new(0.5, 0.5, 1, 1);
    gtk_widget_show(alignment4);
    gtk_container_add(GTK_CONTAINER(vbox2), alignment4);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment4), 0, 0, 12, 0);

    alignment7 = gtk_alignment_new(0.5, 0.5, 1, 1);
    gtk_widget_show(alignment7);
    gtk_container_add(GTK_CONTAINER(vbox2), alignment7);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment7), 0, 0, 12, 0);
    
    alignment9 = gtk_alignment_new(0.5, 0.5, 1, 1);
    gtk_widget_show(alignment9);
    gtk_container_add(GTK_CONTAINER(vbox2), alignment9);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment9), 0, 0, 12, 0);

    checkbuttonConfirmFileOp = gtk_check_button_new_with_mnemonic(_("Confirm File/Folder Delete"));
    gtk_widget_show(checkbuttonConfirmFileOp);
    gtk_container_add(GTK_CONTAINER(alignment3), checkbuttonConfirmFileOp);

    checkbuttonConfirmOverWriteFileOp = gtk_check_button_new_with_mnemonic(_("Prompt if to Overwrite file if already exists"));
    gtk_widget_show(checkbuttonConfirmOverWriteFileOp);
    gtk_container_add(GTK_CONTAINER(alignment4), checkbuttonConfirmOverWriteFileOp);

    checkbuttonSuppressAlbumErrors = gtk_check_button_new_with_mnemonic(_("Suppress Album Errors"));
    gtk_widget_show(checkbuttonSuppressAlbumErrors);
    gtk_container_add(GTK_CONTAINER(alignment7), checkbuttonSuppressAlbumErrors);
    
    checkbuttonAllMediaAsFiles = gtk_check_button_new_with_mnemonic(_("Treat all media as regular files"));
    gtk_widget_show(checkbuttonAllMediaAsFiles);
    gtk_container_add(GTK_CONTAINER(alignment9), checkbuttonAllMediaAsFiles);

    // Playlist Frame.

    frame4 = gtk_frame_new(NULL);
    gtk_widget_show(frame4);
    gtk_box_pack_start(GTK_BOX(vbox1), frame4, TRUE, TRUE, 0);
    gtk_frame_set_shadow_type(GTK_FRAME(frame4), GTK_SHADOW_NONE);

    labelPlaylist = gtk_label_new(_("<b>Playlist</b>"));
    gtk_widget_show(labelPlaylist);
    gtk_frame_set_label_widget(GTK_FRAME(frame4), labelPlaylist);
    gtk_label_set_use_markup(GTK_LABEL(labelPlaylist), TRUE);
#if HAVE_GTK3 == 0
    vbox4 = gtk_vbox_new(FALSE, 5);
#else
    vbox4 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
#endif
    gtk_widget_show(vbox4);
    gtk_container_add(GTK_CONTAINER(frame4), vbox4);

    alignment5 = gtk_alignment_new(0.5, 0.5, 1, 1);
    gtk_widget_show(alignment5);
    gtk_container_add(GTK_CONTAINER(vbox4), alignment5);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment5), 0, 0, 12, 0);

    alignment6 = gtk_alignment_new(0.5, 0.5, 1, 1);
    gtk_widget_show(alignment6);
    gtk_container_add(GTK_CONTAINER(vbox4), alignment6);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment6), 0, 0, 12, 0);

    checkbuttonAutoAddTrackPlaylist = gtk_check_button_new_with_mnemonic(_("Prompt to add New Music track to existing playlist"));
    gtk_widget_show(checkbuttonAutoAddTrackPlaylist);
    gtk_container_add(GTK_CONTAINER(alignment5), checkbuttonAutoAddTrackPlaylist);

    checkbuttonIgnorePathInPlaylist = gtk_check_button_new_with_mnemonic(_("Ignore path information when importing playlist"));
    gtk_widget_show(checkbuttonIgnorePathInPlaylist);
    gtk_container_add(GTK_CONTAINER(alignment6), checkbuttonIgnorePathInPlaylist);

    labelDevice = gtk_label_new(_("<b>File Operations</b>"));
    gtk_widget_show(labelDevice);
    gtk_frame_set_label_widget(GTK_FRAME(frame3), labelDevice);
    gtk_label_set_use_markup(GTK_LABEL(labelDevice), TRUE);

    frame2 = gtk_frame_new(NULL);
    gtk_widget_show(frame2);
    gtk_box_pack_start(GTK_BOX(vbox1), frame2, TRUE, TRUE, 0);
    gtk_frame_set_shadow_type(GTK_FRAME(frame2), GTK_SHADOW_NONE);

    alignment2 = gtk_alignment_new(0.5, 0.5, 1, 1);
    gtk_widget_show(alignment2);
    gtk_container_add(GTK_CONTAINER(frame2), alignment2);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment2), 0, 0, 12, 0);
#if HAVE_GTK3 == 0
    vbox3 = gtk_vbox_new(FALSE, 0);
#else 
    vbox3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
#endif
    gtk_widget_show(vbox3);
    gtk_container_add(GTK_CONTAINER(alignment2), vbox3);

    checkbuttonDownloadPath = gtk_check_button_new_with_mnemonic(_("Always show Download Path?"));
    gtk_widget_show(checkbuttonDownloadPath);
    gtk_box_pack_start(GTK_BOX(vbox3), checkbuttonDownloadPath, FALSE, FALSE, 0);

#if HAVE_GTK3 == 0
    table1 = gtk_table_new(2, 3, FALSE);
    gtk_widget_show(table1);
    gtk_box_pack_start(GTK_BOX(vbox3), table1, TRUE, TRUE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(table1), 5);
    gtk_table_set_row_spacings(GTK_TABLE(table1), 5);
    gtk_table_set_col_spacings(GTK_TABLE(table1), 5);
#else 
    table1 = gtk_grid_new();
    gtk_widget_show(table1);
    gtk_box_pack_start(GTK_BOX(vbox3), table1, TRUE, TRUE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(table1), 5);
    gtk_grid_set_row_spacing(GTK_GRID(table1), 5);
    gtk_grid_set_column_spacing(GTK_GRID(table1), 5);
#endif    

    labelDownloadPath = gtk_label_new(_("Download Path:"));
    gtk_widget_show(labelDownloadPath);
#if HAVE_GTK3 == 0
    gtk_table_attach(GTK_TABLE(table1), labelDownloadPath, 0, 1, 0, 1,
            (GtkAttachOptions) (GTK_FILL),
            (GtkAttachOptions) (0), 0, 0);
#else 
    gtk_grid_attach(GTK_GRID(table1), labelDownloadPath, 0, 0, 1, 1);
#endif
    gtk_misc_set_alignment(GTK_MISC(labelDownloadPath), 0, 0.5);

    labelUploadPath = gtk_label_new(_("Upload Path:"));
    gtk_widget_show(labelUploadPath);
#if HAVE_GTK3 == 0
    gtk_table_attach(GTK_TABLE(table1), labelUploadPath, 0, 1, 1, 2,
            (GtkAttachOptions) (GTK_FILL),
            (GtkAttachOptions) (0), 0, 0);
#else
    gtk_grid_attach(GTK_GRID(table1), labelUploadPath, 0, 1, 1, 1);
#endif

    gtk_misc_set_alignment(GTK_MISC(labelUploadPath), 0, 0.5);

    entryDownloadPath = gtk_entry_new();
    gtk_widget_show(entryDownloadPath);
    gtk_editable_set_editable(GTK_EDITABLE(entryDownloadPath), FALSE);
#if HAVE_GTK3 == 0
    gtk_table_attach(GTK_TABLE(table1), entryDownloadPath, 1, 2, 0, 1,
            (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
            (GtkAttachOptions) (0), 0, 0);
#else
    gtk_grid_attach(GTK_GRID(table1), entryDownloadPath, 1, 0, 1, 1);
#endif

    entryUploadPath = gtk_entry_new();
    gtk_widget_show(entryUploadPath);
    gtk_editable_set_editable(GTK_EDITABLE(entryUploadPath), FALSE);
#if HAVE_GTK3 == 0
    gtk_table_attach(GTK_TABLE(table1), entryUploadPath, 1, 2, 1, 2,
            (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
            (GtkAttachOptions) (0), 0, 0);
#else
    gtk_grid_attach(GTK_GRID(table1), entryUploadPath, 1, 1, 1, 1);
#endif

    buttonDownloadPath = gtk_button_new_with_mnemonic(("..."));
    gtk_widget_show(buttonDownloadPath);
#if HAVE_GTK3 == 0
    gtk_table_attach(GTK_TABLE(table1), buttonDownloadPath, 2, 3, 0, 1,
            (GtkAttachOptions) (GTK_FILL),
            (GtkAttachOptions) (0), 0, 0);
#else
    gtk_grid_attach(GTK_GRID(table1), buttonDownloadPath, 2, 0, 1, 1);
#endif

    buttonUploadPath = gtk_button_new_with_mnemonic(("..."));
    gtk_widget_show(buttonUploadPath);
#if HAVE_GTK3 == 0
    gtk_table_attach(GTK_TABLE(table1), buttonUploadPath, 2, 3, 1, 2,
            (GtkAttachOptions) (GTK_FILL),
            (GtkAttachOptions) (0), 0, 0);
#else
    gtk_grid_attach(GTK_GRID(table1), buttonUploadPath, 2, 1, 1, 1);
#endif

    labelFilePath = gtk_label_new(_("<b>Filepaths on PC</b>"));
    gtk_widget_show(labelFilePath);
    gtk_frame_set_label_widget(GTK_FRAME(frame2), labelFilePath);
    gtk_label_set_use_markup(GTK_LABEL(labelFilePath), TRUE);

    // Now do the ask confirm delete...
#if HAVE_GTK3 == 0
    hbox1 = gtk_hbox_new(FALSE, 0);
#else
    hbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
#endif
    gtk_widget_show(hbox1);
    gtk_box_pack_start(GTK_BOX(vbox1), hbox1, FALSE, FALSE, 0);
#if HAVE_GTK3 == 0
    buttonClose = gtk_button_new_from_stock(GTK_STOCK_CLOSE);
#else
    buttonClose = gtk_button_new_with_mnemonic(_("_Close"));
#endif
    gtk_widget_show(buttonClose);
    gtk_box_pack_end(GTK_BOX(hbox1), buttonClose, FALSE, FALSE, 0);
    
    // And now set the fields.

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonDeviceConnect), Preferences.attemptDeviceConnectOnStart);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonDownloadPath), Preferences.ask_download_path);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonConfirmFileOp), Preferences.confirm_file_delete_op);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonConfirmOverWriteFileOp), Preferences.prompt_overwrite_file_op);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonAutoAddTrackPlaylist), Preferences.auto_add_track_to_playlist);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonIgnorePathInPlaylist), Preferences.ignore_path_in_playlist_import);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonSuppressAlbumErrors), Preferences.suppress_album_errors);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonAltAccessMethod), Preferences.use_alt_access_method);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttonAllMediaAsFiles), Preferences.allmediaasfiles);
    
    gtk_entry_set_text(GTK_ENTRY(entryDownloadPath), Preferences.fileSystemDownloadPath->str);
    gtk_entry_set_text(GTK_ENTRY(entryUploadPath), Preferences.fileSystemUploadPath->str);
    
    // Enable the callback functions.    

    g_signal_connect((gpointer) windowDialog, "destroy",
            G_CALLBACK(on_quitPrefs_activate),
            NULL);

    g_signal_connect((gpointer) buttonClose, "clicked",
            G_CALLBACK(on_quitPrefs_activate),
            NULL);
            
    g_signal_connect((gpointer) comboboxToolbarStyle, "changed",
            G_CALLBACK(on_PrefsToolbarStyle_activate),
            NULL);

    g_signal_connect((gpointer) checkbuttonDeviceConnect, "toggled",
            G_CALLBACK(on_PrefsDevice_activate),
            NULL);

    g_signal_connect((gpointer) checkbuttonConfirmFileOp, "toggled",
            G_CALLBACK(on_PrefsConfirmDelete_activate),
            NULL);

    g_signal_connect((gpointer) checkbuttonConfirmOverWriteFileOp, "toggled",
            G_CALLBACK(on_PrefsConfirmOverWriteFileOp_activate),
            NULL);

    g_signal_connect((gpointer) checkbuttonAutoAddTrackPlaylist, "toggled",
            G_CALLBACK(on_PrefsAutoAddTrackPlaylist_activate),
            NULL);

    g_signal_connect((gpointer) checkbuttonIgnorePathInPlaylist, "toggled",
            G_CALLBACK(on_PrefsIgnorePathInPlaylist_activate),
            NULL);

    g_signal_connect((gpointer) checkbuttonSuppressAlbumErrors, "toggled",
            G_CALLBACK(on_PrefsSuppressAlbumError_activate),
            NULL);

    g_signal_connect((gpointer) checkbuttonAltAccessMethod, "toggled",
            G_CALLBACK(on_PrefsUseAltAccessMethod_activate),
            NULL);
    
        g_signal_connect((gpointer) checkbuttonAllMediaAsFiles, "toggled",
            G_CALLBACK(on_PrefsAllMediaAsFiles_activate),
            NULL);

    g_signal_connect((gpointer) checkbuttonDownloadPath, "toggled",
            G_CALLBACK(on_PrefsAskDownload_activate),
            NULL);

    g_signal_connect((gpointer) buttonDownloadPath, "clicked",
            G_CALLBACK(on_PrefsDownloadPath_activate),
            NULL);

    g_signal_connect((gpointer) buttonUploadPath, "clicked",
            G_CALLBACK(on_PrefsUploadPath_activate),
            NULL);

    // To save the fields, we use callbacks on the widgets via gconf.

    return windowDialog;
}
