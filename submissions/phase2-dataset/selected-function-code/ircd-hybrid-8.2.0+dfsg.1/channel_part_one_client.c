static void
channel_part_one_client(struct Client *source_p, const char *name, const char *reason)
{
  struct Channel *chptr = NULL;
  struct Membership *ms = NULL;

  if ((chptr = hash_find_channel(name)) == NULL)
  {
    sendto_one_numeric(source_p, &me, ERR_NOSUCHCHANNEL, name);
    return;
  }

  if ((ms = find_channel_link(source_p, chptr)) == NULL)
  {
    sendto_one_numeric(source_p, &me, ERR_NOTONCHANNEL, chptr->chname);
    return;
  }

  if (MyConnect(source_p) && !HasUMode(source_p, UMODE_OPER))
    check_spambot_warning(source_p, NULL);

  /*
   * Remove user from the old channel (if any)
   * only allow /part reasons in -m chans
   */
  if (*reason && (!MyConnect(source_p) ||
      ((can_send(chptr, source_p, ms, reason) &&
       (source_p->localClient->firsttime + ConfigGeneral.anti_spam_exit_message_time)
        < CurrentTime))))
  {
    sendto_server(source_p, NOCAPS, NOCAPS, ":%s PART %s :%s",
                  source_p->id, chptr->chname, reason);
    sendto_channel_local(ALL_MEMBERS, 0, chptr, ":%s!%s@%s PART %s :%s",
                         source_p->name, source_p->username,
                         source_p->host, chptr->chname, reason);
  }
  else
  {
    sendto_server(source_p, NOCAPS, NOCAPS, ":%s PART %s",
                  source_p->id, chptr->chname);
    sendto_channel_local(ALL_MEMBERS, 0, chptr, ":%s!%s@%s PART %s",
                         source_p->name, source_p->username,
                         source_p->host, chptr->chname);
  }

  remove_user_from_channel(ms);
}
