hash_del_id(struct Client *client_p)
{
  unsigned int hashv = strhash(client_p->id);
  struct Client *tmp = idTable[hashv];

  if (tmp)
  {
    if (tmp == client_p)
    {
      idTable[hashv] = client_p->idhnext;
      client_p->idhnext = client_p;
    }
    else
    {
      while (tmp->idhnext != client_p)
        if ((tmp = tmp->idhnext) == NULL)
          return;

      tmp->idhnext = tmp->idhnext->idhnext;
      client_p->idhnext = client_p;
    }
  }
}
