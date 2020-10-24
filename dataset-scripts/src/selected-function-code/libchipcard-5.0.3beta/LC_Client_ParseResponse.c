int LC_Client_ParseResponse(LC_CLIENT *cl,
                            GWEN_XMLNODE *node,
                            GWEN_BUFFER *gbuf,
                            GWEN_DB_NODE *rspData){
  GWEN_DB_NODE *dbTmp;
  GWEN_XMLNODE *rnode;
  const char *p;

  assert(cl);

  GWEN_Buffer_Rewind(gbuf); /* just in case ... */

  p=GWEN_DB_GetCharValue(rspData, "result/type", 0, 0);
  if (!p) {
    DBG_ERROR(LC_LOGDOMAIN, "No result type given");
    return -1;
  }
  dbTmp=GWEN_DB_GetGroup(rspData,
                         GWEN_DB_FLAGS_DEFAULT |
                         GWEN_DB_FLAGS_OVERWRITE_GROUPS,
                         "response");
  if (!dbTmp) {
    DBG_ERROR(LC_LOGDOMAIN, "No matching response tag found");
    return -1;
  }

  rnode=LC_Client_FindResponse(cl, node, p);
  if (!rnode) {
    DBG_DEBUG(0, "Did not find response");
    if (GWEN_Buffer_GetUsedBytes(gbuf)) {
      GWEN_DB_SetBinValue(dbTmp,
                          GWEN_DB_FLAGS_OVERWRITE_VARS,
                          "data",
                          GWEN_Buffer_GetStart(gbuf),
                          GWEN_Buffer_GetUsedBytes(gbuf));
    }
  }
  else {
    if (GWEN_MsgEngine_ParseMessage(cl->msgEngine,
                                    rnode,
                                    gbuf,
                                    dbTmp,
                                    GWEN_MSGENGINE_READ_FLAGS_DEFAULT)){
      DBG_ERROR(LC_LOGDOMAIN, "Error parsing response");
      return -1;
    }
  }

  return 0;
}