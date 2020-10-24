user_welcome(struct Client *source_p)
{
#if defined(__TIME__) && defined(__DATE__)
  static const char built_date[] = __DATE__ " at " __TIME__;
#else
  static const char built_date[] = "unknown";
#endif

#ifdef HAVE_LIBCRYPTO
  if (HasFlag(source_p, FLAGS_SSL))
  {
    AddUMode(source_p, UMODE_SSL);
    sendto_one_notice(source_p, &me, ":*** Connected securely via %s",
                      ssl_get_cipher(source_p->localClient->fd.ssl));
  }
#endif

  sendto_one_numeric(source_p, &me, RPL_WELCOME, ConfigServerInfo.network_name,
                     source_p->name);
  sendto_one_numeric(source_p, &me, RPL_YOURHOST,
                     get_listener_name(source_p->localClient->listener), ircd_version);
  sendto_one_numeric(source_p, &me, RPL_CREATED, built_date);
  sendto_one_numeric(source_p, &me, RPL_MYINFO, me.name, ircd_version, umode_buffer);
  show_isupport(source_p);
  sendto_one_numeric(source_p, &me, RPL_YOURID, source_p->id);

  show_lusers(source_p);
  motd_signon(source_p);
}
