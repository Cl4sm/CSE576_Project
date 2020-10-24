static void do_about(gpointer object, gpointer user_data)
{
	GtkWidget *dialog = gtk_about_dialog_new();
	
	const gchar *artists[] = {"Logo: PMOX on commons.wikimedia.org", 
				  "Thumb up: Pratheepps on commons.wikimedia.org",
				  NULL};
	const gchar *authors[] = {"Colin Leroy <colin@colino.net>", NULL};
	
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "QuitCount");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), VERSION);
	if (icon != NULL)
		gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), gtk_status_icon_get_pixbuf(icon));
	gtk_about_dialog_set_artists(GTK_ABOUT_DIALOG(dialog), (const gchar **)artists);
	gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(dialog), (const gchar **)authors);
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), 
		_("Copyright (C) 2009-2013, Colin Leroy <colin@colino.net>\nGPL version 3 or later"));
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), 
		"http://quitcount.sf.net/");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}