     SCM x, env;
{
  SCM proc, argv[3];
  x = CDR(x);
  proc = scm_lookupval(x, 0);
  ASRTGO(KEYWORDP(proc), errout);
  proc = KEYWORD_MACRO(proc);
  argv[0] = x;
  argv[1] = env;
  argv[2] = EOL;
  switch (MAC_TYPE(proc) & ~MAC_PRIMITIVE) {
  default:
  errout: wta(proc, CHARS(CAR(x)), "macro expected");
  case MAC_MACRO:
    x = scm_cvapply(CDR(proc), 3L, argv);
    if (ilength(x) <= 0)
      x = cons2(IM_BEGIN, x, EOL);
    return x;
  case MAC_ACRO:
    x = scm_cvapply(CDR(proc), 3L, argv);
    return cons2(IM_QUOTE, x, EOL);
  }
}
