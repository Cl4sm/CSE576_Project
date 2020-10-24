uint32_t LC_ReaderFlags_fromDb(GWEN_DB_NODE *db, const char *name) {
  int i;
  const char *p;
  uint32_t flags=0;

  for (i=0; ; i++) {
    p=GWEN_DB_GetCharValue(db, name, i, 0);
    if (!p)
      break;
    if (strcasecmp(p, "keypad")==0)
      flags|=LC_READER_FLAGS_KEYPAD;
    else if (strcasecmp(p, "display")==0)
      flags|=LC_READER_FLAGS_DISPLAY;
    else if (strcasecmp(p, "noinfo")==0)
      flags|=LC_READER_FLAGS_NOINFO;
    else if (strcasecmp(p, "remote")==0)
      flags|=LC_READER_FLAGS_REMOTE;
    else if (strcasecmp(p, "auto")==0)
      flags|=LC_READER_FLAGS_AUTO;
    else if (strcasecmp(p, "suspended_checks")==0)
      flags|=LC_READER_FLAGS_SUSPENDED_CHECKS;
    else if (strcasecmp(p, "driverHasVerify")==0)
      flags|=LC_READER_FLAGS_DRIVER_HAS_VERIFY;
    else if (strcasecmp(p, "keepRunning")==0)
      flags|=LC_READER_FLAGS_KEEP_RUNNING;
    else if (strcasecmp(p, "lowWriteBoundary")==0)
      flags|=LC_READER_FLAGS_LOW_WRITE_BOUNDARY;
    else if (strcasecmp(p, "noMemorySw")==0)
      flags|=LC_READER_FLAGS_NO_MEMORY_SW;
    else {
      DBG_WARN(0, "Unknown flag \"%s\", ignoring", p);
    }
  } /* for */

  return flags;
}