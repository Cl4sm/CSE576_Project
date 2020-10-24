int LC_Client_MergeXMLDefs(GWEN_XMLNODE *destNode,
                           GWEN_XMLNODE *node) {
  GWEN_XMLNODE *nsrc, *ndst;

  assert(node);

  nsrc=GWEN_XMLNode_GetChild(node);
  while(nsrc) {
    /* merge 1st level */
    if (GWEN_XMLNode_GetType(nsrc)==GWEN_XMLNodeTypeTag) {
      ndst=GWEN_XMLNode_FindFirstTag(destNode,
                                     GWEN_XMLNode_GetData(nsrc),
                                     "name",
                                     GWEN_XMLNode_GetProperty(nsrc,
                                                              "name",
                                                              ""));
      if (ndst) {
        GWEN_XMLNODE *nsrc2, *ndst2;

        /* merge 2nd level */
        DBG_VERBOUS(LC_LOGDOMAIN, "Merging tags from \"%s\" into \"%s\"",
                    GWEN_XMLNode_GetData(nsrc),
                    GWEN_XMLNode_GetData(ndst));
        nsrc2=GWEN_XMLNode_GetChild(nsrc);
        while(nsrc2) {
          if (GWEN_XMLNode_GetType(nsrc2)==GWEN_XMLNodeTypeTag) {
            ndst2=GWEN_XMLNode_FindNode(ndst,
                                        GWEN_XMLNodeTypeTag,
                                        GWEN_XMLNode_GetData(nsrc2));
            if (ndst2) {
              GWEN_XMLNODE *n;

              DBG_VERBOUS(LC_LOGDOMAIN,
                          "Level2: Merging tags from "
                          "\"%s\" into \"%s\"",
                          GWEN_XMLNode_GetData(nsrc2),
                          GWEN_XMLNode_GetData(ndst2));
              /* node found, copy branch */
              n=GWEN_XMLNode_GetChild(nsrc2);
              while (n) {
                GWEN_XMLNODE *newNode;

                DBG_VERBOUS(LC_LOGDOMAIN, "Adding node \"%s\"",
                            GWEN_XMLNode_GetData(n));
                newNode=GWEN_XMLNode_dup(n);
                GWEN_XMLNode_AddChild(ndst2, newNode);
                n=GWEN_XMLNode_Next(n);
              } /* while n */
            }
            else {
              GWEN_XMLNODE *newNode;

              DBG_VERBOUS(LC_LOGDOMAIN, "Adding branch \"%s\"",
                          GWEN_XMLNode_GetData(nsrc2));
              newNode=GWEN_XMLNode_dup(nsrc2);
              GWEN_XMLNode_AddChild(ndst, newNode);
            }
          } /* if TAG */
          nsrc2=GWEN_XMLNode_Next(nsrc2);
        } /* while there are 2nd level source tags */
      }
      else {
	GWEN_XMLNODE *newNode;

        DBG_VERBOUS(LC_LOGDOMAIN, "Adding branch \"%s\"",
                    GWEN_XMLNode_GetData(nsrc));
        newNode=GWEN_XMLNode_dup(nsrc);
        GWEN_XMLNode_AddChild(destNode, newNode);
      }
    } /* if TAG */
    nsrc=GWEN_XMLNode_Next(nsrc);
  } /* while */

  return 0;
}