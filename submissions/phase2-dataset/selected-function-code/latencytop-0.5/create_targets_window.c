{
	GtkWidget *w, *paned, *paned2, *vbox, *hbox, *btn;

	/* We need a window first for the process list */
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(main_window), "LatencyTOP "VERSION);
	g_signal_connect(G_OBJECT(main_window), "destroy",
			 G_CALLBACK(gtk_main_quit), NULL);
	gtk_container_set_border_width(GTK_CONTAINER(main_window), 5);
	gtk_widget_set_size_request(main_window, 600, 400);

	/* Split top container vs. buttons etc... at the bottom */
	vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(main_window), vbox);

	/* Split it in two with an hpaned */
	paned = gtk_hpaned_new();
	gtk_box_pack_start(GTK_BOX(vbox), paned, TRUE, TRUE, 0);

	/* Create the target list (left pane) */
	w = create_targets_list();
	gtk_paned_add1(GTK_PANED(paned), w);

	/* Split the right pane between results and backtrace */
	paned2 = gtk_vpaned_new();
	gtk_paned_add2(GTK_PANED(paned), paned2);
	
	/* Create the results list (right/top pane) */
	w = create_results_list();
	gtk_paned_add1(GTK_PANED(paned2), w);

	/* Create the backtrace list (right/bottom pane) */
	w = create_backtrace_list();
	gtk_paned_add2(GTK_PANED(paned2), w);

	/* The panner seems to default to a stupid position, I'm not sure
	 * yet what's the right way of getting it to automagically adjust
	 * to the size of the list items, so let's whack it for now to
	 * some arbitrary value that looks better
	 */
	gtk_paned_set_position(GTK_PANED(paned), 200);
	gtk_paned_set_position(GTK_PANED(paned2), 250);

	/* Now create an hbox at the botton for various controls, etc... */
	hbox = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_end(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	/* Add the countdown */
	countdown_label = gtk_label_new("");
	gtk_box_pack_end(GTK_BOX(hbox), countdown_label, FALSE, FALSE, 0);
	btn = gtk_button_new_with_label("Refresh");
	gtk_box_pack_end(GTK_BOX(hbox), btn, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(btn), "clicked",
			 G_CALLBACK(refresh_clicked), NULL);
	freeze_button = gtk_toggle_button_new_with_label("Freeze");
	gtk_box_pack_end(GTK_BOX(hbox), freeze_button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(freeze_button), "toggled",
			 G_CALLBACK(freeze_toggled), NULL);
}
