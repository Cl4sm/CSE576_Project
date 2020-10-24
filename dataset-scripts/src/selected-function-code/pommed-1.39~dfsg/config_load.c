int
config_load(void)
{
  struct passwd *pw;

  int ret;

  if (conffile == NULL)
    {
      pw = getpwuid(getuid());
      if (pw == NULL)
	{
	  fprintf(stderr, "Could not get user information\n");

	  return -1;
	}

      conffile = (char *) malloc(strlen(pw->pw_dir) + strlen(CONFFILE) + 1);
      if (conffile == NULL)
	{
	  fprintf(stderr, "Could not allocate memory\n");

	  return -1;
	}

      strncpy(conffile, pw->pw_dir, strlen(pw->pw_dir) + 1);
      strncat(conffile, CONFFILE, strlen(CONFFILE));
    }

  if (cfg != NULL)
    cfg_free(cfg);

  cfg = cfg_init(cfg_opts, CFGF_NONE);

  if (cfg == NULL)
    {
      fprintf(stderr, "Failed to initialize configuration parser\n");

      return -1;
    }

  /* Set up config values validation */
  cfg_set_validate_func(cfg, "theme", config_validate_string);
  cfg_set_validate_func(cfg, "timeout", config_validate_positive_integer);

  /* 
   * Do the actual parsing.
   * If the file does not exist or cannot be opened,
   * we'll be using the default values defined in the cfg_opt_t array.
   */
  ret = cfg_parse(cfg, conffile);
  if (ret != CFG_SUCCESS)
    {
      if (ret == CFG_FILE_ERROR)
	{
	  config_write();
	}
      else
	{
	  cfg_free(cfg);

	  fprintf(stderr, "Failed to parse configuration file\n");

	  return -1;
	}
    }

  /* Fill up the structs */
  mbp_w.timeout = cfg_getint(cfg, "timeout");

  ret = theme_load(cfg_getstr(cfg, "theme"));
  if (ret < 0)
    {
      fprintf(stderr, "Failed to load theme '%s', using '%s' instead\n",
	      cfg_getstr(cfg, "theme"), DEFAULT_THEME);

      ret = theme_load(DEFAULT_THEME);
      if (ret < 0)
	{
	  fprintf(stderr, "Failed to load default theme '%s'\n", DEFAULT_THEME);

	  return -1;
	}
    }

  return 0;
}