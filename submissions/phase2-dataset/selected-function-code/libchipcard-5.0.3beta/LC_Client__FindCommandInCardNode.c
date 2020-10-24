GWEN_XMLNODE *LC_Client__FindCommandInCardNode(GWEN_XMLNODE *node,
                                               const char *commandName,
                                               const char *driverType,
                                               const char *readerType){
  GWEN_XMLNODE *cmds;
  GWEN_XMLNODE *n;

  DBG_INFO(LC_LOGDOMAIN,
           "Searching in \"%s\" (%s/%s)",
           GWEN_XMLNode_GetProperty(node, "name", "(noname)"),
           driverType?driverType:"(none)",
           readerType?readerType:"(none)");

  cmds=GWEN_XMLNode_FindNode(node,
                             GWEN_XMLNodeTypeTag,
                             "commands");
  if (!cmds) {
    DBG_INFO(LC_LOGDOMAIN, "No commands in card data");
    return 0;
  }

  /* first try exact match */
  if (driverType && readerType) {
    DBG_DEBUG(LC_LOGDOMAIN, "Searching for %s/%s/%s",
              driverType, readerType, commandName);
    n=GWEN_XMLNode_FindFirstTag(cmds,
                                "command",
                                "name",
                                commandName);
    while(n) {
      if (strcasecmp(GWEN_XMLNode_GetProperty(n, "driver", ""),
                     driverType)==0 &&
          strcasecmp(GWEN_XMLNode_GetProperty(n, "reader", ""),
                     readerType)==0) {
        DBG_DEBUG(LC_LOGDOMAIN,
                  "Found command in %s/%s", driverType, readerType);
        return n;
      }
      n=GWEN_XMLNode_FindNextTag(n, "command", "name", commandName);
    } /* while */
  }

  if (driverType) {
    /* try match of driver only */
    DBG_DEBUG(LC_LOGDOMAIN, "Searching for %s/%s",
              driverType, commandName);
    n=GWEN_XMLNode_FindFirstTag(cmds,
                                "command",
                                "name",
                                commandName);
    while(n) {
      if (strcasecmp(GWEN_XMLNode_GetProperty(n, "driver", ""),
                     driverType)==0) {
        DBG_DEBUG(LC_LOGDOMAIN, "Found command in %s", driverType);
        return n;
      }
      n=GWEN_XMLNode_FindNextTag(n, "command", "name", commandName);
    } /* while */
  }

  /* try match of command name only */
  DBG_DEBUG(LC_LOGDOMAIN, "Searching for %s", commandName);
  n=GWEN_XMLNode_FindFirstTag(cmds,
                              "command",
                              "name",
                              commandName);
  while(n) {
    if (!GWEN_XMLNode_GetProperty(n, "driver", 0))
      return n;
    n=GWEN_XMLNode_FindNextTag(n, "command", "name", commandName);
  } /* while */

  return n;
}