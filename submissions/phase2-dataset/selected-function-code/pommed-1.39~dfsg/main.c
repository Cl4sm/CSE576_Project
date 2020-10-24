int
main(int argc, char **argv)
{
  int c;
  int ret;

  GIOChannel *ch;
  int fd;

  gtk_init(&argc, &argv);

  bindtextdomain("gpomme", "/usr/share/locale");
  textdomain("gpomme");

  ret = config_load();
  if (ret < 0)
    {
      fprintf(stderr, "Failed to load configuration\n");

      exit(1);
    }

  while ((c = getopt(argc, argv, "cv")) != -1)
    {
      switch (c)
	{
	  case 'c':
	    config_gui();

	    exit(0);
	    break;

	  case 'v':
	    printf("gpomme v" M_VERSION " graphical client for pommed\n");
	    printf("Copyright (C) 2006-2008 Julien BLACHE <jb@jblache.org> and others\n");

	    exit(0);
	    break;

	  default:
	    usage();

	    exit(-1);
	    break;
	}
    }

  mbp_dbus_connect();

  fd = config_monitor();

  if (fd > 0)
    {
      ch = g_io_channel_unix_new(fd);
      g_io_channel_set_encoding(ch, NULL, NULL);
      g_io_add_watch(ch, G_IO_IN | G_IO_HUP | G_IO_ERR | G_IO_NVAL, mbp_check_config, NULL);
    }

  signal(SIGINT, sig_int_term_handler);
  signal(SIGTERM, sig_int_term_handler);
  signal(SIGCHLD, sig_chld_handler);

  create_window();

  gtk_main();

  mbp_dbus_cleanup();

  return 0;
}