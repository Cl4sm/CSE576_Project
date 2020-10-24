int maxload(LC_CLIENT *cl, GWEN_DB_NODE *dbArgs){
  LC_CARD *card=0;
  LC_CLIENT_RESULT res;
  LC_GELDKARTE_VALUES *values;
  int rv;
  int v;

  v=GWEN_DB_GetIntValue(dbArgs, "verbosity", 0, 0);
  if (v>1)
    fprintf(stderr, "Connecting to server.\n");
  res=LC_Client_Start(cl);
  if (res!=LC_Client_ResultOk) {
    showError(card, res, "Start");
    return RETURNVALUE_WORK;
  }
  if (v>1)
    fprintf(stderr, "Connected.\n");

  if (v>0)
    fprintf(stderr, "Waiting for card...\n");
  res=LC_Client_GetNextCard(cl, &card, 20);
  if (res!=LC_Client_ResultOk) {
    showError(card, res, "GetNextCard");
    return RETURNVALUE_WORK;
  }
  if (v>0)
    fprintf(stderr, "Found a card.\n");

  if (LC_GeldKarte_ExtendCard(card)) {
    fprintf(stderr, "ERROR: Could not extend card as GELDKARTE card\n");
    return RETURNVALUE_WORK;
  }

  if (v>0)
    fprintf(stderr, "Opening card.\n");
  res=LC_Card_Open(card);
  if (res!=LC_Client_ResultOk) {
    fprintf(stderr,
            "ERROR: Error executing command CardOpen (%d).\n",
            res);
    return RETURNVALUE_WORK;
  }
  if (v>0)
    fprintf(stderr, "Card is a GELDKARTE card as expected.\n");

  if (v>1)
    fprintf(stderr, "Telling the server that we need no more cards.\n");
  res=LC_Client_Stop(cl);
  if (res!=LC_Client_ResultOk) {
    showError(card, res, "Stop");
    return RETURNVALUE_WORK;
  }

  values=LC_GeldKarte_Values_new();
  res=LC_GeldKarte_ReadValues(card, values);
  if (res!=LC_Client_ResultOk) {
    showError(card, res, "ReadValues");
    rv=RETURNVALUE_WORK;
  }
  else
    rv=0;

  if (v>0)
    fprintf(stderr, "Closing card.\n");
  res=LC_Card_Close(card);
  if (res!=LC_Client_ResultOk) {
    showError(card, res, "CardClose");
    return RETURNVALUE_WORK;
  }
  if (v>0)
    fprintf(stderr, "Card closed.\n");

  if (v>0)
    fprintf(stderr, "Releasing card.\n");
  res=LC_Client_ReleaseCard(cl, card);
  if (res!=LC_Client_ResultOk) {
    showError(card, res, "ReleaseCard");
    rv=RETURNVALUE_WORK;
  }
  else
    rv=0;
  LC_Card_free(card);

  if (rv==0) {
    fprintf(stdout,
            I18N("Card can be loaded with %6.2f %s\n"),
            LC_GeldKarte_Values_GetMaxLoad(values)/100.0,
            "EUR");
  }
  return rv;
}