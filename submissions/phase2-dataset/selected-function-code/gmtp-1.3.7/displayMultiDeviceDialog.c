    GtkWidget *dialog, *hbox, *label, *textbox;
    gchar *tmp_string = NULL;
    gint dialog_selection = 0;

    dialog = gtk_dialog_new_with_buttons(_("Connect to which device?"), GTK_WINDOW(windowMain),
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
#if HAVE_GTK3 == 0
    gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), hbox);
#else
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), hbox);
#endif

    label = gtk_label_new(_("Device:"));
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
    // We should just take straight strings here, but this is quicker/easier.
    gint i = 0;
    for (i = 0; i < DeviceMgr.numrawdevices; i++) {
        if (DeviceMgr.rawdevices[i].device_entry.vendor != NULL ||
                DeviceMgr.rawdevices[i].device_entry.product != NULL) {
            tmp_string = g_strdup_printf("   %s %s : (%04x:%04x) @ bus %d, dev %d",
                    DeviceMgr.rawdevices[i].device_entry.vendor,
                    DeviceMgr.rawdevices[i].device_entry.product,
                    DeviceMgr.rawdevices[i].device_entry.vendor_id,
                    DeviceMgr.rawdevices[i].device_entry.product_id,
                    DeviceMgr.rawdevices[i].bus_location,
                    DeviceMgr.rawdevices[i].devnum);
        } else {
            tmp_string = g_strdup_printf(_("Unknown : %04x:%04x @ bus %d, dev %d"),
                    DeviceMgr.rawdevices[i].device_entry.vendor_id,
                    DeviceMgr.rawdevices[i].device_entry.product_id,
                    DeviceMgr.rawdevices[i].bus_location,
                    DeviceMgr.rawdevices[i].devnum);
        }
#if HAVE_GTK3 == 0
        gtk_combo_box_append_text(GTK_COMBO_BOX(textbox), tmp_string);
#else
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(textbox), tmp_string);
#endif
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
