GWEN_XMLNODE *LC_Client_FindCardCommand(LC_CLIENT *cl,
                                        LC_CARD *card,
                                        const char *commandName) {
  GWEN_XMLNODE *n;

  n=LC_Card_GetCardNode(card);
  if (n) {
    const char *cardName;
    GWEN_STRINGLIST *sl;

    cardName=GWEN_XMLNode_GetProperty(n, "name", 0);
    assert(cardName);
    DBG_INFO(LC_LOGDOMAIN, "Preselected card type \"%s\"", cardName);

    sl=GWEN_StringList_new();
    n=LC_Client_FindCommandInCardFamily(cl->cardNodes,
                                        sl,
                                        cardName,
                                        commandName,
                                        LC_Card_GetDriverType(card),
                                        LC_Card_GetReaderType(card));
    GWEN_StringList_free(sl);
    if (n)
      return n;
    return 0;
  }
  return LC_Client_FindCommandInCardTypes(cl->cardNodes,
                                          LC_Card_GetCardTypes(card),
                                          commandName,
                                          LC_Card_GetDriverType(card),
                                          LC_Card_GetReaderType(card));
}