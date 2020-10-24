void
channel_member_names(struct Client *source_p, struct Channel *chptr,
                     int show_eon)
{
  const dlink_node *ptr = NULL;
  char buf[IRCD_BUFSIZE + 1] = "";
  char *t = NULL, *start = NULL;
  int tlen = 0;
  int is_member = IsMember(source_p, chptr);
  int multi_prefix = HasCap(source_p, CAP_MULTI_PREFIX) != 0;
  int uhnames = HasCap(source_p, CAP_UHNAMES) != 0;

  if (PubChannel(chptr) || is_member)
  {
    t = buf + snprintf(buf, sizeof(buf), numeric_form(RPL_NAMREPLY),
                       me.name, source_p->name,
                       channel_pub_or_secret(chptr), chptr->chname);
    start = t;

    DLINK_FOREACH(ptr, chptr->members.head)
    {
      const struct Membership *ms = ptr->data;

      if (HasUMode(ms->client_p, UMODE_INVISIBLE) && !is_member)
        continue;

      if (!uhnames)
        tlen = strlen(ms->client_p->name) + 1;  /* +1 for space */
      else
        tlen = strlen(ms->client_p->name) + strlen(ms->client_p->username) +
               strlen(ms->client_p->host) + 3;  /* +3 for ! + @ + space */

      if (!multi_prefix)
      {
        if (ms->flags & (CHFL_CHANOP | CHFL_HALFOP | CHFL_VOICE))
          ++tlen;
      }
      else
      {
        if (ms->flags & CHFL_CHANOP)
          ++tlen;
        if (ms->flags & CHFL_HALFOP)
          ++tlen;
        if (ms->flags & CHFL_VOICE)
          ++tlen;
      }

      if (t + tlen - buf > IRCD_BUFSIZE - 2)
      {
        *(t - 1) = '\0';
        sendto_one(source_p, "%s", buf);
        t = start;
      }

      if (!uhnames)
        t += sprintf(t, "%s%s ", get_member_status(ms, multi_prefix),
                     ms->client_p->name);
      else
        t += sprintf(t, "%s%s!%s@%s ", get_member_status(ms, multi_prefix),
                     ms->client_p->name, ms->client_p->username,
                     ms->client_p->host);
    }

    if (tlen)
    {
      *(t - 1) = '\0';
      sendto_one(source_p, "%s", buf);
    }
  }

  if (show_eon)
    sendto_one_numeric(source_p, &me, RPL_ENDOFNAMES, chptr->chname);
}
