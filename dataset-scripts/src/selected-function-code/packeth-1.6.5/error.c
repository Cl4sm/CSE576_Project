void error(gchar *error_type)
{
	GtkWidget *label;
	
	if ( show_error_dialog == 0)
		return;

      	if (error_dialog_menu != NULL) {
    		label = lookup_widget(GTK_WIDGET(error_dialog_menu), "label165");
		gtk_label_set_text(GTK_LABEL(label), error_type);
                gdk_window_show(error_dialog_menu->window);
                gdk_window_raise(error_dialog_menu->window);
                return;
        }
	else {
   	   	error_dialog_menu = create_error_dialog();
        	label = lookup_widget(GTK_WIDGET(error_dialog_menu), "label165");
		gtk_label_set_text(GTK_LABEL(label), error_type);
        	gtk_widget_show(error_dialog_menu);
	}
}