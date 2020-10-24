GWEN_DB_NODE *LC_DriverInfo_ReaderDbFromXml(GWEN_XMLNODE *node) {
  GWEN_DB_NODE *db;
  GWEN_XMLNODE *n;
  const char *rtype;
  const char *p;
  int i;

  db=GWEN_DB_Group_new("reader");
  rtype=GWEN_XMLNode_GetProperty(node, "name", 0);
  if (!rtype) {
    DBG_ERROR(0, "Reader in XML file has no name");
    GWEN_DB_Group_free(db);
    return 0;
  }
  GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
		       "readerType", rtype);

  p=GWEN_XMLNode_GetCharValue(node, "short", rtype);
  GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                       "shortName", p);

  p=GWEN_XMLNode_GetProperty(node, "busType", "serial");
  GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                       "busType", p);

  GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_DEFAULT,
		      "slots",
		      atoi(GWEN_XMLNode_GetProperty(node, "slots", "1")));

  GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_DEFAULT,
                      "ctn",
                      atoi(GWEN_XMLNode_GetProperty(node, "ctn", "1")));

  if (1==sscanf(GWEN_XMLNode_GetProperty(node, "busId", "-1"),
                "%i", &i))
    if (i!=-1)
      GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_DEFAULT,
                          "busId", i);
  if (1==sscanf(GWEN_XMLNode_GetProperty(node, "vendor", "-1"),
                "%i", &i))
    if (i!=-1)
      GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_DEFAULT,
                          "vendorId", i);
  if (1==sscanf(GWEN_XMLNode_GetProperty(node, "product", "-1"),
                "%i", &i))
    if (i!=-1)
      GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_DEFAULT,
                          "productId", i);

  if (1==sscanf(GWEN_XMLNode_GetProperty(node, "usbclass", "-1"),
                "%i", &i))
    if (i!=-1)
      GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_DEFAULT,
                          "usbclass", i);

  /* read flags */
  n=GWEN_XMLNode_FindNode(node, GWEN_XMLNodeTypeTag, "flags");
  if (n) {
    n=GWEN_XMLNode_FindFirstTag(n, "flag", 0, 0);
    while(n) {
      GWEN_XMLNODE *nData;

      nData=GWEN_XMLNode_GetFirstData(n);
      if (nData) {
	p=GWEN_XMLNode_GetData(nData);
	if (p)
	  GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
			       "flags", p);
      }
      n=GWEN_XMLNode_FindNextTag(n, "flag", 0, 0);
    } /* while */
  }

  /* read ports */
  n=GWEN_XMLNode_FindNode(node, GWEN_XMLNodeTypeTag, "ports");
  if (n) {
    GWEN_DB_NODE *dbPorts;

    dbPorts=GWEN_DB_GetGroup(db, GWEN_DB_FLAGS_OVERWRITE_GROUPS, "ports");
    n=GWEN_XMLNode_FindFirstTag(n, "port", 0, 0);
    while(n) {
      const char *vp;
      int i;

      vp=GWEN_XMLNode_GetProperty(n, "value", "0");
      if (1!=sscanf(vp, "%i", &i)) {
	DBG_ERROR(0, "Bad port value (%s), ignoring", vp);
      }
      else {
	GWEN_XMLNODE *nData;

	nData=GWEN_XMLNode_GetFirstData(n);
	if (nData) {
	  p=GWEN_XMLNode_GetData(nData);
	  if (p)
	    GWEN_DB_SetIntValue(dbPorts,
				GWEN_DB_FLAGS_DEFAULT,
				p, i);
	}
	else {
	  DBG_WARN(0, "No port name for value %d, ignoring", i);
	}
      }
      n=GWEN_XMLNode_FindNextTag(n, "port", 0, 0);
    } /* while */
  }

  /* read autoport settings */
  n=GWEN_XMLNode_FindNode(node, GWEN_XMLNodeTypeTag, "autoport");
  if (n) {
    GWEN_DB_NODE *dbAutoPort;
    const char *s;
    int i;

    dbAutoPort=GWEN_DB_GetGroup(db, GWEN_DB_FLAGS_OVERWRITE_GROUPS,
                                "autoport");

    s=GWEN_XMLNode_GetCharValue(n, "mode", 0);
    if (s) {
      GWEN_DB_SetCharValue(dbAutoPort,
                           GWEN_DB_FLAGS_DEFAULT,
                           "mode", s);
    }

    /* split busorder (for mode=="pos") */
    s=GWEN_XMLNode_GetCharValue(n, "busorder", 0);
    if (s) {
      char *scopy;
      char *psc;

      scopy=strdup(s);

      psc=scopy;

      while(psc && *psc) {
        char *p;

        p=strchr(psc, ' ');
        if (p)
          *(p++)=0;
        GWEN_DB_SetCharValue(dbAutoPort,
                             GWEN_DB_FLAGS_DEFAULT,
                             "busorder", psc);
        psc=p;
      }
      free(scopy);
    }

    /* sortkey (for mode=="pos") */
    s=GWEN_XMLNode_GetCharValue(n, "sortkey", 0);
    if (s) {
      GWEN_DB_SetCharValue(dbAutoPort,
                           GWEN_DB_FLAGS_DEFAULT,
                           "sortkey", s);
    }

    /* offset */
    i=GWEN_XMLNode_GetIntValue(n, "offset", 0);
    GWEN_DB_SetIntValue(dbAutoPort,
                        GWEN_DB_FLAGS_DEFAULT,
                        "offset", i);

  }

  p=GWEN_XMLNode_GetProperty(node, "addressType", "port");
  assert(p);
  GWEN_DB_SetCharValue(db,
                       GWEN_DB_FLAGS_DEFAULT,
                       "addressType", p);

  p=GWEN_XMLNode_GetProperty(node, "devicePathTmpl", 0);
  if (p)
    GWEN_DB_SetCharValue(db,
                         GWEN_DB_FLAGS_DEFAULT,
                         "devicePathTmpl", p);

  return db;
}