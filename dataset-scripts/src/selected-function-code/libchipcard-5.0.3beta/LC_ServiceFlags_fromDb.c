uint32_t LC_ServiceFlags_fromDb(GWEN_DB_NODE *db, const char *name){
  const char *p;
  int i;
  uint32_t flags=0;

  for (i=0; ; i++) {
    p=GWEN_DB_GetCharValue(db, name, i, 0);
    if (!p)
      break;
    if (strcasecmp(p, "autoload")==0)
      flags|=LC_SERVICE_FLAGS_AUTOLOAD;
    else if (strcasecmp(p, "client")==0)
      flags|=LC_SERVICE_FLAGS_CLIENT;
    else if (strcasecmp(p, "silent")==0)
      flags|=LC_SERVICE_FLAGS_SILENT;
    else {
      DBG_WARN(0, "Unknown service flag \"%s\"", p);
    }
  }

  return flags;
}