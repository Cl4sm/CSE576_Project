static void
ipcache_remove_expired_entries(void *unused)
{
  dlink_node *ptr = NULL, *ptr_next = NULL;

  for (unsigned int i = 0; i < IP_HASH_SIZE; ++i)
  {
    DLINK_FOREACH_SAFE(ptr, ptr_next, ip_hash_table[i].head)
    {
      struct ip_entry *iptr = ptr->data;

      if (iptr->count == 0 &&
          (CurrentTime - iptr->last_attempt) >= ConfigGeneral.throttle_time)
      {
        dlinkDelete(&iptr->node, &ip_hash_table[i]);
        mp_pool_release(iptr);
      }
    }
  }
}
