void
save_xline_database(void)
{
  uint32_t records = 0;
  struct dbFILE *f = NULL;
  dlink_node *ptr = NULL;
  struct MaskItem *conf = NULL;

  if (!(f = open_db(XPATH, "w", KLINE_DB_VERSION)))
    return;

  DLINK_FOREACH(ptr, xconf_items.head)
  {
    conf = ptr->data;

    if (IsConfDatabase(conf))
      ++records;
  }

  SAFE_WRITE(write_uint32(records, f), XPATH);

  DLINK_FOREACH(ptr, xconf_items.head)
  {
    conf = ptr->data;

    if (!IsConfDatabase(conf))
      continue;

    SAFE_WRITE(write_string(conf->name, f), XPATH);
    SAFE_WRITE(write_string(conf->reason, f), XPATH);
    SAFE_WRITE(write_uint64(conf->setat, f), XPATH);
    SAFE_WRITE(write_uint64(conf->until, f), XPATH);
  }

  close_db(f);
}
