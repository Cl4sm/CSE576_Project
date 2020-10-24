gint displayFileOverwriteDialog(gchar *filename) {
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new_with_markup(GTK_WINDOW(windowMain),
            (GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_NONE,
            _("File <b>%s</b> already exists in target folder.\nDo you want to:"), filename);
    gtk_dialog_add_buttons(GTK_DIALOG(dialog),
            _("Skip"), MTP_SKIP,
            _("Skip All"), MTP_SKIP_ALL,
            _("Overwrite"), MTP_OVERWRITE,
            _("Overwrite All"), MTP_OVERWRITE_ALL,
            NULL);
    gtk_window_set_title(GTK_WINDOW(dialog), _("Question: Confirm Overwrite of Existing File?"));
    gtk_dialog_set_default_response(GTK_DIALOG(dialog), MTP_OVERWRITE);
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return result;
}
