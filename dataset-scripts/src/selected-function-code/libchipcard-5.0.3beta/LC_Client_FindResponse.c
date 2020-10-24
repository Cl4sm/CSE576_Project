GWEN_XMLNODE *LC_Client_FindResponse(LC_CLIENT *cl,
                                     GWEN_XMLNODE *cmdNode,
                                     const char *typ) {
  GWEN_XMLNODE *tmpNode;
  GWEN_XMLNODE *rnode;

  /* first find response in command node */
  rnode=LC_Client_FindResponseInNode(cmdNode, typ);
  if (rnode)
    return rnode;

  /* try in node <commands> */
  tmpNode=GWEN_XMLNode_GetParent(cmdNode);
  if (!tmpNode)
    return 0;
  rnode=LC_Client_FindResponseInNode(tmpNode, typ);
  if (rnode)
    return rnode;

  /* try in current card node */
  tmpNode=GWEN_XMLNode_GetParent(tmpNode);
  if (!tmpNode)
    return 0;
  rnode=LC_Client_FindResponseInNode(tmpNode, typ);
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

    rnode=LC_Client_FindResponseInNode(tmpNode, typ);
    if (rnode) {
      break;
    }
  } /* for */

  return rnode;
}