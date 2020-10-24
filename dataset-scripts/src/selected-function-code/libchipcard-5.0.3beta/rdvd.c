int rdvd(LC_CLIENT *cl, GWEN_DB_NODE *dbArgs){
  LC_CARD *card=0;
  LC_CLIENT_RESULT res;
  int v;
  int i;
  uint32_t cardId;
  int dobeep;
  int rv=0;

  v=GWEN_DB_GetIntValue(dbArgs, "verbosity", 0, 0);
  cardId=GWEN_DB_GetIntValue(dbArgs, "cardId", 0, 0);
  dobeep=GWEN_DB_GetIntValue(dbArgs, "beep", 0, 0);

  if (v>1)
    fprintf(stderr, "Connecting to server.\n");
  res=LC_Client_Start(cl);
  if (res!=LC_Client_ResultOk) {
    showError(card, res, "StartWait");
    if (dobeep)
      errorBeep();
    return RETURNVALUE_WORK;
  }
  if (v>1)
    fprintf(stderr, "Connected.\n");

  for (i=0;;i++) {
    if (v>0)
      fprintf(stderr, "Waiting for card...\n");
    res=LC_Client_GetNextCard(cl, &card, 20);
    if (res!=LC_Client_ResultOk) {
      showError(card, res, "GetNextCard");
      if (dobeep)
	errorBeep();
      return RETURNVALUE_WORK;
    }
    if (v>0)
      fprintf(stderr, "Found a card.\n");

    rv=readVD(card, dbArgs);

    if (v>0)
      fprintf(stderr, "Releasing card.\n");
    res=LC_Client_ReleaseCard(cl, card);
    if (res!=LC_Client_ResultOk) {
      showError(card, res, "ReleaseCard");
      if (dobeep)
	errorBeep();
      LC_Card_free(card);
      return RETURNVALUE_WORK;
    }
    LC_Card_free(card);

#if 1
    break;
#else
    if (rv!=RETURNVALUE_CARD_NOT_SUPP)
      break;

    if (i>15) {
      fprintf(stderr, "ERROR: No card found.\n");
      if (dobeep)
	errorBeep();
      return RETURNVALUE_WORK;
    }
#endif
  } /* for */

  /* stop waiting */
  if (v>1)
    fprintf(stderr, "Telling the server that we need no more cards.\n");
  res=LC_Client_Stop(cl);
  if (res!=LC_Client_ResultOk) {
    showError(card, res, "Stop");
    if (dobeep)
      errorBeep();
    return RETURNVALUE_WORK;
  }

  /* finished */
  if (v>1)
    fprintf(stderr, "Finished.\n");
  return rv;
}