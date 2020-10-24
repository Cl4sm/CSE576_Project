save_resv_database(void)
{
  uint32_t records = 0;
  struct dbFILE *f = NULL;
  dlink_node *ptr = NULL;
  struct MaskItem *conf = NULL;

  if (!(f = open_db(RESVPATH, "w", KLINE_DB_VERSION)))
    return;

  DLINK_FOREACH(ptr, cresv_items.head)
  {
    conf = ptr->data;

    if (IsConfDatabase(conf))
      ++records;
  }

  DLINK_FOREACH(ptr, nresv_items.head)
  {
    conf = ptr->data;

    if (IsConfDatabase(conf))
      ++records;
  }

  SAFE_WRITE(write_uint32(records, f), RESVPATH);

  DLINK_FOREACH(ptr, cresv_items.head)
  {
    conf = ptr->data;

    if (!IsConfDatabase(conf))
      continue;

    SAFE_WRITE(write_string(conf->name, f), RESVPATH);
    SAFE_WRITE(write_string(conf->reason, f), RESVPATH);
    SAFE_WRITE(write_uint64(conf->setat, f), RESVPATH);
    SAFE_WRITE(write_uint64(conf->until, f), RESVPATH);
  }

  DLINK_FOREACH(ptr, nresv_items.head)
  {
    conf = ptr->data;

    if (!IsConfDatabase(conf))
      continue;

    SAFE_WRITE(write_string(conf->name, f), RESVPATH);
    SAFE_WRITE(write_string(conf->reason, f), RESVPATH);
    SAFE_WRITE(write_uint64(conf->setat, f), RESVPATH);
    SAFE_WRITE(write_uint64(conf->until, f), RESVPATH);
  }

  close_db(f);
}
