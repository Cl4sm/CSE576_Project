    GtkWidget *dialog, *hbox, *label, *textbox;
    LIBMTP_devicestorage_t *devicestorage;
    gchar *tmp_string = NULL;
    gint dialog_selection = 0;

    devicestorage = DeviceMgr.device->storage;

    dialog = gtk_dialog_new_with_buttons(_("Connect to which storage device?"), GTK_WINDOW(windowMain),
            (GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
            _("_OK"), GTK_RESPONSE_OK,
            NULL);

    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
#if HAVE_GTK3 == 0
    hbox = gtk_hbox_new(FALSE, 5);
#else
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
#endif
    gtk_widget_show(hbox);
    //gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), hbox);
#if HAVE_GTK3 == 0
    gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), hbox);
#else
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), hbox);
#endif

    label = gtk_label_new(_("Storage Device:"));
    gtk_widget_show(label);
    gtk_container_add(GTK_CONTAINER(hbox), label);

    // Now create the combo box.
#if HAVE_GTK3 == 0
    textbox = gtk_combo_box_new_text();
#else
    textbox = gtk_combo_box_text_new();
#endif
    gtk_widget_show(textbox);
    gtk_container_add(GTK_CONTAINER(hbox), textbox);
    // Now add in our selection strings.
    while (devicestorage != NULL) {
        if (devicestorage->StorageDescription != NULL) {
#if HAVE_GTK3 == 0
            gtk_combo_box_append_text(GTK_COMBO_BOX(textbox), devicestorage->StorageDescription);
#else
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(textbox), devicestorage->StorageDescription);
#endif
        } else {
            tmp_string = g_strdup_printf(_("Unknown id: %d, %s"), devicestorage->id, calculateFriendlySize((devicestorage->MaxCapacity)));
#if HAVE_GTK3 == 0
            gtk_combo_box_append_text(GTK_COMBO_BOX(textbox), tmp_string);
#else
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(textbox), tmp_string);
#endif
        }
        devicestorage = devicestorage->next;
    }

    gtk_combo_box_set_active(GTK_COMBO_BOX(textbox), 0);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        dialog_selection = gtk_combo_box_get_active(GTK_COMBO_BOX(textbox));
    }
    gtk_widget_destroy(dialog);
    g_free(tmp_string);
    return dialog_selection;

}
