int testPcsc6(int argc, char **argv) {
  LC_CLIENT *cl;
  LC_CLIENT_RESULT res;
  LC_CARD *card;
  GWEN_BUFFER *mbuf;
  GWEN_DB_NODE *dbRecord;

  cl=LC_Client_new("test", "0.1");
  if (!cl) {
    fprintf(stderr, "ERROR: Could not create client.\n");
    return 1;
  }

  res=LC_Client_Init(cl);
  if (res!=LC_Client_ResultOk) {
    fprintf(stderr, "ERROR: Could not init client (%d).\n",
            res);
    return 2;
  }

  res=LC_Client_Start(cl);
  if (res!=LC_Client_ResultOk) {
    fprintf(stderr, "ERROR: Could not start using cards (%d).\n",
            res);
    return 2;
  }

  res=LC_Client_GetNextCard(cl, &card, 20);
  if (res!=LC_Client_ResultOk) {
    fprintf(stderr, "ERROR: No card found (%d).\n",
            res);
    return 2;
  }

  fprintf(stderr, "Got this card:\n");
  LC_Card_Dump(card, 2);


  if (LC_DDVCard_ExtendCard(card)) {
    fprintf(stderr, "ERROR: Could not extend card as DDV card\n");
    return 2;
  }

  fprintf(stderr, "INFO: Opening card\n");
  res=LC_Card_Open(card);
  if (res!=LC_Client_ResultOk) {
    fprintf(stderr, "ERROR: Unable to open card (%d).\n", res);
    return 2;
  }
  fprintf(stderr, "Response was %d\n", res);

  fprintf(stderr, "Selecting EF...\n");
  res=LC_Card_SelectEf(card, "EF_BNK");
  if (res!=LC_Client_ResultOk) {
    fprintf(stderr, "ERROR: Wait timed out.\n");
    return 2;
  }
  fprintf(stderr, "Response was %d\n", res);

  fprintf(stderr, "Reading record...\n");
  mbuf=GWEN_Buffer_new(0, 256, 0, 1);
  res=LC_Card_IsoReadRecord(card, LC_CARD_ISO_FLAGS_RECSEL_GIVEN,
                            1, mbuf);
  if (res!=LC_Client_ResultOk) {
    fprintf(stderr, "ERROR: Wait timed out.\n");
    return 2;
  }
  fprintf(stderr, "Response was %d\n", res);
  GWEN_Text_DumpString(GWEN_Buffer_GetStart(mbuf),
                       GWEN_Buffer_GetUsedBytes(mbuf), 2);
  GWEN_Buffer_Rewind(mbuf);

  dbRecord=GWEN_DB_Group_new("record");
  if (LC_Card_ParseRecord(card, 1, mbuf, dbRecord)) {
    fprintf(stderr, "Error parsing record.\n");
  }
  else {
    fprintf(stderr, "Parsed record is:\n");
    GWEN_DB_Dump(dbRecord, 2);
  }

  GWEN_Buffer_Reset(mbuf);
  if (LC_Card_CreateRecord(card, 1, mbuf, dbRecord)) {
    fprintf(stderr, "Error creating record.\n");
  }
  else {
    fprintf(stderr, "Created record is:\n");
    GWEN_Buffer_Dump(mbuf, 2);
  }


  res=LC_Client_ReleaseCard(cl, card);
  if (res!=LC_Client_ResultOk) {
    fprintf(stderr, "ERROR: Unable to release card (%d).\n",
            res);
    return 2;
  }
  LC_Card_free(card);


  res=LC_Client_Stop(cl);
  if (res!=LC_Client_ResultOk) {
    fprintf(stderr, "ERROR: Could not stop using cards (%d).\n",
            res);
    return 2;
  }

  res=LC_Client_Fini(cl);
  if (res!=LC_Client_ResultOk) {
    fprintf(stderr, "ERROR: Could not fini client (%d).\n",
            res);
    return 2;
  }

  return 0;
}