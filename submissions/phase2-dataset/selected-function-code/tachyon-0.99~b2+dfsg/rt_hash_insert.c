int rt_hash_insert(rt_hash_t *tptr, const char *key, int data) {
  int tmp;
  hash_node_t *node;
  int h;


  /* check to see if the entry exists */
  if ((tmp=rt_hash_lookup(tptr, key)) != HASH_FAIL)
    return(tmp);

  /* expand the table if needed */
  while (tptr->entries>=HASH_LIMIT*tptr->size)
    rebuild_table(tptr);

  /* insert the new entry */
  h=hash(tptr, key);
  node=(struct hash_node_t *) malloc(sizeof(hash_node_t));
  node->data=data;
  node->key=key;
  node->next=tptr->bucket[h];
  tptr->bucket[h]=node;
  tptr->entries++;

  return HASH_FAIL;
}
