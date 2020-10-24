void
delete_user_host(const char *user, const char *host, int global)
{
  dlink_node *ptr = NULL;
  struct UserHost *found_userhost = NULL;
  unsigned int hasident = 1;

  if (*user == '~')
  {
    hasident = 0;
    ++user;
  }

  if ((found_userhost = hash_find_userhost(host)) == NULL)
    return;

  DLINK_FOREACH(ptr, found_userhost->list.head)
  {
    struct NameHost *nameh = ptr->data;

    if (!irccmp(user, nameh->name))
    {
      if (nameh->gcount > 0)
        nameh->gcount--;
      if (!global)
      {
        if (nameh->lcount > 0)
          nameh->lcount--;
        if (hasident && nameh->icount > 0)
          nameh->icount--;
      }

      if (nameh->gcount == 0 && nameh->lcount == 0)
      {
        dlinkDelete(&nameh->node, &found_userhost->list);
        mp_pool_release(nameh);
      }

      if (dlink_list_length(&found_userhost->list) == 0)
      {
        hash_del_userhost(found_userhost);
        mp_pool_release(found_userhost);
      }

      return;
    }
  }
}
