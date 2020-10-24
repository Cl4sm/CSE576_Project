void run_quitcount(GtkApplication *app)
{
	GList *windows = gtk_application_get_windows(app);
	if (windows == NULL) {
		GtkWidget *dummy = gtk_window_new(GTK_WINDOW_TOPLEVEL);

		gtk_window_set_application(GTK_WINDOW(dummy), app);

		set_icon(FALSE);

		if (!load_config()) {
			currency = g_strdup("€");
			show_config(NULL, NULL);
		}
		install_anniversary_watch();
	
		g_signal_connect(G_OBJECT(icon), "popup-menu", 
				 G_CALLBACK(do_popup_menu), app);
		g_signal_connect(G_OBJECT(icon), "activate", 
				 G_CALLBACK(do_activate), app);
	} else if (stop_date != (time_t)0) {
		do_activate(NULL, NULL);
	}
}