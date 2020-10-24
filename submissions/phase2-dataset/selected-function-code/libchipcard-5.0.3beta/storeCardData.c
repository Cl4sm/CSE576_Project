static int storeCardData(const char *fname,
			 LC_CARD *cd,
			 int dosMode,
                         const char *cardType,
			 const char *cardNumber,
			 const LC_HI_PERSONAL_DATA *pData,
			 const LC_HI_INSURANCE_DATA *iData) {
  FILE *f;
  GWEN_BUFFER *dbuf;
  GWEN_TIME *ti;
  const char *CRLF;
  const char *s;
  const GWEN_TIME *cti;

  ti=GWEN_CurrentTime();
  assert(ti);

  if (dosMode)
    CRLF="\r\n";
  else
    CRLF="\n";

  if (fname) {
    f=fopen(fname, "w+");
    if (f==0) {
      fprintf(stderr,
	      "Could not create file \"%s\", reason: %s\n",
	      fname,
	      strerror(errno));
      GWEN_Time_free(ti);
      return -1;
    }
  }
  else
    f=stdout;

  /* header */
  fprintf(f,"Version:libchipcard4-"CHIPCARD_VERSION_FULL_STRING"%s", CRLF);
  dbuf=GWEN_Buffer_new(0, 32, 0, 1);
  GWEN_Time_toString(ti, "DD.MM.YYYY", dbuf);
  fprintf(f, "Datum:%s%s", GWEN_Buffer_GetStart(dbuf), CRLF);
  GWEN_Buffer_Reset(dbuf);
  GWEN_Time_toString(ti, "hh:mm:ss", dbuf);
  fprintf(f, "Zeit:%s%s", GWEN_Buffer_GetStart(dbuf), CRLF);
  GWEN_Time_free(ti);
  GWEN_Buffer_free(dbuf); dbuf=0;
  fprintf(f, "Lesertyp:%s%s", LC_Card_GetReaderType(cd), CRLF);
  fprintf(f, "Kartentyp:%s%s", cardType?cardType:"", CRLF);

  /* insurance data */
  s=LC_HIInsuranceData_GetInstitutionName(iData);
  fprintf(f,"KK-Name:%s%s", s?s:"", CRLF);
  s=LC_HIInsuranceData_GetInstitutionId(iData);
  fprintf(f,"KK-Nummer:%s%s", s?s:"", CRLF);
  /* this is not the card number but the "Vertragskassennummer" */
  fprintf(f,"VKNR:%s%s", cardNumber?cardNumber:"", CRLF);
  s=LC_HIPersonalData_GetInsuranceId(pData);
  fprintf(f,"V-Nummer:%s%s", s?s:"", CRLF);
  s=LC_HIInsuranceData_GetStatus(iData);
  fprintf(f,"V-Status:%s%s", s?s:"", CRLF);
  s=LC_HIInsuranceData_GetGroup(iData);
  fprintf(f,"V-Statusergaenzung:%s%s", s?s:"", CRLF);
  if (s) {
    const char *x=0;

    switch(*s) {
    case '1': x="west"; break;
    case '9': x="ost"; break;
    case '6': x="BVG"; break;
    case '7': x="SVA, nach Aufwand, dt.-nl Grenzgaenger"; break;
    case '8': x="SVA, pauschal"; break;
    case 'M': x="DMP Diabetes mellitus Typ 2, west"; break;
    case 'X': x="DMP Diabetes mellitus Typ 2, ost"; break;
    case 'A': x="DMP Brustkrebs, west"; break;
    case 'C': x="DMP Brustkrebs, ost"; break;
    case 'K': x="DMP KHK, west"; break;
    case 'L': x="DMP KHK, ost"; break;
    case '4': x="nichtversicherter Sozialhilfe-Empfaenger"; break;
    case 'E': x="DMP Diabetes mellitus Typ 1, west"; break;
    case 'N': x="DMP Diabetes mellitus Typ 1, ost"; break;
    case 'D': x="DMP Asthma bronchiale, west"; break;
    case 'F': x="DMP Asthma bronchiale, ost"; break;
    case 'S': x="DMP COPD, west"; break;
    case 'P': x="DMP COPD, ost"; break;
    default:  x=0;
    }
    if (x)
      fprintf(f,"V-Status-Erlaeuterung:%s%s", x, CRLF);
  }
  s=LC_HIPersonalData_GetTitle(pData);
  fprintf(f,"Titel:%s%s", s?s:"", CRLF);
  s=LC_HIPersonalData_GetPrename(pData);
  fprintf(f,"Vorname:%s%s", s?s:"", CRLF);
  s=LC_HIPersonalData_GetNameSuffix(pData);
  fprintf(f,"Namenszusatz:%s%s", s?s:"", CRLF);
  s=LC_HIPersonalData_GetName(pData);
  fprintf(f,"Familienname:%s%s", s?s:"", CRLF);
  cti=LC_HIPersonalData_GetDateOfBirth(pData);
  if (cti) {
    GWEN_BUFFER *tbuf;

    tbuf=GWEN_Buffer_new(0, 32, 0, 1);
    GWEN_Time_toUtcString(cti, "DDMMYYYY", tbuf);
    fprintf(f,"Geburtsdatum:%s%s",
	    GWEN_Buffer_GetStart(tbuf), CRLF);
    GWEN_Buffer_free(tbuf);
  }

  s=LC_HIPersonalData_GetAddrStreet(pData);
  fprintf(f,"Strasse:%s%s", s?s:"", CRLF);
  s=LC_HIPersonalData_GetAddrState(pData);
  fprintf(f,"Laendercode:%s%s", s?s:"", CRLF);
  s=LC_HIPersonalData_GetAddrZipCode(pData);
  fprintf(f,"PLZ:%s%s", s?s:"", CRLF);
  s=LC_HIPersonalData_GetAddrCity(pData);
  fprintf(f,"Ort:%s%s", s?s:"", CRLF);

  cti=LC_HIInsuranceData_GetCoverBegin(iData);
  if (cti) {
    GWEN_BUFFER *tbuf;

    tbuf=GWEN_Buffer_new(0, 32, 0, 1);
    GWEN_Time_toUtcString(cti, "DDMMYY", tbuf);
    fprintf(f,"gueltig-seit:%s%s",
	    GWEN_Buffer_GetStart(tbuf), CRLF);
    GWEN_Buffer_free(tbuf);
  }

  cti=LC_HIInsuranceData_GetCoverEnd(iData);
  if (cti) {
    GWEN_BUFFER *tbuf;

    tbuf=GWEN_Buffer_new(0, 32, 0, 1);
    GWEN_Time_toUtcString(cti, "MMYY", tbuf);
    fprintf(f,"gueltig-bis:%s%s",
	    GWEN_Buffer_GetStart(tbuf), CRLF);
    GWEN_Buffer_free(tbuf);
  }

  fprintf(f,"Pruefsumme-gueltig:ja%s", CRLF);
  fprintf(f,"Kommentar:derzeit keiner%s", CRLF);

  if (fname) {
    if (fclose(f)) {
      DBG_ERROR(0, "Could not close file \"%s\", reason: \n %s",
		fname,
		strerror(errno));
      return -1;
    }
  }

  return 0;
}