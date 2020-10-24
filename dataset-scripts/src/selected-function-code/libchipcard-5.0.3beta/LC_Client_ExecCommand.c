LC_CLIENT_RESULT LC_Client_ExecCommand(LC_CLIENT *cl,
                                       LC_CARD *card,
                                       const char *commandName,
                                       GWEN_DB_NODE *cmdData,
                                       GWEN_DB_NODE *rspData) {
  GWEN_XMLNODE *node;
  GWEN_BUFFER *buf;
  GWEN_BUFFER *rbuf;
  LC_CLIENT_RESULT res;
  LC_CLIENT_CMDTARGET t;
  const char *s;

  DBG_INFO(LC_LOGDOMAIN, "Executing command \"%s\"", commandName);

  /* lookup card command */
  DBG_INFO(LC_LOGDOMAIN, "- looking up command");
  node=LC_Card_FindCommand(card, commandName);
  if (!node) {
    DBG_INFO(LC_LOGDOMAIN, "Command \"%s\" not found",
             commandName);
    return LC_Client_ResultNotFound;
  }

  /* determine target of the command */
  DBG_INFO(LC_LOGDOMAIN, "- determining target");
  t=LC_Client_CmdTargetCard;
  s=GWEN_XMLNode_GetProperty(node, "target", "card");
  if (s) {
    if (strcasecmp(s, "card")==0)
      t=LC_Client_CmdTargetCard;
    else if (strcasecmp(s, "reader")==0)
      t=LC_Client_CmdTargetReader;
    else {
      DBG_ERROR(LC_LOGDOMAIN,
                "Invalid target given in command \"%s\": %s",
                commandName, s);
      return LC_Client_ResultCfgError;
    }
  }

  /* build APDU */
  DBG_INFO(LC_LOGDOMAIN, "- building APDU");
  buf=GWEN_Buffer_new(0, 256, 0, 1);
  res=LC_Client__BuildApdu(cl, node, cmdData, buf);
  if (res!=LC_Client_ResultOk) {
    DBG_INFO(LC_LOGDOMAIN,
	     "Error building APDU for command \"%s\" (%d)",
	     commandName, res);
    GWEN_Buffer_free(buf);
    return res;
  }

  /* send APDU */
  DBG_INFO(LC_LOGDOMAIN, "- sending APDU, waiting for answer");
  rbuf=GWEN_Buffer_new(0, 256, 0, 1);
  res=LC_Card_ExecApdu(card,
                       GWEN_Buffer_GetStart(buf),
                       GWEN_Buffer_GetUsedBytes(buf),
                       rbuf,
                       t);
  if (res!=LC_Client_ResultOk) {
    DBG_INFO(LC_LOGDOMAIN, "here (%d)", res);
    GWEN_Buffer_free(rbuf);
    GWEN_Buffer_free(buf);
    return res;
  }
  GWEN_Buffer_free(buf);

  /* parse answer */
  DBG_INFO(LC_LOGDOMAIN, "- parsing response");
  if (LC_Client_ParseAnswer(cl, node, rbuf, rspData)) {
    DBG_INFO(LC_LOGDOMAIN, "Error parsing answer");
    GWEN_Buffer_free(rbuf);
    return LC_Client_ResultCmdError;
  }

  /* store response data */
  if (GWEN_Buffer_GetUsedBytes(rbuf)) {
    GWEN_DB_SetBinValue(rspData, GWEN_DB_FLAGS_OVERWRITE_VARS,
                        "data",
                        GWEN_Buffer_GetStart(rbuf),
                        GWEN_Buffer_GetUsedBytes(rbuf));
  }
  GWEN_Buffer_free(rbuf);

  /* check for error result of command */
  s=GWEN_DB_GetCharValue(rspData, "result/type", 0, "error");
  if (strcasecmp(s, "success")!=0) {
    DBG_INFO(LC_LOGDOMAIN, "Command execution error flagged by card (%s)",
             s?s:"(none)");
    return LC_Client_ResultCmdError;
  }

  /* done */
  return LC_Client_ResultOk;
}