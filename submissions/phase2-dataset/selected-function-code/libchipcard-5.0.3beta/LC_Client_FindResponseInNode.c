GWEN_XMLNODE *LC_Client_FindResponseInNode(GWEN_XMLNODE *cmd,
                                           const char *typ) {
  GWEN_XMLNODE *rnode;
  GWEN_XMLNODE *n;
  const char *ltyp;

  DBG_DEBUG(0, "Searching for response type \"%s\"", typ);
  rnode=GWEN_XMLNode_FindNode(cmd,
                              GWEN_XMLNodeTypeTag,
                              "responses");
  if (!rnode) {
    DBG_DEBUG(0, "No <responses> tag in command definition");
    return 0;
  }

  /* first try exact match */
  n=GWEN_XMLNode_GetFirstTag(rnode);
  while(n) {
    ltyp=GWEN_XMLNode_GetProperty(n, "type", 0);
    if (ltyp) {
      if (strcasecmp(ltyp, typ)==0)
        return n;
    }
    n=GWEN_XMLNode_GetNextTag(n);
  } /* while */

  /* then try a response without any type */
  n=GWEN_XMLNode_GetFirstTag(rnode);
  while(n) {
    ltyp=GWEN_XMLNode_GetProperty(n, "type", 0);
    if (!ltyp)
      return n;
    n=GWEN_XMLNode_GetNextTag(n);
  } /* while */

  return 0;
}