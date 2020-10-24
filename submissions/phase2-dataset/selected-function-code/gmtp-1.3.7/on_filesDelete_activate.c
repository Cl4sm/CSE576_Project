void on_filesDelete_activate(GtkMenuItem *menuitem, gpointer user_data) {
    GtkWidget *dialog;

    // Let's check to see if we have anything selected in our treeview?
    if (fileListGetSelection() == NULL) {
        if (folderListGetSelection() != -1) {
            on_folderRemoveFolder_activate(menuitem, user_data);
        } else {
            displayInformation(_("No files/folders selected?"));
        }
        return;
    }

    // Now we prompt to confirm delete?
    if (Preferences.confirm_file_delete_op == FALSE) {
        // Now download the actual file from the MTP device.
        fileListRemove(fileListGetSelection());
    } else {
        dialog = gtk_message_dialog_new(GTK_WINDOW(windowMain),
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_WARNING,
                GTK_BUTTONS_YES_NO,
                _("Are you sure you want to delete these files?"));
        gtk_window_set_title(GTK_WINDOW(dialog), _("Confirm Delete"));

        // Run the Dialog and get our result.
        gint result = gtk_dialog_run(GTK_DIALOG(dialog));
        if (result == GTK_RESPONSE_YES)
            fileListRemove(fileListGetSelection());

        // Destroy the dialog box.
        gtk_widget_destroy(dialog);
    }
} // on_filesDelete_activate()
