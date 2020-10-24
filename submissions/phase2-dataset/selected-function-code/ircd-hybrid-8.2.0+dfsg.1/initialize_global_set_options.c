initialize_global_set_options(void)
{
  GlobalSetOptions.autoconn  = 1;
  GlobalSetOptions.spam_time = MIN_JOIN_LEAVE_TIME;
  GlobalSetOptions.spam_num  = MAX_JOIN_LEAVE_COUNT;

  if (ConfigGeneral.default_floodcount)
    GlobalSetOptions.floodcount = ConfigGeneral.default_floodcount;
  else
    GlobalSetOptions.floodcount = 10;

  /* XXX I have no idea what to try here - Dianora */
  GlobalSetOptions.joinfloodcount = 16;
  GlobalSetOptions.joinfloodtime = 8;

  split_servers = ConfigChannel.default_split_server_count;
  split_users   = ConfigChannel.default_split_user_count;

  if (split_users && split_servers && (ConfigChannel.no_create_on_split ||
                                       ConfigChannel.no_join_on_split))
  {
    splitmode     = 1;
    splitchecking = 1;
  }

  GlobalSetOptions.ident_timeout = IDENT_TIMEOUT;
}
