LC_CLIENT_RESULT LC_Client_BuildApdu(LC_CLIENT *cl,
                                     LC_CARD *card,
                                     const char *command,
                                     GWEN_DB_NODE *cmdData,
                                     GWEN_BUFFER *buf) {
  GWEN_XMLNODE *node;
  LC_CLIENT_RESULT res;

  DBG_INFO(LC_LOGDOMAIN, "Building APDU for command \"%s\"", command);
  /* lookup card command */
  DBG_INFO(LC_LOGDOMAIN, "- looking up command");
  node=LC_Card_FindCommand(card, command);
  if (!node) {
    DBG_INFO(LC_LOGDOMAIN, "Command \"%s\" not found",
             command);
    return LC_Client_ResultNotFound;
  }

  /* build APDU */
  DBG_INFO(LC_LOGDOMAIN, "- building APDU");
  res=LC_Client__BuildApdu(cl, node, cmdData, buf);
  if (res!=LC_Client_ResultOk) {
    DBG_INFO(LC_LOGDOMAIN,
	     "Error building APDU for command \"%s\" (%d)",
	     command, res);
    return res;
  }

  return LC_Client_ResultOk;
}