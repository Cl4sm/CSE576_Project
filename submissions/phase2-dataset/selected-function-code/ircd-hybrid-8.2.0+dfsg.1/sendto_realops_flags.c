sendto_realops_flags(unsigned int flags, int level, int type, const char *pattern, ...)
{
  const char *ntype = NULL;
  dlink_node *ptr = NULL;
  char nbuf[IRCD_BUFSIZE] = "";
  va_list args;

  va_start(args, pattern);
  vsnprintf(nbuf, sizeof(nbuf), pattern, args);
  va_end(args);

  switch (type)
  {
    case SEND_NOTICE:
      ntype = "Notice";
      break;
    case SEND_GLOBAL:
      ntype = "Global";
      break;
    case SEND_LOCOPS:
      ntype = "LocOps";
      break;
    default:
      assert(0);
  }

  DLINK_FOREACH(ptr, oper_list.head)
  {
    struct Client *client_p = ptr->data;
    assert(HasUMode(client_p, UMODE_OPER));

    /*
     * If we're sending it to opers and they're an admin, skip.
     * If we're sending it to admins, and they're not, skip.
     */
    if (((level == L_ADMIN) && !HasUMode(client_p, UMODE_ADMIN)) ||
        ((level == L_OPER) && HasUMode(client_p, UMODE_ADMIN)))
      continue;

    if (HasUMode(client_p, flags))
      sendto_one(client_p, ":%s NOTICE %s :*** %s -- %s",
                 me.name, client_p->name, ntype, nbuf);
  }
}
