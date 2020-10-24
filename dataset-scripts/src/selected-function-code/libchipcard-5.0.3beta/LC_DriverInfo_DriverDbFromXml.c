GWEN_DB_NODE *LC_DriverInfo_DriverDbFromXml(GWEN_XMLNODE *node,
					    int dontSearchDrivers) {
  GWEN_DB_NODE *db;
  GWEN_XMLNODE *n;
  GWEN_XMLNODE *nLib;
  const char *p;
  const char *dname;
  GWEN_STRINGLIST *slDirs;
  GWEN_STRINGLIST *slNames;
  GWEN_BUFFER *nbuf;

  db=GWEN_DB_Group_new("driver");
  dname=GWEN_XMLNode_GetProperty(node, "name", 0);
  if (!dname) {
    DBG_ERROR(0, "Driver in XML file has no name");
    GWEN_DB_Group_free(db);
    return 0;
  }
  GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
		       "driverName", dname);

  n=GWEN_XMLNode_FindFirstTag(node, "manufacturer", 0, 0);
  if (n) {
    p=GWEN_XMLNode_GetCharValue(n, "name", 0);
    if (p)
      GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                           "manufacturer", p);
    p=GWEN_XMLNode_GetCharValue(n, "url", 0);
    if (p)
      GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                           "url", p);
  }

  /* read variables */
  n=GWEN_XMLNode_FindFirstTag(node, "vars", "osname", OS_SHORTNAME);
  if (!n)
    n=GWEN_XMLNode_FindFirstTag(node, "vars", "ostype", OS_TYPE);
  if (!n)
    n=GWEN_XMLNode_FindFirstTag(node, "vars", "ostype", 0);
  if (!n) {
    n=GWEN_XMLNode_FindFirstTag(node, "vars", 0, 0);
    while(n) {
      if (GWEN_XMLNode_GetProperty(n, "osname", 0)==0 &&
          GWEN_XMLNode_GetProperty(n, "ostype", 0)==0)
        break;
      n=GWEN_XMLNode_FindNextTag(n, "vars", 0, 0);
    } /* while */
  }
  if (n) {
    GWEN_DB_NODE *dbVars;
    GWEN_XMLNODE *nn;

    dbVars=GWEN_DB_GetGroup(db, GWEN_DB_FLAGS_OVERWRITE_GROUPS, "vars");
    assert(dbVars);
    nn=GWEN_XMLNode_FindFirstTag(n, "var", 0, 0);
    while(nn) {
      const char *name;
      const char *value;
      GWEN_XMLNODE *nd;

      name=GWEN_XMLNode_GetProperty(nn, "name", 0);
      assert(name);

      nd=GWEN_XMLNode_GetFirstData(nn);
      if (nd)
	value=GWEN_XMLNode_GetData(nd);
      else
	value="";
      GWEN_DB_SetCharValue(dbVars, GWEN_DB_FLAGS_DEFAULT,
			   name, value);

      nn=GWEN_XMLNode_FindNextTag(nn, "var", 0, 0);
    } /* while */
  }


  p=GWEN_XMLNode_GetProperty(node, "type", 0);
  if (!p) {
    DBG_ERROR(0, "Driver \"%s\" in XML file has no type",
	      dname);
    GWEN_DB_Group_free(db);
    return 0;
  }
  GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
		       "driverType", p);

  GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_DEFAULT,
		      "maxReaders",
		      atoi(GWEN_XMLNode_GetProperty(node,
						    "maxReaders",
						    "1")));

  p=GWEN_XMLNode_GetCharValue(node, "short", 0);
  if (p)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         "short", p);

  if (!dontSearchDrivers) {
    nLib=GWEN_XMLNode_FindFirstTag(node, "lib", "osname", OS_SHORTNAME);
    if (!nLib)
      nLib=GWEN_XMLNode_FindFirstTag(node, "lib", "ostype", OS_TYPE);
    if (!nLib) {
      nLib=GWEN_XMLNode_FindFirstTag(node, "lib", 0, 0);
      while(nLib) {
	if (GWEN_XMLNode_GetProperty(nLib, "osname", 0)==0 &&
	    GWEN_XMLNode_GetProperty(nLib, "ostype", 0)==0)
	  break;
	nLib=GWEN_XMLNode_FindNextTag(nLib, "lib", 0, 0);
      } /* while */
    }
    if (nLib) {
      /* fetch dirs */
      n=GWEN_XMLNode_FindFirstTag(nLib, "locations", 0, 0);
      if (!n) {
	DBG_ERROR(0, "No locations given for driver \"%s\"", dname);
	GWEN_DB_Group_free(db);
	return 0;
      }
  
      slDirs=GWEN_StringList_new();

      n=GWEN_XMLNode_FindFirstTag(n, "loc", 0, 0);
      while(n) {
	GWEN_XMLNODE *nData;
  
	nData=GWEN_XMLNode_GetFirstData(n);
	if (n) {
	  p=GWEN_XMLNode_GetData(nData);
	  if (p)
	    GWEN_StringList_AppendString(slDirs,
					 p, 0, 1);
	}
	n=GWEN_XMLNode_FindNextTag(n, "loc", 0, 0);
      } /* while */
  
      /* fetch names */
      n=GWEN_XMLNode_FindFirstTag(nLib, "names", 0, 0);
      if (!n) {
	DBG_ERROR(0, "No names given for driver \"%s\"", dname);
	GWEN_StringList_free(slDirs);
	GWEN_DB_Group_free(db);
	return 0;
      }
  
      slNames=GWEN_StringList_new();
      n=GWEN_XMLNode_FindFirstTag(n, "name", 0, 0);
      while(n) {
	GWEN_XMLNODE *nData;
  
	nData=GWEN_XMLNode_GetFirstData(n);
	if (n) {
	  p=GWEN_XMLNode_GetData(nData);
	  if (p)
	    GWEN_StringList_AppendString(slNames,
					 p, 0, 1);
	}
	n=GWEN_XMLNode_FindNextTag(n, "name", 0, 0);
      } /* while */
  
      nbuf=GWEN_Buffer_new(0, 256, 0, 1);
      if (!LC_DriverInfo_FindFile(slDirs, slNames, nbuf)) {
	GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
			     "libraryFile", GWEN_Buffer_GetStart(nbuf));
      }
      GWEN_Buffer_free(nbuf);
      GWEN_StringList_free(slNames);
      GWEN_StringList_free(slDirs);
    }
    else {
      GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
			   "libraryFile", "none");
    }
  }

  /* read driver flags */
  n=GWEN_XMLNode_FindFirstTag(node, "flags", "osname", OS_SHORTNAME);
  if (!n)
    n=GWEN_XMLNode_FindFirstTag(node, "flags", "ostype", OS_TYPE);
  if (!n)
    n=GWEN_XMLNode_FindFirstTag(node, "flags", "ostype", 0);
  if (!n) {
    n=GWEN_XMLNode_FindFirstTag(node, "flags", 0, 0);
    while(n) {
      if (GWEN_XMLNode_GetProperty(n, "osname", 0)==0 &&
          GWEN_XMLNode_GetProperty(n, "ostype", 0)==0)
        break;
      n=GWEN_XMLNode_FindNextTag(n, "flags", 0, 0);
    } /* while */
  }
  if (n) {
    GWEN_XMLNODE *nn;

    nn=GWEN_XMLNode_FindFirstTag(n, "flag", 0, 0);
    while(nn) {
      const char *value;
      GWEN_XMLNODE *nd;

      nd=GWEN_XMLNode_GetFirstData(nn);
      if (nd)
	value=GWEN_XMLNode_GetData(nd);
      else
	value="";
      GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
			   "flags", value);

      nn=GWEN_XMLNode_FindNextTag(nn, "flag", 0, 0);
    } /* while */
  }

  return db;
}