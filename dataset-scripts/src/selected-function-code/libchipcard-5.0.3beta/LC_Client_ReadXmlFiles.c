int LC_Client_ReadXmlFiles(GWEN_XMLNODE *root,
			   const char *basedir,
                           const char *tPlural,
                           const char *tSingular) {
  GWEN_STRINGLIST *sl;
  GWEN_STRINGLISTENTRY *se;
  GWEN_BUFFER *buf;
  int filesLoaded=0;

  /* prepare path */
  sl=GWEN_StringList_new();
  buf=GWEN_Buffer_new(0, 256, 0, 1);
  GWEN_Buffer_AppendString(buf, basedir);
  GWEN_Buffer_AppendString(buf, DIRSEP);
  GWEN_Buffer_AppendString(buf, tPlural);

  DBG_DEBUG(0, "Reading XML file (%s) from here: %s",
            tPlural,
            GWEN_Buffer_GetStart(buf));

  /* sample all XML files from that path */
  LC_Client__SampleXmlFiles(GWEN_Buffer_GetStart(buf), sl);

  /* load all files from the list */
  se=GWEN_StringList_FirstEntry(sl);
  while(se) {
    GWEN_XMLNODE *n;
  
    n=GWEN_XMLNode_new(GWEN_XMLNodeTypeTag, tSingular);
    if (GWEN_XML_ReadFile(n,
                          GWEN_StringListEntry_Data(se),
                          GWEN_XML_FLAGS_DEFAULT)) {
      DBG_ERROR(LC_LOGDOMAIN, "Could not read XML file \"%s\"",
                GWEN_StringListEntry_Data(se));
    }
    else {
      GWEN_XMLNODE *nn;
  
      nn=GWEN_XMLNode_FindNode(n, GWEN_XMLNodeTypeTag, tPlural);
      if (!nn) {
        DBG_WARN(LC_LOGDOMAIN, "File \"%s\" does not contain <%s>",
                 GWEN_StringListEntry_Data(se), tPlural);
      }
      else {
        if (LC_Client_MergeXMLDefs(root, nn)) {
          DBG_ERROR(LC_LOGDOMAIN, "Could not merge file \"%s\"",
                    GWEN_StringListEntry_Data(se));
        }
        else {
          filesLoaded++;
        }
      }
    }
    GWEN_XMLNode_free(n);

    se=GWEN_StringListEntry_Next(se);
  }

  /* cleanup */
  GWEN_StringList_free(sl);

  /* done */
  if (filesLoaded==0) {
    DBG_ERROR(LC_LOGDOMAIN, "No %s files loaded", tSingular);
    return -1;
  }

  return 0;
}