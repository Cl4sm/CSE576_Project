static
int LC_Crypt_Token__EnterPin(GWEN_CRYPT_TOKEN *ct,
			     LC_CARD *hcard,
			     GWEN_CRYPT_PINTYPE pt,
			     uint32_t guiid) {
  LC_PININFO *pi;
  int rv;

  assert(hcard);

  if (pt==GWEN_Crypt_PinType_Manage) {
    pi=LC_Card_GetPinInfoByName(hcard, "eg_pin");
  }
  else {
    pi=LC_Card_GetPinInfoByName(hcard, "ch_pin");
  }
  assert(pi);

  rv=LC_Crypt_Token__EnterPinWithPinInfo(ct, hcard, pt, pi, guiid);
  LC_PinInfo_free(pi);

  return rv;
}