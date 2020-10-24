GsdLdsmDialog*
gsd_ldsm_dialog_new (gboolean     other_usable_partitions,
                     gboolean     other_partitions,
                     gboolean     display_baobab,
                     gboolean     display_empty_trash,
                     gint64       space_remaining,
                     const gchar *partition_name,
                     const gchar *mount_path)
{
        GsdLdsmDialog *dialog;
        GtkWidget *button_empty_trash, *button_ignore, *button_analyze;
        GtkWidget *empty_trash_image, *analyze_image;
        gchar *primary_text, *primary_text_markup;
        const gchar *secondary_text, *checkbutton_text;
	
        dialog = GSD_LDSM_DIALOG (g_object_new (GSD_TYPE_LDSM_DIALOG,
                                                "other-usable-partitions", other_usable_partitions,
                                                "other-partitions", other_partitions,
                                                "has-trash", display_empty_trash,
                                                "space-remaining", space_remaining,
                                                "partition-name", partition_name,
                                                "mount-path", mount_path,
                                                NULL));
	
        /* Add some buttons */
        if (dialog->priv->has_trash) {
                button_empty_trash = gtk_dialog_add_button (GTK_DIALOG (dialog),
                                                            _("Empty Trash"),
                                                            GSD_LDSM_DIALOG_RESPONSE_EMPTY_TRASH);
                empty_trash_image = gtk_image_new_from_icon_name ("edit-clear", GTK_ICON_SIZE_BUTTON);
                gtk_button_set_image (GTK_BUTTON (button_empty_trash), empty_trash_image);
        }
	
        if (display_baobab) {
                button_analyze = gtk_dialog_add_button (GTK_DIALOG (dialog),
                                                        _("Examine…"),
                                                        GSD_LDSM_DIALOG_RESPONSE_ANALYZE);
                analyze_image = gtk_image_new_from_icon_name ("baobab", GTK_ICON_SIZE_BUTTON);
                gtk_button_set_image (GTK_BUTTON (button_analyze), analyze_image);
        }
			
        button_ignore = gtk_dialog_add_button (GTK_DIALOG (dialog), 
                                               _("Ignore"), 
                                               GTK_RESPONSE_CANCEL);

        gtk_widget_grab_default (button_ignore);
	
        /* Set the label text */	
        primary_text = gsd_ldsm_dialog_get_primary_text (dialog);
        primary_text_markup = g_markup_printf_escaped ("<big><b>%s</b></big>", primary_text);
        gtk_label_set_markup (GTK_LABEL (dialog->priv->primary_label), primary_text_markup);

        secondary_text = gsd_ldsm_dialog_get_secondary_text (dialog);
        gtk_label_set_text (GTK_LABEL (dialog->priv->secondary_label), secondary_text);
        
        checkbutton_text = gsd_ldsm_dialog_get_checkbutton_text (dialog);
        gtk_button_set_label (GTK_BUTTON (dialog->priv->ignore_check_button), checkbutton_text);
        
        g_free (primary_text);
        g_free (primary_text_markup);
	
        return dialog;
} 