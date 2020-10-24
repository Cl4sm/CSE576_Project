clear_out_address_conf(void)
{
  dlink_node *ptr = NULL, *ptr_next = NULL;

  for (unsigned int i = 0; i < ATABLE_SIZE; ++i)
  {
    DLINK_FOREACH_SAFE(ptr, ptr_next, atable[i].head)
    {
      struct AddressRec *arec = ptr->data;

      /*
       * We keep the temporary K-lines and destroy the permanent ones,
       * just to be confusing :) -A1kmm
       */
      if (arec->conf->until || IsConfDatabase(arec->conf))
        continue;

      dlinkDelete(&arec->node, &atable[i]);
      arec->conf->active = 0;

      if (!arec->conf->ref_count)
        conf_free(arec->conf);
      MyFree(arec);
    }
  }
}
