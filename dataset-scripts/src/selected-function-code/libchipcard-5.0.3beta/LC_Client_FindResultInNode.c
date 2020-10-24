GWEN_XMLNODE *LC_Client_FindResultInNode(GWEN_XMLNODE *node,
                                         int sw1, int sw2) {
  GWEN_XMLNODE *rnode;
  GWEN_XMLNODE *n;
  int lsw1, lsw2;

  DBG_DEBUG(0, "Searching for result type of %02x/%02x", sw1, sw2);
  while(node) {
    rnode=GWEN_XMLNode_FindNode(node,
                                GWEN_XMLNodeTypeTag,
                                "results");
    if (rnode) {
      /* first try exact match */
      n=GWEN_XMLNode_GetFirstTag(rnode);
      while(n) {
        if (1==sscanf(GWEN_XMLNode_GetProperty(n, "sw1", "-1"),
                      "%i", &lsw1) &&
            1==sscanf(GWEN_XMLNode_GetProperty(n, "sw2", "-1"),
                      "%i", &lsw2)) {
          DBG_VERBOUS(0, "Checking %02x/%02x", lsw1, lsw2);
          if (lsw1==sw1 && lsw2==sw2) {
            return n;
          }
        }
        else {
          DBG_WARN(0, "Bad SW1 or SW2 value");
        }
        n=GWEN_XMLNode_GetNextTag(n);
      } /* while */

      /* try SW1 only */
      n=GWEN_XMLNode_GetFirstTag(rnode);
      while(n) {
        if (1==sscanf(GWEN_XMLNode_GetProperty(n, "sw1", "-1"),
                      "%i", &lsw1) &&
            1==sscanf(GWEN_XMLNode_GetProperty(n, "sw2", "-1"),
                      "%i", &lsw2)) {
          if (lsw1==sw1 && lsw2==-1) {
            return n;
          }
        }
        else {
          DBG_WARN(0, "Bad SW1 or SW2 value");
        }
        n=GWEN_XMLNode_GetNextTag(n);
      } /* while */
    } /* if rnode */

    /* select parent */
    node=GWEN_XMLNode_GetParent(node);
  }

  return 0;
}