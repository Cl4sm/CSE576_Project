void
read_conf_files(int cold)
{
  const char *filename = NULL;
  char chanmodes[IRCD_BUFSIZE] = "";
  char chanlimit[IRCD_BUFSIZE] = "";

  conf_parser_ctx.boot = cold;
  filename = ConfigGeneral.configfile;

  /* We need to know the initial filename for the yyerror() to report
     FIXME: The full path is in conffilenamebuf first time since we
             don't know anything else

     - Gozem 2002-07-21
  */
  strlcpy(conffilebuf, filename, sizeof(conffilebuf));

  if ((conf_parser_ctx.conf_file = fopen(filename, "r")) == NULL)
  {
    if (cold)
    {
      ilog(LOG_TYPE_IRCD, "Unable to read configuration file '%s': %s",
           filename, strerror(errno));
      exit(-1);
    }
    else
    {
      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "Unable to read configuration file '%s': %s",
                           filename, strerror(errno));
      return;
    }
  }

  if (!cold)
    clear_out_old_conf();

  read_conf(conf_parser_ctx.conf_file);
  fclose(conf_parser_ctx.conf_file);

  log_reopen_all();

  add_isupport("NICKLEN", NULL, ConfigServerInfo.max_nick_length);
  add_isupport("NETWORK", ConfigServerInfo.network_name, -1);

  snprintf(chanmodes, sizeof(chanmodes), "beI:%d", ConfigChannel.max_bans);
  add_isupport("MAXLIST", chanmodes, -1);
  add_isupport("MAXTARGETS", NULL, ConfigGeneral.max_targets);
  add_isupport("CHANTYPES", "#", -1);

  snprintf(chanlimit, sizeof(chanlimit), "#:%d",
           ConfigChannel.max_channels);
  add_isupport("CHANLIMIT", chanlimit, -1);
  snprintf(chanmodes, sizeof(chanmodes), "%s", "beI,k,l,cimnprstMORS");
  add_isupport("CHANNELLEN", NULL, CHANNELLEN);
  add_isupport("TOPICLEN", NULL, ConfigServerInfo.max_topic_length);
  add_isupport("CHANMODES", chanmodes, -1);

  /*
   * message_locale may have changed.  rebuild isupport since it relies
   * on strlen(form_str(RPL_ISUPPORT))
   */
  rebuild_isupport_message_line();
}
