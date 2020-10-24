int b_logs(LC_CLIENT *cl, GWEN_DB_NODE *dbArgs){
  LC_CARD *card=0;
  LC_CLIENT_RESULT res;
  LC_GELDKARTE_BLOG_LIST2 *bll;
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


  bll=LC_GeldKarte_BLog_List2_new();
  res=LC_GeldKarte_ReadBLogs(card, bll);
  if (res!=LC_Client_ResultOk) {
    showError(card, res, "ReadBLogs");
    return RETURNVALUE_WORK;
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
    LC_GELDKARTE_BLOG_LIST2_ITERATOR *blli;

    blli=LC_GeldKarte_BLog_List2_First(bll);
    if (blli) {
      LC_GELDKARTE_BLOG *bl;

      bl=LC_GeldKarte_BLog_List2Iterator_Data(blli);
      assert(bl);
      fprintf(stdout,
	      "Status bSEQ lSEQ hSEQ sSEQ KeyId Merchant     Value   Loaded \n");
      while(bl) {

	fprintf(stdout,
		"%6d %4d %4d %4d %4d %4d %12s %7.2f %7.2f\n",
		LC_GeldKarte_BLog_GetStatus(bl),
		LC_GeldKarte_BLog_GetBSeq(bl),
		LC_GeldKarte_BLog_GetLSeq(bl),
		LC_GeldKarte_BLog_GetHSeq(bl),
		LC_GeldKarte_BLog_GetSSeq(bl),
		LC_GeldKarte_BLog_GetKeyId(bl),
		LC_GeldKarte_BLog_GetMerchantId(bl),
		(float)(LC_GeldKarte_BLog_GetValue(bl)/100.0),
		(float)(LC_GeldKarte_BLog_GetLoaded(bl)/100.0));

	bl=LC_GeldKarte_BLog_List2Iterator_Next(blli);
      }
      LC_GeldKarte_BLog_List2Iterator_free(blli);
    }
    else {
      fprintf(stdout, I18N("No BLogs on the card\n"));
    }
  }
  return rv;
}