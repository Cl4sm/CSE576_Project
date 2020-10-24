load_gline_database(void)
{
  struct dbFILE *f = NULL;
  struct MaskItem *conf = NULL;
  char *field_1 = NULL;
  char *field_2 = NULL;
  char *field_3 = NULL;
  uint32_t i = 0;
  uint32_t records = 0;
  uint64_t field_4 = 0;
  uint64_t field_5 = 0;

  if (!(f = open_db(GPATH, "r", KLINE_DB_VERSION)))
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
    SAFE_READ(read_string(&field_3, f));
    SAFE_READ(read_uint64(&field_4, f));
    SAFE_READ(read_uint64(&field_5, f));

    conf = conf_make(CONF_GLINE);
    conf->user = field_1;
    conf->host = field_2;
    conf->reason = field_3;
    conf->setat = field_4;
    conf->until = field_5;
    SetConfDatabase(conf);

    add_conf_by_address(CONF_GLINE, conf);
  }

  close_db(f);
}
