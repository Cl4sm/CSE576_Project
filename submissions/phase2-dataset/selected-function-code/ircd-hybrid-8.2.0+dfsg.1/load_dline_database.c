void
load_dline_database(void)
{
  struct dbFILE *f = NULL;
  struct MaskItem *conf = NULL;
  char *field_1 = NULL;
  char *field_2 = NULL;
  uint32_t i = 0;
  uint32_t records = 0;
  uint64_t field_3 = 0;
  uint64_t field_4 = 0;

  if (!(f = open_db(DLPATH, "r", KLINE_DB_VERSION)))
    return;

  if (get_file_version(f) < 1)
  {
    close_db(f);
    return;
  }

  read_uint32(&records, f);

  for (i = 0; i < records; ++i)
  {
    SAFE_READ(read_string(&field_1, f));
    SAFE_READ(read_string(&field_2, f));
    SAFE_READ(read_uint64(&field_3, f));
    SAFE_READ(read_uint64(&field_4, f));

    conf = conf_make(CONF_DLINE);
    conf->host = field_1;
    conf->reason = field_2;
    conf->setat = field_3;
    conf->until = field_4;
    SetConfDatabase(conf);

    add_conf_by_address(CONF_DLINE, conf);
  }

  close_db(f);
}
