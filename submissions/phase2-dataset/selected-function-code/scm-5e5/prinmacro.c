     SCM exp;
     SCM port;
     int writing;
{
  lputs("#<", port);
  if (MAC_TYPE(exp) & MAC_PRIMITIVE) lputs("primitive-", port);
  switch (MAC_TYPE(exp) & ~MAC_PRIMITIVE) {
  default:
    lputs("macro", port); break;
  case MAC_ACRO:
    lputs("syntax", port); break;
#ifdef MAC_INLINE
  case MAC_INLINE:
    lputs("inline function", port); break;
#endif
  }
  if (MAC_TYPE(exp) & MAC_MEMOIZING) lputc('!', port);
  lputc(' ', port);
  scm_iprin1(CDR(exp), port, writing);
  lputc('>', port);
  return !0;
}
