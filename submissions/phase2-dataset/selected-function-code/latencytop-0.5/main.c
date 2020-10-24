int main(int argc, char **argv)
{
	int i, use_gtk = 0;

	enable_sysctl();
	enable_fsync_tracer();
	atexit(cleanup_sysctl);

#ifdef HAS_GTK_GUI
	if (preinitialize_gtk_ui(&argc, &argv))
		use_gtk = 1;
#endif
	if (!use_gtk)
		preinitialize_text_ui(&argc, &argv);

	for (i = 1; i < argc; i++)		
		if (strcmp(argv[i],"-d") == 0) {
			init_translations("latencytop.trans");
			parse_global_list();
			sort_list();
			dump_global_to_console();
			return EXIT_SUCCESS;
		}
	for (i = 1; i < argc; i++)
		if (strcmp(argv[i], "--unknown") == 0) {
			noui = 1;
			dump_unknown = 1;
		}

	/* Allow you to specify a process name to track */
	for (i = 1; i < argc; i++)
		if (argv[i][0] != '-') {
			prefered_process = strdup(argv[i]);
			break;
		}

	init_translations("/usr/share/latencytop/latencytop.trans");
	if (!translations)
		init_translations("latencytop.trans"); /* for those who don't do make install */
	
	while (noui) {
		sleep(5);
		fprintf(stderr, ".");
	}
#ifdef HAS_GTK_GUI
	if (use_gtk)
		start_gtk_ui();
	else
#endif
		start_text_ui();

	prune_unused_procs();
	delete_list();

	return EXIT_SUCCESS;
}
