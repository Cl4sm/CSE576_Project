hash_find_client(const char *name)
{
  unsigned int hashv = strhash(name);
  struct Client *client_p;

  if ((client_p = clientTable[hashv]))
  {
    if (irccmp(name, client_p->name))
    {
      struct Client *prev;

      while (prev = client_p, (client_p = client_p->hnext))
      {
        if (!irccmp(name, client_p->name))
        {
          prev->hnext = client_p->hnext;
          client_p->hnext = clientTable[hashv];
          clientTable[hashv] = client_p;
          break;
        }
      }
    }
  }

  return client_p;
}
