     SCM sstr, start, args;
{
  SCM end, port;
  long len;
  long alen = ilength(args);
  ASRTER(1 <= alen && alen <= 2, args, WNA, s_sub_rd);
  end = CAR(args);
  port = (2==alen) ? CAR(CDR(args)) : cur_inp;
  ASRTER(NIMP(sstr) && STRINGP(sstr), sstr, ARG1, s_sub_rd);
  ASRTER(INUMP(start), start, ARG2, s_sub_rd);
  ASRTER(INUMP(end), end, ARG3, s_sub_rd);
  ASRTER(NIMP(port) && OPINFPORTP(port), port, ARG4, s_sub_rd);
  len = LENGTH(sstr);
  start = INUM(start);
  end = INUM(end);
  ASRTER(0 <= start && start <= len, MAKINUM(start), OUTOFRANGE, s_sub_rd);
  ASRTER(0 <= end && end <= len, MAKINUM(end), OUTOFRANGE, s_sub_rd);
  if (start==end) return INUM0;
  if (start < end) {
    long ans = 0;
    /* An ungetc before an fread will not work on some systems if setbuf(0),
       so we read one element char by char. */
    if (CRDYP(port)) {
      CHARS(sstr)[start] = lgetc(port);
      start += 1;
      len -= 1;
      ans = 1;
    }
    SYSCALL(ans += fread(CHARS(sstr)+start,
			 (sizet)1,
			 (sizet)(end - start),
			 STREAM(port)););
    return MAKINUM(ans);
  }
  else {
    long idx = start;
    while (end < idx) {
      int chr = lgetc(port);
      if (EOF==chr) return MAKINUM(start - idx);
      CHARS(sstr)[--idx] = chr;
    }
    return MAKINUM(start - end);
  }
}
