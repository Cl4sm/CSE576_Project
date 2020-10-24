hash_find_id(const char *name)
{
  unsigned int hashv = strhash(name);
  struct Client *client_p;

  if ((client_p = idTable[hashv]))
  {
    if (strcmp(name, client_p->id))
    {
      struct Client *prev;

      while (prev = client_p, (client_p = client_p->idhnext))
      {
        if (!strcmp(name, client_p->id))
        {
          prev->idhnext = client_p->idhnext;
          client_p->idhnext = idTable[hashv];
          idTable[hashv] = client_p;
          break;
        }
      }
    }
  }

  return client_p;
}
