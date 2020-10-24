  hash_node_t *node, *last;
  int data;
  int h;

  /* find the node to remove */
  h=hash(tptr, key);
  for (node=tptr->bucket[h]; node; node=node->next) {
    if (!strcmp(node->key, key))
      break;
  }

  /* Didn't find anything, return HASH_FAIL */
  if (node==NULL)
    return HASH_FAIL;

  /* if node is at head of bucket, we have it easy */
  if (node==tptr->bucket[h])
    tptr->bucket[h]=node->next;
  else {
    /* find the node before the node we want to remove */
    for (last=tptr->bucket[h]; last && last->next; last=last->next) {
      if (last->next==node)
        break;
    }
    last->next=node->next;
  }

  /* free memory and return the data */
  data=node->data;
  free(node);

  return(data);
}
