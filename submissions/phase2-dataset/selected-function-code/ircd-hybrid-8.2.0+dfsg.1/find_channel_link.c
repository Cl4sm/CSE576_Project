struct Membership *
find_channel_link(struct Client *client_p, struct Channel *chptr)
{
  dlink_node *ptr = NULL;

  if (!IsClient(client_p))
    return NULL;

  if (dlink_list_length(&chptr->members) < dlink_list_length(&client_p->channel))
  {
    DLINK_FOREACH(ptr, chptr->members.head)
      if (((struct Membership *)ptr->data)->client_p == client_p)
        return ptr->data;
  }
  else
  {
    DLINK_FOREACH(ptr, client_p->channel.head)
      if (((struct Membership *)ptr->data)->chptr == chptr)
        return ptr->data;
  }

  return NULL;
}
