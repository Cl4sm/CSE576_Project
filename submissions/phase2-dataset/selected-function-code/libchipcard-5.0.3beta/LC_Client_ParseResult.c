int LC_Client_ParseResult(LC_CLIENT *cl,
                          GWEN_XMLNODE *node,
                          GWEN_BUFFER *gbuf,
                          GWEN_DB_NODE *rspData){
  unsigned int i;
  int sw1, sw2;
  GWEN_DB_NODE *dbTmp;
  GWEN_XMLNODE *rnode;

  GWEN_Buffer_Rewind(gbuf); /* just in case ... */
  i=GWEN_Buffer_GetUsedBytes(gbuf);
  if (i<2) {
    DBG_ERROR(LC_LOGDOMAIN, "Answer too small (less than 2 bytes)");
    return -1;
  }
  sw1=(unsigned char)(GWEN_Buffer_GetStart(gbuf)[i-2]);
  sw2=(unsigned char)(GWEN_Buffer_GetStart(gbuf)[i-1]);
  GWEN_Buffer_Crop(gbuf, 0, i-2);
  /* store result */
  dbTmp=GWEN_DB_GetGroup(rspData,
                         GWEN_DB_FLAGS_DEFAULT |
                         GWEN_DB_FLAGS_OVERWRITE_GROUPS,
                         "result");
  assert(dbTmp);
  GWEN_DB_SetIntValue(dbTmp, GWEN_DB_FLAGS_DEFAULT,
                      "sw1", sw1);
  GWEN_DB_SetIntValue(dbTmp, GWEN_DB_FLAGS_DEFAULT,
                      "sw2", sw2);

  rnode=LC_Client_FindResult(cl, node, sw1, sw2);
  if (rnode) {
    const char *t;
    GWEN_XMLNODE *tnode;
    GWEN_BUFFER *txtbuf;
    int first;

    t=GWEN_XMLNode_GetProperty(rnode, "type", "success");
    DBG_INFO(0, "Result is: %s", t);
    GWEN_DB_SetCharValue(dbTmp,
                         GWEN_DB_FLAGS_DEFAULT,
                         "type", t);
    /* get text */
    txtbuf=GWEN_Buffer_new(0, 256, 0, 1);
    first=1;
    tnode=GWEN_XMLNode_GetFirstData(rnode);
    while(tnode) {
      const char *p;

      p=GWEN_XMLNode_GetData(tnode);
      if (p) {
        if (!first)
          GWEN_Buffer_AppendByte(txtbuf, ' ');
        GWEN_Buffer_AppendString(txtbuf, p);
      }
      if (first)
        first=0;
      tnode=GWEN_XMLNode_GetNextData(tnode);
    } /* while */

    if (GWEN_Buffer_GetUsedBytes(txtbuf))
      GWEN_DB_SetCharValue(dbTmp, GWEN_DB_FLAGS_DEFAULT, "text",
                           GWEN_Buffer_GetStart(txtbuf));
    GWEN_Buffer_free(txtbuf);
  }
  else {
    DBG_ERROR(LC_LOGDOMAIN,
              "Result for %02x/%02x not found, assuming error",
              sw1, sw2);
    GWEN_DB_SetCharValue(dbTmp,
                         GWEN_DB_FLAGS_DEFAULT,
                         "type", "error");
    GWEN_DB_SetCharValue(dbTmp,
                         GWEN_DB_FLAGS_DEFAULT,
                         "text", "Result not found");
  }

  return 0;
}