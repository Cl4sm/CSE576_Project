main(int argc, char *argv[])
{
  /* Check to see if the user is running us as root, which is a nono */
  if (geteuid() == 0)
  {
    fprintf(stderr, "ERROR: This server won't run as root/superuser\n");
    return -1;
  }

  /* Setup corefile size immediately after boot -kre */
  setup_corefile();

  /* save server boot time right away, so getrusage works correctly */
  set_time();

  /* It ain't random, but it ought to be a little harder to guess */
  init_genrand(SystemTime.tv_sec ^ (SystemTime.tv_usec | (getpid() << 20)));

  me.localClient = &meLocalUser;
  dlinkAdd(&me, &me.node, &global_client_list);  /* Pointer to beginning
						   of Client list */
  ConfigLog.use_logging = 1;
  ConfigGeneral.dpath      = DPATH;
  ConfigGeneral.spath      = SPATH;
  ConfigGeneral.mpath      = MPATH;
  ConfigGeneral.configfile = CPATH;    /* Server configuration file */
  ConfigGeneral.klinefile  = KPATH;    /* Server kline file         */
  ConfigGeneral.glinefile  = GPATH;    /* Server gline file         */
  ConfigGeneral.xlinefile  = XPATH;    /* Server xline file         */
  ConfigGeneral.dlinefile  = DLPATH;   /* dline file                */
  ConfigGeneral.resvfile   = RESVPATH; /* resv file                 */

  myargv = argv;
  umask(077);                /* better safe than sorry --SRB */

  parseargs(&argc, &argv, myopts);

  if (printVersion)
  {
    printf("ircd: version %s(%s)\n", ircd_version, serno);
    exit(EXIT_SUCCESS);
  }

  if (chdir(ConfigGeneral.dpath))
  {
    perror("chdir");
    exit(EXIT_FAILURE);
  }

  ssl_init();

  if (!server_state.foreground)
  {
    make_daemon();
    close_standard_fds(); /* this needs to be before init_netio()! */
  }
  else
    print_startup(getpid());

  setup_signals();

  /* We need this to initialise the fd array before anything else */
  fdlist_init();
  log_set_file(LOG_TYPE_IRCD, 0, logFileName);

  init_netio();         /* This needs to be setup early ! -- adrian */

  /* Check if there is pidfile and daemon already running */
  check_pidfile(pidFileName);

  mp_pool_init();
  init_dlink_nodes();
  init_isupport();
  dbuf_init();
  hash_init();
  ipcache_init();
  init_host_hash();          /* Host-hashtable. */
  client_init();
  class_init();
  whowas_init();
  watch_init();
  auth_init();          /* Initialise the auth code */
  init_resolver();      /* Needs to be setup before the io loop */
  modules_init();
  read_conf_files(1);   /* cold start init conf files */
  init_uid();
  initialize_server_capabs();   /* Set up default_server_capabs */
  initialize_global_set_options();
  channel_init();
  read_links_file();
  motd_init();
#ifdef HAVE_LIBGEOIP
  geoip_ctx = GeoIP_new(GEOIP_MEMORY_CACHE);
#endif

  if (EmptyString(ConfigServerInfo.sid))
  {
    ilog(LOG_TYPE_IRCD, "ERROR: No server id specified in serverinfo block.");
    exit(EXIT_FAILURE);
  }

  strlcpy(me.id, ConfigServerInfo.sid, sizeof(me.id));

  if (EmptyString(ConfigServerInfo.name))
  {
    ilog(LOG_TYPE_IRCD, "ERROR: No server name specified in serverinfo block.");
    exit(EXIT_FAILURE);
  }

  strlcpy(me.name, ConfigServerInfo.name, sizeof(me.name));

  /* serverinfo{} description must exist.  If not, error out.*/
  if (EmptyString(ConfigServerInfo.description))
  {
    ilog(LOG_TYPE_IRCD, "ERROR: No server description specified in serverinfo block.");
    exit(EXIT_FAILURE);
  }

  strlcpy(me.info, ConfigServerInfo.description, sizeof(me.info));

  me.from                   = &me;
  me.servptr                = &me;
  me.localClient->lasttime  = CurrentTime;
  me.localClient->since     = CurrentTime;
  me.localClient->firsttime = CurrentTime;

  SetMe(&me);
  make_server(&me);

  hash_add_id(&me);
  hash_add_client(&me);

  dlinkAdd(&me, make_dlink_node(), &global_server_list);

  load_kline_database();
  load_dline_database();
  load_gline_database();
  load_xline_database();
  load_resv_database();

  if (chdir(MODPATH))
  {
    ilog(LOG_TYPE_IRCD, "Could not load core modules. Terminating!");
    exit(EXIT_FAILURE);
  }

  load_all_modules(1);
  load_conf_modules();
  load_core_modules(1);

  /* Go back to DPATH after checking to see if we can chdir to MODPATH */
  if (chdir(ConfigGeneral.dpath))
  {
    perror("chdir");
    exit(EXIT_FAILURE);
  }

  /*
   * assemble_umode_buffer() has to be called after
   * reading conf/loading modules.
   */
  assemble_umode_buffer();

  write_pidfile(pidFileName);

  ilog(LOG_TYPE_IRCD, "Server Ready");

  event_addish(&event_cleanup_glines, NULL);
  event_addish(&event_cleanup_tklines, NULL);

  /* We want try_connections to be called as soon as possible now! -- adrian */
  /* No, 'cause after a restart it would cause all sorts of nick collides */
  event_addish(&event_try_connections, NULL);

  /* Setup the timeout check. I'll shift it later :)  -- adrian */
  event_add(&event_comm_checktimeouts, NULL);

  event_addish(&event_save_all_databases, NULL);

  if (ConfigServerHide.links_delay > 0)
  {
    event_write_links_file.when = ConfigServerHide.links_delay;
    event_addish(&event_write_links_file, NULL);
  }
  else
    ConfigServerHide.links_disabled = 1;

  if (splitmode)
    event_addish(&splitmode_event, NULL);

  io_loop();
  return 0;
}
