int LC_Client_AddCardTypesByAtr(LC_CLIENT *cl, LC_CARD *card){
  GWEN_XMLNODE *cardNode;
  const unsigned char *atr;
  unsigned int atrLen;
  GWEN_BUFFER *hexAtr;
  int types=0;
  int done;

  DBG_DEBUG(0, "Adding card types...");

  /* get ATR, convert it to hex */
  atrLen=LC_Card_GetAtr(card, &atr);
  if (atr==0 || atrLen==0) {
    DBG_INFO(0, "No ATR");
    return 1;
  }
  hexAtr=GWEN_Buffer_new(0, 256, 0, 1);
  if (GWEN_Text_ToHexBuffer((const char*)atr, atrLen, hexAtr, 0, 0, 0)) {
    DBG_ERROR(LC_LOGDOMAIN, "Internal error");
    abort();
  }

  cardNode=GWEN_XMLNode_FindFirstTag(cl->cardNodes, "card", 0, 0);
  if (!cardNode) {
    DBG_ERROR(LC_LOGDOMAIN, "No card nodes.");
    return -1;
  }
  while(cardNode) {
    const char *name;
    const char *tp;
    const char *xtp;
    int sameBaseType=0;

    name=GWEN_XMLNode_GetProperty(cardNode, "name", 0);
    assert(name);
    tp=GWEN_XMLNode_GetProperty(cardNode, "type", 0);

    DBG_VERBOUS(LC_LOGDOMAIN, "Checking card \"%s\"", name);
    xtp=LC_Card_GetCardType(card);
    sameBaseType=(tp && xtp && strcasecmp(tp, xtp)==0);
    if (sameBaseType) {
      GWEN_XMLNODE *nAtrs;

      nAtrs=GWEN_XMLNode_FindFirstTag(cardNode, "cardinfo", 0, 0);
      if (nAtrs)
        nAtrs=GWEN_XMLNode_FindFirstTag(nAtrs, "atrs", 0, 0);
      if (nAtrs) {
        GWEN_XMLNODE *nAtr;
  
        nAtr=GWEN_XMLNode_GetFirstTag(nAtrs);
        while(nAtr) {
          GWEN_XMLNODE *nData;
  
          nData=GWEN_XMLNode_GetFirstData(nAtr);
          if (nData) {
            const char *p;
  
            p=GWEN_XMLNode_GetData(nData);
            if (p) {
              GWEN_BUFFER *dbuf;

              /* compress ATR from XML file */
              dbuf=GWEN_Buffer_new(0, 256, 0, 1);
              while(*p) {
                if (!isspace(*p))
                  GWEN_Buffer_AppendByte(dbuf, *p);
                p++;
              } /* while */
              if (-1!=GWEN_Text_ComparePattern(GWEN_Buffer_GetStart(hexAtr),
                                               GWEN_Buffer_GetStart(dbuf),
                                               0)) {
                DBG_DEBUG(LC_LOGDOMAIN, "Card \"%s\" matches ATR", name);
                if (LC_Card_AddCardType(card, name)) {
                  DBG_INFO(LC_LOGDOMAIN, "Added card type \"%s\"", name);
                  types++;
                }
              }
              GWEN_Buffer_free(dbuf);
            } /* if data */
          } /* if data node */
          nAtr=GWEN_XMLNode_GetNextTag(nAtr);
        } /* while */
      } /* if atrs */
    } /* if sameBaseType */
    cardNode=GWEN_XMLNode_FindNextTag(cardNode, "card", 0, 0);
  } /* while */
  GWEN_Buffer_free(hexAtr);

  /* add all cards whose base types are contained in the list.
   * repeat this as long as we added cards */
  done=0;
  while(!done) {
    done=1;
    cardNode=GWEN_XMLNode_FindFirstTag(cl->cardNodes, "card", 0, 0);
    while(cardNode) {
      const char *name;
      const char *extends;

      name=GWEN_XMLNode_GetProperty(cardNode, "name", 0);
      assert(name);
      extends=GWEN_XMLNode_GetProperty(cardNode, "extends", 0);
      if (extends) {
        if (GWEN_StringList_HasString(LC_Card_GetCardTypes(card), extends)) {
          if (LC_Card_AddCardType(card, name)) {
            DBG_INFO(LC_LOGDOMAIN, "Added card type \"%s\"", name);
            types++;
            done=0;
          }
        }
      }
      cardNode=GWEN_XMLNode_FindNextTag(cardNode, "card", 0, 0);
    }
  } /* while */

  return (types!=0)?0:1;
}