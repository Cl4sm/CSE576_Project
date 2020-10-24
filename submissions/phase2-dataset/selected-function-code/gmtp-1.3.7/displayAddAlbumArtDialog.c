void displayAddAlbumArtDialog(void) {
    //Album_Struct* albumdetails;
    GtkWidget *hbox, *label;
    GtkWidget *buttonBox;
    LIBMTP_album_t *albuminfo = NULL;
    LIBMTP_album_t *album_orig = NULL;

    AlbumArtDialog = gtk_dialog_new_with_buttons(_("Album Art"), GTK_WINDOW(windowMain),
            (GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
            _("_Close"), GTK_RESPONSE_CLOSE,
            NULL);

    gtk_dialog_set_default_response(GTK_DIALOG(AlbumArtDialog), GTK_RESPONSE_CLOSE);
    gtk_window_set_resizable(GTK_WINDOW(AlbumArtDialog), FALSE);
#if HAVE_GTK3 == 0
    gtk_dialog_set_has_separator(GTK_DIALOG(AlbumArtDialog), TRUE);
#endif

    // Set some nice 5px spacing.
#if HAVE_GTK3 == 0
    gtk_box_set_spacing(GTK_BOX(GTK_DIALOG(AlbumArtDialog)->vbox), 10);
#else
    gtk_box_set_spacing(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(AlbumArtDialog))), 10);
#endif
#if HAVE_GTK3 == 0
    hbox = gtk_hbox_new(FALSE, 5);
#else
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
#endif
    gtk_widget_show(hbox);

#if HAVE_GTK3 == 0
    gtk_container_add(GTK_CONTAINER(GTK_DIALOG(AlbumArtDialog)->vbox), hbox);
#else
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(AlbumArtDialog))), hbox);
#endif

    label = gtk_label_new(_("Album:"));
    gtk_widget_show(label);
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
    gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);

    // Now create the combo box.
#if HAVE_GTK3 == 0
    textboxAlbumArt = gtk_combo_box_new_text();
#else
    textboxAlbumArt = gtk_combo_box_text_new();
#endif
    gtk_widget_show(textboxAlbumArt);
    gtk_box_pack_start(GTK_BOX(hbox), textboxAlbumArt, TRUE, TRUE, 0);
    // Now add in our selection strings.
    albuminfo = LIBMTP_Get_Album_List_For_Storage(DeviceMgr.device, DeviceMgr.devicestorage->id);
    // Better check to see if we actually have anything?
    if (albuminfo == NULL) {
        // we have no albums.
        displayInformation(_("No Albums available to set Album Art with. Either:\n1. You have no music files uploaded?\n2. Your device does not support Albums?\n3. Previous applications used to upload files do not autocreate albums for you or support the metadata for those files in order to create the albums for you?\n"));
        gtk_widget_destroy(AlbumArtDialog);
        AlbumArtImage = NULL;
        AlbumArtDialog = NULL;
        textboxAlbumArt = NULL;
        return;
    }

    album_orig = albuminfo;
    while (albuminfo != NULL) {
        if (albuminfo->name != NULL)
#if HAVE_GTK3 == 0
            gtk_combo_box_append_text(GTK_COMBO_BOX(textboxAlbumArt), albuminfo->name);
#else
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(textboxAlbumArt), albuminfo->name);
#endif
        albuminfo = albuminfo->next;
    }
    // End add selection.
    gtk_combo_box_set_active(GTK_COMBO_BOX(textboxAlbumArt), 0);

    // Add in a image view of the current uploaded album art.
#if HAVE_GTK3 == 0
    AlbumArtImage = gtk_image_new_from_stock(GTK_STOCK_MISSING_IMAGE, GTK_ICON_SIZE_DIALOG);
#else
    AlbumArtImage = gtk_image_new_from_icon_name("image-missing", GTK_ICON_SIZE_DIALOG);
#endif

#if HAVE_GTK3 == 0
    gtk_container_add(GTK_CONTAINER(GTK_DIALOG(AlbumArtDialog)->vbox), AlbumArtImage);
#else
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(AlbumArtDialog))), AlbumArtImage);
#endif
    gtk_widget_show(AlbumArtImage);


    // Add in the album art operations area.
#if HAVE_GTK3 == 0
    buttonBox = gtk_hbutton_box_new();
#else
    buttonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
#endif
    gtk_box_set_spacing(GTK_BOX(buttonBox), 5);
    gtk_widget_show(buttonBox);

#if HAVE_GTK3 == 0
    gtk_container_add(GTK_CONTAINER(GTK_DIALOG(AlbumArtDialog)->vbox), buttonBox);
#else
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(AlbumArtDialog))), buttonBox);
#endif

    buttonAlbumAdd = gtk_button_new_with_mnemonic(_("Upload"));
    gtk_widget_show(buttonAlbumAdd);
    gtk_box_pack_start(GTK_BOX(buttonBox), buttonAlbumAdd, TRUE, TRUE, 0);

    buttonAlbumDelete = gtk_button_new_with_mnemonic(_("Remove"));
    gtk_widget_show(buttonAlbumDelete);
    gtk_box_pack_start(GTK_BOX(buttonBox), buttonAlbumDelete, TRUE, TRUE, 0);
    gtk_widget_set_sensitive(GTK_WIDGET(buttonAlbumDelete), FALSE);

    buttonAlbumDownload = gtk_button_new_with_mnemonic(_("Download"));
    gtk_widget_show(buttonAlbumDownload);
    gtk_box_pack_start(GTK_BOX(buttonBox), buttonAlbumDownload, TRUE, TRUE, 0);
    gtk_widget_set_sensitive(GTK_WIDGET(buttonAlbumDownload), FALSE);

    // Now, update the stock image with one from the selected album.
    AlbumArtUpdateImage(album_orig);

    g_signal_connect((gpointer) textboxAlbumArt, "changed",
            G_CALLBACK(on_albumtextbox_activate),
            NULL);

    g_signal_connect((gpointer) buttonAlbumAdd, "clicked",
            G_CALLBACK(on_buttonAlbumArtAdd_activate),
            NULL);

    g_signal_connect((gpointer) buttonAlbumDelete, "clicked",
            G_CALLBACK(on_buttonAlbumArtDelete_activate),
            NULL);

    g_signal_connect((gpointer) buttonAlbumDownload, "clicked",
            G_CALLBACK(on_buttonAlbumArtDownload_activate),
            NULL);

    gtk_dialog_run(GTK_DIALOG(AlbumArtDialog));

    gtk_widget_destroy(AlbumArtDialog);
    clearAlbumStruc(album_orig);

    //Clean up global pointers.
    AlbumArtImage = NULL;
    AlbumArtDialog = NULL;
    textboxAlbumArt = NULL;
}
