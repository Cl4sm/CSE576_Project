motd_signon(struct Client *source_p)
{
  const struct MotdCache *cache = motd_cache(motd_lookup(source_p));

  if (!ConfigGeneral.short_motd || !cache)
    motd_forward(source_p, cache);
  else
  {
    sendto_one_notice(source_p, &me, ":*** Notice -- motd was last changed at %d/%d/%d %d:%02d",
                      cache->modtime.tm_year + 1900,
                      cache->modtime.tm_mon + 1,
                      cache->modtime.tm_mday,
                      cache->modtime.tm_hour,
                      cache->modtime.tm_min);
    sendto_one_notice(source_p, &me, ":*** Notice -- Please read the motd if you haven't read it");
    sendto_one_numeric(source_p, &me, RPL_MOTDSTART, me.name);
    sendto_one_numeric(source_p, &me, RPL_MOTD,
                       "*** This is the short motd ***");
    sendto_one_numeric(source_p, &me, RPL_ENDOFMOTD);
  }
}
