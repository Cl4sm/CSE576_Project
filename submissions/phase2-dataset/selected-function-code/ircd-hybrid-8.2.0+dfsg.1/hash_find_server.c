struct Client *
hash_find_server(const char *name)
{
  unsigned int hashv = strhash(name);
  struct Client *client_p = NULL;

  if (IsDigit(*name) && strlen(name) == IRC_MAXSID)
    return hash_find_id(name);

  if ((client_p = clientTable[hashv]))
  {
    if ((!IsServer(client_p) && !IsMe(client_p)) ||
        irccmp(name, client_p->name))
    {
      struct Client *prev;

      while (prev = client_p, (client_p = client_p->hnext))
      {
        if ((IsServer(client_p) || IsMe(client_p)) &&
            !irccmp(name, client_p->name))
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
