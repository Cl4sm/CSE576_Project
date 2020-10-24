void
on_ok_button3_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
	FILE *file_p;
	gchar *fname;

	fname = g_strdup(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(lookup_widget(gtk_widget_get_toplevel(GTK_WIDGET(button)),"fileselection3"))));

	if (strlen(fname) == 0)
		return;	

	if (strlen(fname) > 99) {
                error("Error: database file name too long (>100 chars)");
		return;	
	}

	strncpy(address_filename, fname, strlen(fname));

	if((file_p = fopen(fname, "a")) == NULL) { 
                //printf("can not open or create database file\n");
                error("Error: can't open or create database file");
        }
        else
		fclose(file_p);

	gtk_grab_remove(gtk_widget_get_toplevel(GTK_WIDGET(button)));
	gtk_widget_destroy(gtk_widget_get_toplevel(GTK_WIDGET(button)));
}