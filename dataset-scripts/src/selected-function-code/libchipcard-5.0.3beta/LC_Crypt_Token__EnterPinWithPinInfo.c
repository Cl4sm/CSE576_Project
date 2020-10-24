static
int LC_Crypt_Token__EnterPinWithPinInfo(GWEN_CRYPT_TOKEN *ct,
					LC_CARD *hcard,
					GWEN_CRYPT_PINTYPE pt,
					const LC_PININFO *pi,
					uint32_t guiid) {
  LC_CLIENT_RESULT res;
  int maxErrors;
  int currentErrors;

  assert(hcard);
  assert(pi);

#if 0
  if (pi) {
    GWEN_DB_NODE *dbDEBUG;

    dbDEBUG=GWEN_DB_Group_new("PinInfo");
    LC_PinInfo_toDb(pi, dbDEBUG);
    GWEN_DB_Dump(dbDEBUG, stderr, 2);
    GWEN_DB_Group_free(dbDEBUG);
  }
#endif

  res=LC_Card_GetPinStatus(hcard,
                           LC_PinInfo_GetId(pi),
                           &maxErrors,
                           &currentErrors);
  if (res!=LC_Client_ResultNotSupported) {
    if (res!=LC_Client_ResultOk) {
      DBG_ERROR(LC_LOGDOMAIN,
                "Unable to read status of pin %x (%d)",
                LC_PinInfo_GetId(pi),
                res);
      return LC_Crypt_Token_ResultToError(res);
    }

    if ((currentErrors!=maxErrors) &&
        !(GWEN_Crypt_Token_GetModes(ct) &
	  GWEN_CRYPT_TOKEN_MODE_FORCE_PIN_ENTRY)
       ){
      DBG_ERROR(LC_LOGDOMAIN,
                "Bad pin entered at least once before, aborting");
      return GWEN_ERROR_ABORTED;
    }
  }
  else {
    DBG_INFO(LC_LOGDOMAIN,
	     "Unable to read pin status for pin %02x (not supported)",
	     LC_PinInfo_GetId(pi));
  }

  if ((pt!=GWEN_Crypt_PinType_Manage) &&
      (LC_Card_GetReaderFlags(hcard) & LC_READER_FLAGS_KEYPAD) &&
      !(GWEN_Crypt_Token_GetModes(ct) & GWEN_CRYPT_TOKEN_MODE_FORCE_PIN_ENTRY)) {
    uint32_t bid;
    int triesLeft=-1;

    DBG_INFO(LC_LOGDOMAIN,"Terminal has a keypad, will ask for pin.");
    /* tell the user about pin verification */
    bid=GWEN_Crypt_Token_BeginEnterPin(ct, pt, guiid);
    if (bid==0) {
      DBG_ERROR(LC_LOGDOMAIN, "Error in user interaction");
      return GWEN_ERROR_GENERIC;
    }

    res=LC_Card_IsoPerformVerification(hcard, 0, pi, &triesLeft);

    if (res!=LC_Client_ResultOk) {
      /* tell the user about end of pin verification */
      GWEN_Crypt_Token_EndEnterPin(ct, pt, 0, bid);
      DBG_ERROR(LC_LOGDOMAIN, "sw1=%02x sw2=%02x (%s)",
                LC_Card_GetLastSW1(hcard),
                LC_Card_GetLastSW2(hcard),
                LC_Card_GetLastText(hcard));

      if (LC_Card_GetLastSW1(hcard)==0x63) {
	switch (LC_Card_GetLastSW2(hcard)) {
        case 0xc0: /* no error left */
	  return GWEN_ERROR_BAD_PIN_0_LEFT;
        case 0xc1: /* one left */
          return GWEN_ERROR_BAD_PIN_1_LEFT;
        case 0xc2: /* two left */
          return GWEN_ERROR_BAD_PIN_2_LEFT;
        default:   /* unknown error */
          return GWEN_ERROR_BAD_PIN;
        } // switch
      }
      else if (LC_Card_GetLastSW1(hcard)==0x69 &&
               LC_Card_GetLastSW2(hcard)==0x83) {
        DBG_ERROR(LC_LOGDOMAIN, "Card unusable");
        return GWEN_ERROR_IO;
      }
      else if (LC_Card_GetLastSW1(hcard)==0x64 &&
               LC_Card_GetLastSW2(hcard)==0x01) {
        DBG_ERROR(LC_LOGDOMAIN, "Aborted by user");
        return GWEN_ERROR_USER_ABORTED;
      }
      else {
        if (triesLeft>=0) {
          switch (triesLeft) {
          case 0: /* no error left */
            return GWEN_ERROR_BAD_PIN_0_LEFT;
          case 1: /* one left */
            return GWEN_ERROR_BAD_PIN_1_LEFT;
          case 2: /* two left */
            return GWEN_ERROR_BAD_PIN_2_LEFT;
          default:   /* unknown count */
            return GWEN_ERROR_BAD_PIN;
          } // switch
        }

        return GWEN_ERROR_IO;
      }
    } /* if not ok */
    else {
      /* PIN ok */
      DBG_INFO(LC_LOGDOMAIN, "Pin ok");
      GWEN_Crypt_Token_EndEnterPin(ct, pt, 1, bid);
    }
  } /* if hasKeyPad */
  else {
    unsigned char pinBuffer[64];
    int mres;
    int pinMaxLen;
    unsigned int pinLength;
    unsigned int origPinLength;
    uint32_t pflags=0;
    GWEN_CRYPT_PINENCODING pe;
    int triesLeft=-1;

    DBG_INFO(LC_LOGDOMAIN, "No keypad (or disabled), will ask for PIN");
    memset(pinBuffer, 0, sizeof(pinBuffer));

    pe=LC_PinInfo_GetEncoding(pi);
    if (pt==GWEN_Crypt_PinType_Manage)
      pflags|=GWEN_GUI_INPUT_FLAGS_ALLOW_DEFAULT;
    pflags|=GWEN_GUI_INPUT_FLAGS_NUMERIC;
    pinLength=0;
    pinMaxLen=LC_PinInfo_GetMaxLength(pi);
    if (!pinMaxLen || pinMaxLen>sizeof(pinBuffer)-1)
      pinMaxLen=sizeof(pinBuffer)-1;
    mres=LC_Crypt_Token__GetPin(ct,
				hcard,
				LC_PinInfo_GetId(pi),
				pt,
				pe,
				pflags,
				pinBuffer,
				LC_PinInfo_GetMinLength(pi),
				pinMaxLen,
				&pinLength,
				guiid);
    if (mres!=0) {
      DBG_ERROR(LC_LOGDOMAIN, "Error asking for PIN, aborting");
      memset(pinBuffer, 0, sizeof(pinBuffer));
      return mres;
    }
    origPinLength=pinLength;

    if (pinLength<pinMaxLen && LC_PinInfo_GetFiller(pi)) {
      int i;
      unsigned char c;

      c=(unsigned char)LC_PinInfo_GetFiller(pi);
      for (i=pinLength; i<pinMaxLen; i++)
        pinBuffer[i]=c;
      pinLength=pinMaxLen;
    }

    DBG_INFO(LC_LOGDOMAIN, "Verifying the PIN");
    res=LC_Card_IsoVerifyPin(hcard,
                             0,
                             pi,
                             pinBuffer,
                             pinLength,
                             &triesLeft);
    if (res!=LC_Client_ResultOk) {
      DBG_ERROR(LC_LOGDOMAIN, "sw1=%02x sw2=%02x (%s)",
                LC_Card_GetLastSW1(hcard),
                LC_Card_GetLastSW2(hcard),
                LC_Card_GetLastText(hcard));

      if (LC_Card_GetLastSW1(hcard)==0x63) {
        /* set pin status */
	GWEN_Crypt_Token_SetPinStatus(ct,
				      pt,
				      pe,
				      pflags,
				      pinBuffer,
				      origPinLength,
				      0,
				      guiid);

        switch (LC_Card_GetLastSW2(hcard)) {
        case 0xc0: /* no error left */
          return GWEN_ERROR_BAD_PIN_0_LEFT;
        case 0xc1: /* one left */
          return GWEN_ERROR_BAD_PIN_1_LEFT;
        case 0xc2: /* two left */
          return GWEN_ERROR_BAD_PIN_2_LEFT;
        default:
          return GWEN_ERROR_BAD_PIN;
        } // switch
      }
      else if (LC_Card_GetLastSW1(hcard)==0x69 &&
               LC_Card_GetLastSW2(hcard)==0x83) {
        /* set pin status */
	GWEN_Crypt_Token_SetPinStatus(ct,
				      pt,
				      pe,
				      pflags,
				      pinBuffer,
				      origPinLength,
				      0,
				      guiid);
        DBG_ERROR(LC_LOGDOMAIN, "Card unusable");
        return GWEN_ERROR_IO;
      }
      else if (LC_Card_GetLastSW1(hcard)==0x64 &&
               LC_Card_GetLastSW2(hcard)==0x01) {
        return GWEN_ERROR_USER_ABORTED;
      }
      else {
        if (triesLeft>=0) {
          /* set pin status */
	  GWEN_Crypt_Token_SetPinStatus(ct,
					pt,
					pe,
					pflags,
					pinBuffer,
					origPinLength,
					0,
					guiid);
	  switch (triesLeft) {
          case 0: /* no error left */
            return GWEN_ERROR_BAD_PIN_0_LEFT;
          case 1: /* one left */
	    return GWEN_ERROR_BAD_PIN_1_LEFT;
          case 2: /* two left */
            return GWEN_ERROR_BAD_PIN_2_LEFT;
          default:   /* unknown count */
            return GWEN_ERROR_BAD_PIN;
          } // switch
        }
        DBG_ERROR(LC_LOGDOMAIN, "Unknown error");
        return GWEN_ERROR_IO;
      }
    } // if not ok
    else {
      DBG_INFO(LC_LOGDOMAIN, "PIN ok");
      /* set pin status */
      GWEN_Crypt_Token_SetPinStatus(ct,
				    pt,
				    pe,
				    pflags,
				    pinBuffer,
				    origPinLength,
				    1,
				    guiid);
    }
  } // if no keyPad

  return 0;
}