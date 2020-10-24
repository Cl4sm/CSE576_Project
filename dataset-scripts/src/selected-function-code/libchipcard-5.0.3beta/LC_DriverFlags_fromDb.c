uint32_t LC_DriverFlags_fromDb(GWEN_DB_NODE *db, const char *name) {
  const char *p;
  int i;
  uint32_t flags=0;

  for (i=0; ; i++) {
    p=GWEN_DB_GetCharValue(db, name, i, 0);
    if (!p)
      break;
    if (strcasecmp(p, "auto")==0)
      flags|=LC_DRIVER_FLAGS_AUTO;
    else if (strcasecmp(p, "remote")==0)
      flags|=LC_DRIVER_FLAGS_REMOTE;
    else if (strcasecmp(p, "has_verify_fn")==0)
      flags|=LC_DRIVER_FLAGS_HAS_VERIFY_FN;
    else if (strcasecmp(p, "has_modify_fn")==0)
      flags|=LC_DRIVER_FLAGS_HAS_MODIFY_FN;
    else if (strcasecmp(p, "config")==0)
      flags|=LC_DRIVER_FLAGS_CONFIG;
    else {
      DBG_WARN(0, "Unknown driver flag \"%s\"", p);
    }
  }

  return flags;
}