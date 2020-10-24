GWEN_XMLNODE *LC_Client_FindResult(LC_CLIENT *cl,
                                   GWEN_XMLNODE *cmdNode,
                                   int sw1, int sw2) {
  GWEN_XMLNODE *tmpNode;
  GWEN_XMLNODE *rnode;

  /* first find result in command node */
  rnode=LC_Client_FindResultInNode(cmdNode, sw1, sw2);
  if (rnode)
    return rnode;
  rnode=LC_Client_FindResultInNode(cmdNode, -1, -1);
  if (rnode)
    return rnode;

  /* try in node <commands> */
  tmpNode=GWEN_XMLNode_GetParent(cmdNode);
  if (!tmpNode)
    return 0;
  rnode=LC_Client_FindResultInNode(tmpNode, sw1, sw2);
  if (rnode)
    return rnode;
  rnode=LC_Client_FindResultInNode(tmpNode, -1, -1);
  if (rnode)
    return rnode;

  /* try in current card node */
  tmpNode=GWEN_XMLNode_GetParent(tmpNode);
  if (!tmpNode)
    return 0;
  rnode=LC_Client_FindResultInNode(tmpNode, sw1, sw2);
  if (rnode)
    return rnode;
  rnode=LC_Client_FindResultInNode(tmpNode, -1, -1);
  if (rnode)
    return rnode;

  /* try in parents */
  for(;;) {
    const char *parent;

    parent=GWEN_XMLNode_GetProperty(tmpNode, "extends", 0);
    if (!parent) {
      break;
    }
    tmpNode=GWEN_XMLNode_FindFirstTag(cl->cardNodes,
                                      "card",
                                      "name",
                                      parent);
    if (!tmpNode)
      break;

    rnode=LC_Client_FindResultInNode(tmpNode, sw1, sw2);
    if (rnode) {
      break;
    }
    rnode=LC_Client_FindResultInNode(tmpNode, -1, -1);
    if (rnode) {
      break;
    }
  } /* for */

  return rnode;
}