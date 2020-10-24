static
int LC_Crypt_Token__GetPin(GWEN_CRYPT_TOKEN *ct,
			   LC_CARD *hcard,
			   int pid,
			   GWEN_CRYPT_PINTYPE pt,
			   GWEN_CRYPT_PINENCODING pe,
			   uint32_t flags,
			   unsigned char *buffer,
			   unsigned int minLength,
			   unsigned int maxLength,
			   unsigned int *pinLength,
			   uint32_t guiid) {
  int rv;

  rv=GWEN_Crypt_Token_GetPin(ct,
			     pt, pe, flags,
			     buffer,
			     minLength, maxLength,
			     pinLength,
			     guiid);
  if (rv==GWEN_ERROR_DEFAULT_VALUE) {
    LC_CLIENT_RESULT res;

    res=LC_Card_GetInitialPin(hcard, pid, buffer, maxLength,
                              pinLength);
    if (res) {
      DBG_INFO(LC_LOGDOMAIN, "here (%d)", res);
      return GWEN_ERROR_IO;
    }

    if (pe!=GWEN_Crypt_PinEncoding_Ascii) {
      rv=GWEN_Crypt_TransformPin(GWEN_Crypt_PinEncoding_Ascii,
				 pe,
				 buffer,
				 maxLength,
				 pinLength);
      if (rv) {
        DBG_INFO(LC_LOGDOMAIN, "here (%d)", rv);
        return rv;
      }
    }
  }
  else if (rv) {
    DBG_INFO(LC_LOGDOMAIN, "here (%d)", rv);
    return rv;
  }

  return 0;
}