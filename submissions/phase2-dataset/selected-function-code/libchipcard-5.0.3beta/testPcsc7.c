int testPcsc7(int argc, char **argv) {
  LC_CLIENT *cl;
  LC_CLIENT_RESULT res;
  LC_CARD *card;
  uint8_t cmdSelectDF[]={
      0x00, 0xa4, 0x04, 0x00,
      0x07,
      0xa0, 0x00, 0x00, 0x00, 0x04, 0x30, 0x60, 0x00};
  uint8_t cmdReadRecord[]={ 0x00, 0xb2, 0x00, 0x00, 0x00};
  GWEN_BUFFER *mbuf;
  int i;
  int j;

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

  fprintf(stderr, "Client Start\n");
  res=LC_Client_Start(cl);
  if (res!=LC_Client_ResultOk) {
    fprintf(stderr, "ERROR: Could not start using cards (%d).\n",
            res);
    return 2;
  }

  fprintf(stderr, "GetNextCard\n");
  res=LC_Client_GetNextCard(cl, &card, 20);
  if (res!=LC_Client_ResultOk) {
    fprintf(stderr, "ERROR: No card found (%d).\n",
            res);
    return 2;
  }

  fprintf(stderr, "Got this card:\n");
  LC_Card_Dump(card, 2);

  if (LC_ProcessorCard_ExtendCard(card)) {
    fprintf(stderr, "ERROR: Could not extend card as Processor card\n");
    return 2;
  }

  fprintf(stderr, "INFO: Opening card\n");
  res=LC_Card_Open(card);
  if (res!=LC_Client_ResultOk) {
    fprintf(stderr, "ERROR: Unable to open card (%d).\n", res);
    return 2;
  }
  fprintf(stderr, "Response was %d\n", res);

  mbuf=GWEN_Buffer_new(0, 256, 0, 1);

  res=LC_Card_ExecApdu(card, (const char*)cmdSelectDF, sizeof(cmdSelectDF),
		       mbuf, LC_Client_CmdTargetCard);
  if (res!=LC_Client_ResultOk) {
    fprintf(stderr, "ERROR: Wait timed out.\n");
    return 2;
  }

  for (i=1; i<32; i++) {
    cmdReadRecord[3]=(i<<3) | 4; /* SFI */
    for (j=1; j<16; j++) {
      cmdReadRecord[2]=j; /* record number */
      res=LC_Card_ExecApdu(card, (const char*)cmdReadRecord, sizeof(cmdReadRecord),
			   mbuf, LC_Client_CmdTargetCard);
      if (res!=LC_Client_ResultOk) {
	fprintf(stderr, "ERROR: Wait timed out.\n");
      }
      else {
	fprintf(stdout, "SFI=%d, Record=%d:\n", i, j);
	GWEN_Text_DumpString(GWEN_Buffer_GetStart(mbuf), GWEN_Buffer_GetUsedBytes(mbuf),
                             2);
      }
      GWEN_Buffer_Reset(mbuf);
    }
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