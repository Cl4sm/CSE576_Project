void
channel_do_join_0(struct Client *source_p)
{
  dlink_node *ptr = NULL, *ptr_next = NULL;

  if (source_p->channel.head)
    if (MyConnect(source_p) && !HasUMode(source_p, UMODE_OPER))
      check_spambot_warning(source_p, NULL);

  DLINK_FOREACH_SAFE(ptr, ptr_next, source_p->channel.head)
  {
    struct Channel *chptr = ((struct Membership *)ptr->data)->chptr;

    sendto_server(source_p, NOCAPS, NOCAPS, ":%s PART %s",
                  source_p->id, chptr->chname);
    sendto_channel_local(ALL_MEMBERS, 0, chptr, ":%s!%s@%s PART %s",
                         source_p->name, source_p->username,
                         source_p->host, chptr->chname);

    remove_user_from_channel(ptr->data);
  }
}
