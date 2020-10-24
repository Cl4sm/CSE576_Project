GWEN_XMLNODE *LC_Client_FindCommandInCardFamily(GWEN_XMLNODE *cardNodes,
                                                GWEN_STRINGLIST *handled,
                                                const char *cardType,
                                                const char *commandName,
                                                const char *driverType,
                                                const char *readerType){
  GWEN_XMLNODE *node;

  DBG_DEBUG(LC_LOGDOMAIN, "Searching in family of \"%s\"", cardType);
  node=GWEN_XMLNode_FindFirstTag(cardNodes, "card", "name", cardType);
  if (node) {
    while(node) {
      GWEN_XMLNODE *n;
      const char *parent;

      cardType=GWEN_XMLNode_GetProperty(node, "name", 0);
      assert(cardType);
      DBG_VERBOUS(LC_LOGDOMAIN, "Searching in \"%s\" (%s/%s)",
                  GWEN_XMLNode_GetProperty(node, "name", "(noname)"),
                  driverType?driverType:"(none)",
                  readerType?readerType:"(none)");
      if (!GWEN_StringList_HasString(handled, cardType)) {
        n=LC_Client_FindCommandInCardNode(node, commandName,
                                          driverType, readerType);
        GWEN_StringList_AppendString(handled, cardType, 0, 1);
        if (n) {
          return n;
        }
      }
      else {
        DBG_INFO(LC_LOGDOMAIN, "Card type \"%s\" already handled",
                 cardType);
      }

      /* search in parents */
      parent=GWEN_XMLNode_GetProperty(node, "extends", 0);
      if (!parent) {
        DBG_VERBOUS(LC_LOGDOMAIN, "Card type \"%s\" has no parent",
                    GWEN_XMLNode_GetProperty(node, "name", "(noname)"));
        break;
      }
      DBG_DEBUG(LC_LOGDOMAIN, "Searching for extended card \"%s\"", parent);
      node=GWEN_XMLNode_FindFirstTag(cardNodes,
                                     "card",
                                     "name",
                                     parent);
      if (!node) {
        DBG_WARN(LC_LOGDOMAIN,
                 "Extended card \"%s\" not found",
                 parent);
        break;
      }
      DBG_DEBUG(LC_LOGDOMAIN, "Searching in parent \"%s\"", parent);
    } /* while */
  }
  else {
    DBG_INFO(LC_LOGDOMAIN, "Card \"%s\" not found", cardType);
  }
  DBG_DEBUG(0, "Command \"%s\" not found", commandName);
  return 0;
}