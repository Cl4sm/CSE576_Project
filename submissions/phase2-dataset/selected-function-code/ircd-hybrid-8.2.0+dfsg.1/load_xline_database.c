load_xline_database(void)
{
  uint32_t i = 0;
  uint32_t records = 0;
  uint64_t tmp64_hold = 0, tmp64_setat = 0;
  struct dbFILE *f = NULL;
  char *name = NULL;
  char *reason = NULL;
  struct MaskItem *conf = NULL;

  if (!(f = open_db(XPATH, "r", KLINE_DB_VERSION)))
    return;

  if (get_file_version(f) < 1)
  {
    close_db(f);
    return;
  }

  read_uint32(&records, f);

  for (i = 0; i < records; ++i)
  {
    SAFE_READ(read_string(&name, f));
    SAFE_READ(read_string(&reason, f));
    SAFE_READ(read_uint64(&tmp64_setat, f));
    SAFE_READ(read_uint64(&tmp64_hold, f));

    conf = conf_make(CONF_XLINE);

    SetConfDatabase(conf);

    conf->name = name;
    conf->reason = reason;
    conf->setat = tmp64_setat;
    conf->until = tmp64_hold;
  }

  close_db(f);
}
