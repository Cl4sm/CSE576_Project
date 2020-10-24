hostmask_expire_temporary(void)
{
  dlink_node *ptr = NULL, *ptr_next = NULL;

  for (unsigned int i = 0; i < ATABLE_SIZE; ++i)
  {
    DLINK_FOREACH_SAFE(ptr, ptr_next, atable[i].head)
    {
      struct AddressRec *arec = ptr->data;

      if (!arec->conf->until || arec->conf->until > CurrentTime)
        continue;

      switch (arec->type)
      {
        case CONF_KLINE:
        case CONF_DLINE:
        case CONF_GLINE:
          hostmask_send_expiration(arec);

          dlinkDelete(&arec->node, &atable[i]);
          conf_free(arec->conf);
          MyFree(arec);
          break;
        default: break;
      }
    }
  }
}
