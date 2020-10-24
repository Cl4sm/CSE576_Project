int handleCard(LC_CARD *card, GWEN_DB_NODE *dbArgs) {
  int rv;
  LC_CLIENT_RESULT res;
  int v;
  int dobeep;

  v=GWEN_DB_GetIntValue(dbArgs, "verbosity", 0, 0);
  dobeep=GWEN_DB_GetIntValue(dbArgs, "beep", 0, 0);

  /* try to open as KVK */
  rv=LC_KVKCard_ExtendCard(card);
  if (rv) {
    if (dobeep)
      errorBeep();
    return rv;
  }

  if (v>0)
    fprintf(stderr, "Opening card as KVK card.\n");
  res=LC_Card_Open(card);
  if (res==LC_Client_ResultOk) {
    if (v>0)
      fprintf(stderr, "Card is a KVK card, handling it.\n");
    rv=handleKvkCard(card, dbArgs);

    /* close card */
    if (v>0)
      fprintf(stderr, "Closing card.\n");
    res=LC_Card_Close(card);
    if (res!=LC_Client_ResultOk) {
      showError(card, res, "CardClose");
      return RETURNVALUE_WORK;
    }
    if (v>1)
      fprintf(stderr, "Card closed.\n");

    return rv;
  }
  else {
    if (v>0)
      fprintf(stderr, "Card is not a KVK card.\n");
  }
  LC_KVKCard_UnextendCard(card);

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
    if (v>0)
      fprintf(stderr, "Card is a EGK card, handling it.\n");
    rv=handleEgkCard(card, dbArgs);

    /* close card */
    if (v>0)
      fprintf(stderr, "Closing card.\n");
    res=LC_Card_Close(card);
    if (res!=LC_Client_ResultOk) {
      showError(card, res, "CardClose");
      return RETURNVALUE_WORK;
    }

    if (v>1)
      fprintf(stderr, "Card closed.\n");
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