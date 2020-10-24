int handleKvkCard(LC_CARD *card, GWEN_DB_NODE *dbArgs) {
  int v;
  const char *fname;
  int dobeep;
  int dosMode;
  LC_CLIENT_RESULT res;
  LC_HI_PERSONAL_DATA *pdata=NULL;
  LC_HI_INSURANCE_DATA *idata=NULL;

  v=GWEN_DB_GetIntValue(dbArgs, "verbosity", 0, 0);
  dobeep=GWEN_DB_GetIntValue(dbArgs, "beep", 0, 0);
  dosMode=GWEN_DB_GetIntValue(dbArgs, "dosMode", 0, 0);

  res=LC_KvkCard_ReadCardData(card, &pdata, &idata);
  if (res!=LC_Client_ResultOk) {
    showError(card, res, "LC_KvkCard_ReadCardData");
    if (dobeep)
      errorBeep();
    return RETURNVALUE_WORK;
  }

  /* open file */
  if (v>0)
    fprintf(stderr, "Writing data to file\n");
  fname=GWEN_DB_GetCharValue(dbArgs, "fileName", 0, 0);
  if (storeCardData(fname, card, dosMode,
                    "Krankenversichertenkarte",
		    LC_KvkCard_GetCardNumber(card),
		    pdata, idata)) {
    fprintf(stderr, "ERROR: Could not write to file.\n");
    if (dobeep)
      errorBeep();
    return RETURNVALUE_WORK;
  }

  if (v>1)
    fprintf(stderr, "Data written.\n");

  if (dobeep)
    okBeep();

  return 0;
}