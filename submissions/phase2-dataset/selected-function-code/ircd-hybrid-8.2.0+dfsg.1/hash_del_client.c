void
hash_del_client(struct Client *client_p)
{
  unsigned int hashv = strhash(client_p->name);
  struct Client *tmp = clientTable[hashv];

  if (tmp)
  {
    if (tmp == client_p)
    {
      clientTable[hashv] = client_p->hnext;
      client_p->hnext = client_p;
    }
    else
    {
      while (tmp->hnext != client_p)
        if ((tmp = tmp->hnext) == NULL)
          return;

      tmp->hnext = tmp->hnext->hnext;
      client_p->hnext = client_p;
    }
  }
}
