int LC_DriverInfo_ReadDriverFile(const char *fname,
                                 GWEN_DB_NODE *dbDrivers,
				 int availOnly,
				 int dontSearchDrivers) {
  GWEN_XMLNODE *nFile;
  int driverAdded=0;

  nFile=GWEN_XMLNode_new(GWEN_XMLNodeTypeTag, "drivers");
  if (GWEN_XML_ReadFile(nFile, fname,
                        GWEN_XML_FLAGS_DEFAULT)) {
    DBG_INFO(0, "Could not read file \"%s\"", fname);
    return -1;
  }
  else {
    GWEN_XMLNODE *nDriver;

    nDriver=GWEN_XMLNode_FindNode(nFile, GWEN_XMLNodeTypeTag, "driver");
    if (!nDriver) {
      DBG_INFO(0, "XML file \"%s\" does not contain a driver", fname);
    }
    else {
      GWEN_DB_NODE *dbDriver;
  
      dbDriver=LC_DriverInfo_DriverDbFromXml(nDriver,
					     dontSearchDrivers);
      if (!dbDriver) {
        DBG_INFO(0, "Could not create driver from file \"%s\"", fname);
      }
      else {
        if (GWEN_DB_GetCharValue(dbDriver, "libraryFile", 0, 0) ||
	    !availOnly ||
	    dontSearchDrivers) {
          GWEN_XMLNODE *nReader;
  
          nReader=GWEN_XMLNode_FindFirstTag(nDriver, "readers",
                                            "osname", OS_SHORTNAME);
          if (!nReader)
            nReader=GWEN_XMLNode_FindFirstTag(nDriver, "readers",
                                              "ostype", OS_TYPE);
          if (!nReader) {
            nReader=GWEN_XMLNode_FindFirstTag(nDriver, "readers", 0, 0);
            while(nReader) {
              if (GWEN_XMLNode_GetProperty(nReader, "osname", 0)==0 &&
                  GWEN_XMLNode_GetProperty(nReader, "ostype", 0)==0)
                break;
              nReader=GWEN_XMLNode_FindNextTag(nReader, "readers",
                                               0, 0);
            } /* while */
          }
          if (!nReader) {
            DBG_INFO(0, "XML file \"%s\" contains no <readers> tag", fname);
          }
          else {
            int readers;
  
            readers=0;
            nReader=GWEN_XMLNode_FindFirstTag(nReader, "reader", 0, 0);
            while(nReader) {
              GWEN_DB_NODE *dbReader;
  
              dbReader=LC_DriverInfo_ReaderDbFromXml(nReader);
              if (dbReader) {
                GWEN_DB_AddGroup(dbDriver, dbReader);
                readers++;
              }
              nReader=GWEN_XMLNode_FindNextTag(nReader, "reader", 0, 0);
            } /* while */
            if (!readers) {
              DBG_INFO(0, "XML file \"%s\" contains no readers", fname);
            }
          }
          GWEN_DB_AddGroup(dbDrivers, dbDriver);
          driverAdded=1;
        }
        else {
          GWEN_DB_Group_free(dbDriver);
        }
      }
    }
  }
  GWEN_XMLNode_free(nFile);

  if (!driverAdded)
    return 1;

  return 0;
}