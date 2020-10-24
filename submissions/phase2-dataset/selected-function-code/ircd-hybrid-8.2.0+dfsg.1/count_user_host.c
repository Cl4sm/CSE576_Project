void
count_user_host(const char *user, const char *host, unsigned int *global_p,
                unsigned int *local_p, unsigned int *icount_p)
{
  dlink_node *ptr;
  struct UserHost *found_userhost;
  struct NameHost *nameh;

  if ((found_userhost = hash_find_userhost(host)) == NULL)
    return;

  DLINK_FOREACH(ptr, found_userhost->list.head)
  {
    nameh = ptr->data;

    if (!irccmp(user, nameh->name))
    {
      if (global_p)
        *global_p = nameh->gcount;
      if (local_p)
        *local_p  = nameh->lcount;
      if (icount_p)
        *icount_p = nameh->icount;
      return;
    }
  }
}
