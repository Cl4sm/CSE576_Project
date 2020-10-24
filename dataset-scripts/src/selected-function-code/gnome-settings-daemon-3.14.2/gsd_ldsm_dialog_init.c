static void
gsd_ldsm_dialog_init (GsdLdsmDialog *dialog)
{
        GtkWidget *main_vbox, *text_vbox, *hbox;
        GtkWidget *image;
	
        dialog->priv = GSD_LDSM_DIALOG_GET_PRIVATE (dialog);
        
        main_vbox = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

        /* Set up all the window stuff here */
        gtk_window_set_title (GTK_WINDOW (dialog), _("Low Disk Space"));
        gtk_window_set_icon_name (GTK_WINDOW (dialog), 
                                  "dialog-warning");
        gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);
        gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
        gtk_window_set_urgency_hint (GTK_WINDOW (dialog), TRUE);
        gtk_window_set_focus_on_map (GTK_WINDOW (dialog), FALSE);
        gtk_container_set_border_width (GTK_CONTAINER (dialog), 5);

        /* Create the image */
        image = gtk_image_new_from_icon_name ("dialog-warning", GTK_ICON_SIZE_DIALOG);
        gtk_misc_set_alignment (GTK_MISC (image), 0.5, 0.0);

        /* Create the labels */
        dialog->priv->primary_label = gtk_label_new (NULL);	
        gtk_label_set_line_wrap (GTK_LABEL (dialog->priv->primary_label), TRUE);
        gtk_label_set_single_line_mode (GTK_LABEL (dialog->priv->primary_label), FALSE);
        gtk_misc_set_alignment (GTK_MISC (dialog->priv->primary_label), 0.0, 0.0);
	
        dialog->priv->secondary_label = gtk_label_new (NULL);
        gtk_label_set_line_wrap (GTK_LABEL (dialog->priv->secondary_label), TRUE);
        gtk_label_set_single_line_mode (GTK_LABEL (dialog->priv->secondary_label), FALSE);
        gtk_misc_set_alignment (GTK_MISC (dialog->priv->secondary_label), 0.0, 0.0);

        /* Create the check button to ignore future warnings */
        dialog->priv->ignore_check_button = gtk_check_button_new ();
        /* The button should be inactive if the dialog was just called.
         * I suppose it could be possible for the user to manually edit the GSettings key between
         * the mount being checked and the dialog appearing, but I don't think it matters
         * too much */
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (dialog->priv->ignore_check_button), FALSE);
        g_signal_connect (dialog->priv->ignore_check_button, "toggled",
                          G_CALLBACK (ignore_check_button_toggled_cb), dialog);
        
        /* Now set up the dialog's GtkBox's' */
        gtk_box_set_spacing (GTK_BOX (main_vbox), 14);
	
        hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
        gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
	
        text_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
        
        gtk_box_pack_start (GTK_BOX (text_vbox), dialog->priv->primary_label, FALSE, FALSE, 0);
        gtk_box_pack_start (GTK_BOX (text_vbox), dialog->priv->secondary_label, TRUE, TRUE, 0);
        gtk_box_pack_start (GTK_BOX (text_vbox), dialog->priv->ignore_check_button, FALSE, FALSE, 0);
        gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, FALSE, 0);
        gtk_box_pack_start (GTK_BOX (hbox), text_vbox, TRUE, TRUE, 0);	
        gtk_box_pack_start (GTK_BOX (main_vbox), hbox, FALSE, FALSE, 0);
						
        /* Set up the action area */
        gtk_box_set_spacing (GTK_BOX (gtk_dialog_get_action_area (GTK_DIALOG (dialog))), 6);
        gtk_container_set_border_width (GTK_CONTAINER (gtk_dialog_get_action_area (GTK_DIALOG (dialog))), 5);
	
        gtk_widget_show_all (hbox);
}