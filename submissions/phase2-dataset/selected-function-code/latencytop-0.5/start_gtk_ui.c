void start_gtk_ui(void) 
{
	load_icons();
	create_targets_window();

	/* Initialize the lists */
	update_list();
	insert_all_targets();

	/* Make them look good */
	gtk_tree_view_columns_autosize(GTK_TREE_VIEW(targets_view));
	gtk_tree_view_columns_autosize(GTK_TREE_VIEW(results_view));

	/* Show everything */
	gtk_widget_show_all(main_window);

	/* Start the timer */
	countdown = countdown_max;
	update_countdown();
	g_timeout_add(1000, timer_tick, NULL);

	/* Run the main event loop */
	gtk_main();
}
