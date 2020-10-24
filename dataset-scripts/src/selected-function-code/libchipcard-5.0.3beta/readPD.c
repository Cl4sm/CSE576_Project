int readPD(LC_CARD *card, GWEN_DB_NODE *dbArgs) {
  int rv;
  LC_CLIENT_RESULT res;
  int v;
  int dobeep;

  v=GWEN_DB_GetIntValue(dbArgs, "verbosity", 0, 0);
  dobeep=GWEN_DB_GetIntValue(dbArgs, "beep", 0, 0);

  /* try to open as EGK */
  rv=LC_EgkCard_ExtendCard(card);
  if (rv) {
    if (dobeep)
      errorBeep();
    return rv;
  }

  if (v>0)
    fprintf(stderr, "Opening card as EGK card.\n");
  res=LC_Card_Open(card);
  if (res==LC_Client_ResultOk) {
    GWEN_BUFFER *tbuf;

    if (v>0)
      fprintf(stderr, "Card is a EGK card, handling it.\n");
    tbuf=GWEN_Buffer_new(0, 256, 0, 1);
    res=LC_EgkCard_ReadRawPd(card, tbuf);
    if (res!=LC_Client_ResultOk) {
      showError(card, res, "LC_EgkCard_ReadPd");
      GWEN_Buffer_free(tbuf);
      return RETURNVALUE_WORK;
    }

    /* close card */
    if (v>0)
      fprintf(stderr, "Closing card.\n");
    res=LC_Card_Close(card);
    if (res!=LC_Client_ResultOk) {
      showError(card, res, "CardClose");
      GWEN_Buffer_free(tbuf);
      return RETURNVALUE_WORK;
    }

    if (v>1)
      fprintf(stderr, "Card closed.\n");

    if (v>1)
      fprintf(stderr, "Writing data.\n");
    writeFile(stdout,
	      GWEN_Buffer_GetStart(tbuf),
	      GWEN_Buffer_GetUsedBytes(tbuf));

    GWEN_Buffer_free(tbuf);

    return rv;
  }
  else {
    if (v>0)
      fprintf(stderr, "Card is not a EGK card.\n");
    showError(card, res, "CardOpen");
  }

  /* not supported */
  if (dobeep)
    errorBeep();
  return RETURNVALUE_CARD_NOT_SUPP;
}