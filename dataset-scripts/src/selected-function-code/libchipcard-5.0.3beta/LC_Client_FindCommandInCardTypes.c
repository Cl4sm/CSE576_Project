GWEN_XMLNODE*
LC_Client_FindCommandInCardTypes(GWEN_XMLNODE *cardNodes,
                                 const GWEN_STRINGLIST *cardTypes,
                                 const char *commandName,
                                 const char *driverType,
                                 const char *readerType){
  GWEN_STRINGLIST *handled;
  GWEN_STRINGLISTENTRY *se;
  GWEN_XMLNODE *node=0;

  handled=GWEN_StringList_new();
  se=GWEN_StringList_FirstEntry(cardTypes);
  while(se) {
    const char *s;

    s=GWEN_StringListEntry_Data(se);
    assert(s);

    DBG_INFO(LC_LOGDOMAIN,
             "Searching in card type \"%s\"", s);
    node=LC_Client_FindCommandInCardFamily(cardNodes,
                                           handled,
                                           s,
                                           commandName,
                                           driverType,
                                           readerType);
    if (node)
      break;
    se=GWEN_StringListEntry_Next(se);
  } /* while */

  return node;
}