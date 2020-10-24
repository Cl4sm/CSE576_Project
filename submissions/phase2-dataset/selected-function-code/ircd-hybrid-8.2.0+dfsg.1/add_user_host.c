void
add_user_host(const char *user, const char *host, int global)
{
  dlink_node *ptr;
  struct UserHost *found_userhost;
  struct NameHost *nameh;
  unsigned int hasident = 1;

  if (*user == '~')
  {
    hasident = 0;
    ++user;
  }

  if ((found_userhost = find_or_add_userhost(host)) == NULL)
    return;

  DLINK_FOREACH(ptr, found_userhost->list.head)
  {
    nameh = ptr->data;

    if (!irccmp(user, nameh->name))
    {
      nameh->gcount++;

      if (!global)
      {
        if (hasident)
          nameh->icount++;
        nameh->lcount++;
      }

      return;
    }
  }

  nameh = mp_pool_get(namehost_pool);

  strlcpy(nameh->name, user, sizeof(nameh->name));

  nameh->gcount = 1;

  if (!global)
  {
    if (hasident)
      nameh->icount = 1;
    nameh->lcount = 1;
  }

  dlinkAdd(nameh, &nameh->node, &found_userhost->list);
}
