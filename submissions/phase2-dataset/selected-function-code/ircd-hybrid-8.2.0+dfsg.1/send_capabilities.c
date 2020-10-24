void
send_capabilities(struct Client *client_p, int cap_can_send)
{
  char buf[IRCD_BUFSIZE] = "";
  const dlink_node *ptr = NULL;

  DLINK_FOREACH(ptr, cap_list.head)
  {
    const struct Capability *cap = ptr->data;

    if (cap->cap & (cap_can_send|default_server_capabs))
    {
      strlcat(buf, cap->name, sizeof(buf));
      if (ptr->next)
        strlcat(buf, " ", sizeof(buf));
    }
  }

  sendto_one(client_p, "CAPAB :%s", buf);
}
