static gboolean
mbp_check_config(GIOChannel *ch, GIOCondition condition, gpointer userdata)
{
  int fd;
  int ret;

  fd = g_io_channel_unix_get_fd(ch);

  if (condition & G_IO_IN)
    {
      ret = config_load();
      if (ret < 0)
	{
	  fprintf(stderr, "Failed to reload config file, exiting\n");

	  gtk_main_quit();
	}
    }

  close(fd);

  fd = config_monitor();

  if (fd > 0)
    {
      ch = g_io_channel_unix_new(fd);
      g_io_channel_set_encoding(ch, NULL, NULL);
      g_io_add_watch(ch, G_IO_IN | G_IO_HUP | G_IO_ERR | G_IO_NVAL, mbp_check_config, NULL);
    }

  return FALSE;
}