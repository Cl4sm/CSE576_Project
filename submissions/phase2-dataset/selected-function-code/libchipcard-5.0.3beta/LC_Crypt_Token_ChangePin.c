int LC_Crypt_Token_ChangePin(GWEN_CRYPT_TOKEN *ct,
			     LC_CARD *hcard,
			     GWEN_CRYPT_PINTYPE pt,
			     int initial,
			     uint32_t guiid) {
  int rv;

  if (pt!=GWEN_Crypt_PinType_Access &&
      pt==GWEN_Crypt_PinType_Manage) {
    DBG_ERROR(LC_LOGDOMAIN, "Unknown pin type \"%s\"",
              GWEN_Crypt_PinType_toString(pt));
    return GWEN_ERROR_INVALID;
  }

  /* enter pin */
  rv=LC_Crypt_Token__ChangePin(ct, hcard, pt, initial, guiid);
  if (rv) {
    DBG_INFO(LC_LOGDOMAIN, "Error in pin input");
    return rv;
  }

  return 0;
}