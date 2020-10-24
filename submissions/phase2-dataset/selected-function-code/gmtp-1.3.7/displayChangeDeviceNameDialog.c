gchar* displayChangeDeviceNameDialog(gchar* devicename) {
    GtkWidget *dialog, *hbox, *label, *textbox;
    gchar* textfield;

    dialog = gtk_dialog_new_with_buttons(_("Change Device Name"), GTK_WINDOW(windowMain),
            (GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
            _("_OK"), GTK_RESPONSE_OK,
            _("_Cancel"), GTK_RESPONSE_CANCEL,
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

    label = gtk_label_new(_("Device Name:"));
    gtk_widget_show(label);
    gtk_container_add(GTK_CONTAINER(hbox), label);

    textbox = gtk_entry_new();
    gtk_widget_show(textbox);
    if (devicename != NULL) {
        gtk_entry_set_text(GTK_ENTRY(textbox), devicename);
    }
    gtk_entry_set_max_length(GTK_ENTRY(textbox), 64);
    gtk_entry_set_has_frame(GTK_ENTRY(textbox), TRUE);
    gtk_entry_set_activates_default(GTK_ENTRY(textbox), TRUE);
    gtk_container_add(GTK_CONTAINER(hbox), textbox);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        textfield = g_strdup(gtk_entry_get_text(GTK_ENTRY(textbox)));
        if (strlen(textfield) == 0) {
            // We have an emtpy string.
            gtk_widget_destroy(dialog);
            return NULL;
        } else {
            gtk_widget_destroy(dialog);
            return textfield;
        }
    } else {
        gtk_widget_destroy(dialog);
        return NULL;
    }
}
