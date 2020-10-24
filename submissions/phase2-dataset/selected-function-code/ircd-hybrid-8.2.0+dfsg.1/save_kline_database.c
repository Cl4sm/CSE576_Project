void
save_kline_database(void)
{
  uint32_t i = 0;
  uint32_t records = 0;
  struct dbFILE *f = NULL;
  dlink_node *ptr = NULL;

  if (!(f = open_db(KPATH, "w", KLINE_DB_VERSION)))
    return;

  for (i = 0; i < ATABLE_SIZE; ++i)
  {
    DLINK_FOREACH(ptr, atable[i].head)
    {
      struct AddressRec *arec = ptr->data;

      if (arec->type == CONF_KLINE && IsConfDatabase(arec->conf))
        ++records;
    }
  }

  SAFE_WRITE(write_uint32(records, f), KPATH);

  for (i = 0; i < ATABLE_SIZE; ++i)
  {
    DLINK_FOREACH(ptr, atable[i].head)
    {
      struct AddressRec *arec = ptr->data;

      if (arec->type == CONF_KLINE && IsConfDatabase(arec->conf))
      {
        SAFE_WRITE(write_string(arec->conf->user, f), KPATH);
        SAFE_WRITE(write_string(arec->conf->host, f), KPATH);
        SAFE_WRITE(write_string(arec->conf->reason, f), KPATH);
        SAFE_WRITE(write_uint64(arec->conf->setat, f), KPATH);
        SAFE_WRITE(write_uint64(arec->conf->until, f), KPATH);
      }
    }
  }

  close_db(f);
}
