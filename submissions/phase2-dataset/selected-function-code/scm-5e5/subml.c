     SCM str1, start1, args;
{
  SCM end1, str2, start2;
  long i, j, e;
  ASRTER(3==ilength(args), args, WNA, s_subml);
  end1 = CAR(args); args = CDR(args);
  str2 = CAR(args); args = CDR(args);
  start2 = CAR(args);
  ASRTER(NIMP(str1) && STRINGP(str1), str1, ARG1, s_subml);
  ASRTER(INUMP(start1), start1, ARG2, s_subml);
  ASRTER(INUMP(end1), end1, ARG3, s_subml);
  ASRTER(NIMP(str2) && STRINGP(str2), str2, ARG4, s_subml);
  ASRTER(INUMP(start2), start2, ARG5, s_subml);
  i = INUM(start1), j = INUM(start2), e = INUM(end1);
  ASRTER(i <= LENGTH(str1) && i >= 0, start1, OUTOFRANGE, s_subml);
  ASRTER(j <= LENGTH(str2) && j >= 0, start2, OUTOFRANGE, s_subml);
  ASRTER(e <= LENGTH(str1) && e >= 0, end1, OUTOFRANGE, s_subml);
  ASRTER(e-i+j <= LENGTH(str2), start2, OUTOFRANGE, s_subml);
  while(i<e) CHARS(str2)[j++] = CHARS(str1)[i++];
  return UNSPECIFIED;
}
