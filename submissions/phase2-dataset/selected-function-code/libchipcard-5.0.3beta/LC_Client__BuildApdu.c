LC_CLIENT_RESULT LC_Client__BuildApdu(LC_CLIENT *cl,
				      GWEN_XMLNODE *node,
				      GWEN_DB_NODE *cmdData,
				      GWEN_BUFFER *gbuf) {
  GWEN_XMLNODE *sendNode;
  GWEN_XMLNODE *dataNode;
  GWEN_XMLNODE *apduNode;
  GWEN_BUFFER *dataBuffer;
  unsigned int i;
  int j;

  assert(cl);

  sendNode=GWEN_XMLNode_FindNode(node, GWEN_XMLNodeTypeTag, "send");
  if (!sendNode) {
    DBG_INFO(LC_LOGDOMAIN,
	     "No <send> tag in command definition, do not execute");
    return LC_Client_ResultDontExecute;
  }

  apduNode=GWEN_XMLNode_FindNode(sendNode,
                                 GWEN_XMLNodeTypeTag, "apdu");
  if (!apduNode) {
    DBG_ERROR(LC_LOGDOMAIN, "No <apdu> tag in command definition");
    abort();
  }

  dataBuffer=GWEN_Buffer_new(0, 256, 0, 1);
  dataNode=GWEN_XMLNode_FindNode(sendNode,
                                 GWEN_XMLNodeTypeTag, "data");
  if (dataNode) {
    /* there is a data node, sample data */
    if (GWEN_MsgEngine_CreateMessageFromNode(cl->msgEngine,
                                             dataNode,
                                             dataBuffer,
                                             cmdData)) {
      DBG_ERROR(LC_LOGDOMAIN, "Error creating data for APDU");
      GWEN_Buffer_free(dataBuffer);
      GWEN_Buffer_AppendString(gbuf, "Error creating APDU data from command");
      return -1;
    }
  }

  if (GWEN_MsgEngine_CreateMessageFromNode(cl->msgEngine,
                                           apduNode,
                                           gbuf,
                                           cmdData)) {
    DBG_ERROR(LC_LOGDOMAIN, "Error creating APDU");
    GWEN_Buffer_free(dataBuffer);
    GWEN_Buffer_AppendString(gbuf, "Error creating APDU from command");
    return -1;
  }

  i=GWEN_Buffer_GetUsedBytes(dataBuffer);
  if (i) {
    GWEN_Buffer_AppendByte(gbuf, (unsigned char)i);
    GWEN_Buffer_AppendBuffer(gbuf, dataBuffer);
  }
  GWEN_Buffer_free(dataBuffer);

  j=0;
  if (1!=sscanf(GWEN_XMLNode_GetProperty(apduNode, "lr", "0"),
                "%i", &j))
    j=0;

  if (j!=-1) {
    j=GWEN_DB_GetIntValue(cmdData, "lr", 0, -1);
    if (j==-1) {
      if (1!=sscanf(GWEN_XMLNode_GetProperty(apduNode, "lr", "-1"),
                    "%i", &j))
        j=-1;
    }
  }
  if (j>=0)
    GWEN_Buffer_AppendByte(gbuf, (unsigned char)j);

  return 0;
}