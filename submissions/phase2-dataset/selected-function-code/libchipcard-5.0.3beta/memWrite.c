int memWrite(LC_CLIENT *cl, GWEN_DB_NODE *dbArgs){
  LC_CARD *card=0;
  LC_CLIENT_RESULT res;
  int rv;
  int v;
  const char *s;
  int i;
  int offset;
  int size;
  const char *fname;
  FILE *f;
  int bps;
  int dt;
  time_t t0;
  time_t t1;
  int wantVerify;

  v=GWEN_DB_GetIntValue(dbArgs, "verbosity", 0, 0);
  wantVerify=GWEN_DB_GetIntValue(dbArgs, "verify", 0, 0);

  /* open file */
  fname=GWEN_DB_GetCharValue(dbArgs, "fileName", 0, 0);
  if (fname==0) {
    f=stdin;
  }
  else {
    f=fopen(fname,"r");
  }
  if (!f) {
    fprintf(stderr,
            I18N("ERROR: Could not open file (%s).\n"),
            strerror(errno));
    return RETURNVALUE_WORK;
  }

  if (v>1)
    fprintf(stderr, "Connecting to server.\n");
  res=LC_Client_Start(cl);
  if (res!=LC_Client_ResultOk) {
    showError(card, res, "StartWait");
    if (fname)
      fclose(f);
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
      if (fname)
        fclose(f);
      return RETURNVALUE_WORK;
    }
    if (v>0)
      fprintf(stderr, "Found a card.\n");

    s=LC_Card_GetCardType(card);
    assert(s);
    if (strcasecmp(s, "memory")==0)
      break;

    if (v>0)
      fprintf(stderr, "Not a memory card, releasing.\n");
    res=LC_Client_ReleaseCard(cl, card);
    if (res!=LC_Client_ResultOk) {
      showError(card, res, "ReleaseCard");
      if (fname)
        fclose(f);
      return RETURNVALUE_WORK;
    }
    LC_Card_free(card);

    if (i>15) {
      fprintf(stderr, "ERROR: No card found.\n");
      if (fname)
        fclose(f);
      return RETURNVALUE_WORK;
    }
  } /* for */

  /* extend card */
  rv=LC_MemoryCard_ExtendCard(card);
  if (rv) {
    fprintf(stderr, "Could not extend card as memory card\n");
    if (fname)
      fclose(f);
    LC_Client_ReleaseCard(cl, card);
    return RETURNVALUE_WORK;
  }

  /* open card */
  if (v>0)
    fprintf(stderr, "Opening card.\n");
  res=LC_Card_Open(card);
  if (res!=LC_Client_ResultOk) {
    fprintf(stderr,
            "ERROR: Error executing command CardOpen (%d).\n",
            res);
    if (fname)
      fclose(f);
    LC_Client_ReleaseCard(cl, card);
    return RETURNVALUE_WORK;
  }
  if (v>0)
    fprintf(stderr, "Card is a memory card as expected.\n");

  /* stop waiting */
  if (v>1)
    fprintf(stderr, "Telling the server that we need no more cards.\n");
  res=LC_Client_Stop(cl);
  if (res!=LC_Client_ResultOk) {
    showError(card, res, "Stop");
    if (fname)
      fclose(f);
    LC_Client_ReleaseCard(cl, card);
    return RETURNVALUE_WORK;
  }

  /* read file, write to card */
  offset=GWEN_DB_GetIntValue(dbArgs, "offset", 0, 0);
  size=GWEN_DB_GetIntValue(dbArgs, "size", 0, 0);

  if (v>0)
    fprintf(stderr, "Starting to write %d bytes at offset 0x%04x.\n",
            size, offset);
  bps=0;
  t0=time(0);
  i=size;
  while(i) {
    int t;
    char buf[1024];

    if (i>sizeof(buf))
      t=sizeof(buf);
    else
      t=i;

    /* read bytes */
    if (v>2)
      fprintf(stderr, "  Reading data from file\n");
    t=fread(buf, 1, t, f);
    if (t<1) {
      fprintf(stderr,
              I18N("ERROR: Could not read from file (%s).\n"),
              strerror(errno));
      if (fname)
        fclose(f);
      LC_Card_Close(card);
      LC_Client_ReleaseCard(cl, card);
      return RETURNVALUE_WORK;
    }

    if (v>1) {
      if (i>t)
        fprintf(stderr,
                "  Writing %4d bytes at 0x%04x (%d bytes left)\n",
                t, offset, i);
      else
        fprintf(stderr,
                "  Writing %4d bytes at 0x%04x (last chunk)\n",
                t, offset);
    }

    /* write bytes */
    res=LC_Client_ResultOk;
    res=LC_MemoryCard_WriteBinary(card,
                                  offset,
                                  buf,
                                  t);
    if (res!=LC_Client_ResultOk) {
      showError(card, res, "WriteBinary");
      if (fname)
        fclose(f);
      LC_Card_Close(card);
      LC_Client_ReleaseCard(cl, card);
      return RETURNVALUE_WORK;
    }

    if (wantVerify) {
      GWEN_BUFFER *tbuf;

      if (v>1)
        fprintf(stderr, "  Verifying data\n");
      tbuf=GWEN_Buffer_new(0, t, 0, 1);
      if (v>2)
        fprintf(stderr, "    Reading data for verification\n");
      res=LC_MemoryCard_ReadBinary(card,
                                   offset,
                                   t,
                                   tbuf);

      if (res!=LC_Client_ResultOk) {
        showError(card, res, "ReadBinary");
        if (fname)
          fclose(f);
        LC_Card_Close(card);
        LC_Client_ReleaseCard(cl, card);
        GWEN_Buffer_free(tbuf);
        return RETURNVALUE_WORK;
      }

      if (GWEN_Buffer_GetUsedBytes(tbuf)!=t ||
          memcmp(buf, GWEN_Buffer_GetStart(tbuf), t)!=0) {
        fprintf(stderr, "ERROR: Verification failed at offset 0x%04x\n",
                offset);
        if (fname)
          fclose(f);
        LC_Card_Close(card);
        LC_Client_ReleaseCard(cl, card);
        GWEN_Buffer_free(tbuf);
        return RETURNVALUE_WORK;
      }
      GWEN_Buffer_free(tbuf);
      if (v>2)
        fprintf(stderr, "    Verification ok\n");
    } /* if wantVerify */

    i-=t;
    offset+=t;
  } /* while */
  t1=time(0);
  dt=(int)difftime(t1, t0);
  if (dt==0)
    dt=1;
  bps=size/dt;

  if (v>0)
    fprintf(stderr,
            "Writing done [%d bytes/s].\n",
            bps);

  /* close file */
  if (fname) {
    if (fclose(f)) {
      fprintf(stderr,
              I18N("ERROR: Could not close file (%s).\n"),
              strerror(errno));
      LC_Card_Close(card);
      LC_Client_ReleaseCard(cl, card);
      return RETURNVALUE_WORK;
    }
  }

  /* close card */
  if (v>0)
    fprintf(stderr, "Closing card.\n");
  res=LC_Card_Close(card);
  if (res!=LC_Client_ResultOk) {
    showError(card, res, "CardClose");
    return RETURNVALUE_WORK;
  }
  else
    if (v>1)
      fprintf(stderr, "Card closed.\n");

  res=LC_Client_ReleaseCard(cl, card);
  if (res!=LC_Client_ResultOk) {
    showError(card, res, "ReleaseCard");
    return RETURNVALUE_WORK;
  }
  else
    if (v>1)
      fprintf(stderr, "Card released.\n");

  /* finished */
  if (v>1)
    fprintf(stderr, "Finished.\n");
  return 0;
}