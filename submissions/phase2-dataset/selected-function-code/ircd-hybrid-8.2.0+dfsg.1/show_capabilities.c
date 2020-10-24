const char *
show_capabilities(const struct Client *target_p)
{
  static char msgbuf[IRCD_BUFSIZE] = "";
  const dlink_node *ptr = NULL;

  strlcpy(msgbuf, "TS", sizeof(msgbuf));

  DLINK_FOREACH(ptr, cap_list.head)
  {
    const struct Capability *cap = ptr->data;

    if (!IsCapable(target_p, cap->cap))
      continue;

    strlcat(msgbuf,       " ", sizeof(msgbuf));
    strlcat(msgbuf, cap->name, sizeof(msgbuf));
  }

  return msgbuf;
}
