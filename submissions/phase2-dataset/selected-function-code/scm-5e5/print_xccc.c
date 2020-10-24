     SCM exp;
     SCM f;
     int writing;
{
  XcmsColorSpace **papColorSpaces;
  XcmsCCC xccc = XCCC(exp);
  lputs("#<X CCC", f);
  papColorSpaces =
    ((XcmsFunctionSet *)xccc->pPerScrnInfo->functionSet)->DDColorSpaces;
  if (papColorSpaces != NULL) {
    while (*papColorSpaces != NULL) {
      lputs(" ", f);
      lputs((*papColorSpaces)->prefix, f);
      papColorSpaces++;
    }
  }

  lputc('>', f);
  return 1;
}
