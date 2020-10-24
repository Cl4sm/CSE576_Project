void
hash_del_userhost(struct UserHost *userhost)
{
  unsigned int hashv = strhash(userhost->host);
  struct UserHost *tmp = userhostTable[hashv];

  if (tmp)
  {
    if (tmp == userhost)
    {
      userhostTable[hashv] = userhost->next;
      userhost->next = userhost;
    }
    else
    {
      while (tmp->next != userhost)
        if ((tmp = tmp->next) == NULL)
          return;

      tmp->next = tmp->next->next;
      userhost->next = userhost;
    }
  }
}
