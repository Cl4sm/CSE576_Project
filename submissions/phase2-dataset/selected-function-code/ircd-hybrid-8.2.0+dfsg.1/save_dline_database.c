void
save_dline_database(void)
{
  uint32_t i = 0;
  uint32_t records = 0;
  struct dbFILE *f = NULL;
  dlink_node *ptr = NULL;

  if (!(f = open_db(DLPATH, "w", KLINE_DB_VERSION)))
    return;

  for (i = 0; i < ATABLE_SIZE; ++i)
  {
    DLINK_FOREACH(ptr, atable[i].head)
    {
      struct AddressRec *arec = ptr->data;

      if (arec->type == CONF_DLINE && IsConfDatabase(arec->conf))
        ++records;
    }
  }

  SAFE_WRITE(write_uint32(records, f), DLPATH);

  for (i = 0; i < ATABLE_SIZE; ++i)
  {
    DLINK_FOREACH(ptr, atable[i].head)
    {
      struct AddressRec *arec = ptr->data;

      if (arec->type == CONF_DLINE && IsConfDatabase(arec->conf))
      {
        SAFE_WRITE(write_string(arec->conf->host, f), DLPATH);
        SAFE_WRITE(write_string(arec->conf->reason, f), DLPATH);
        SAFE_WRITE(write_uint64(arec->conf->setat, f), DLPATH);
        SAFE_WRITE(write_uint64(arec->conf->until, f), DLPATH);
      }
    }
  }

  close_db(f);
}
