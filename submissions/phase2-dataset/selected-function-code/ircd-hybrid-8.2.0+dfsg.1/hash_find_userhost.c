struct UserHost *
hash_find_userhost(const char *host)
{
  unsigned int hashv = strhash(host);
  struct UserHost *userhost;

  if ((userhost = userhostTable[hashv]))
  {
    if (irccmp(host, userhost->host))
    {
      struct UserHost *prev;

      while (prev = userhost, (userhost = userhost->next))
      {
        if (!irccmp(host, userhost->host))
        {
          prev->next = userhost->next;
          userhost->next = userhostTable[hashv];
          userhostTable[hashv] = userhost;
          break;
        }
      }
    }
  }

  return userhost;
}
