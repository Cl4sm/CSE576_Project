uint32_t LC_ReaderFlags_fromXml(GWEN_XMLNODE *node, const char *name){
  const char *p;
  uint32_t flags=0;
  GWEN_XMLNODE *n;

  n=GWEN_XMLNode_FindFirstTag(node, name, 0, 0);
  while(n) {
    GWEN_XMLNODE *nn;

    nn=GWEN_XMLNode_GetFirstData(n);
    if (nn) {
      p=GWEN_XMLNode_GetData(nn);
      assert(p);

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
    }
    n=GWEN_XMLNode_FindNextTag(n, name, 0, 0);
  } /* while */

  return flags;
}