SCM scm_subbytes_write(sstr, start, args)
     SCM sstr, start, args;
{
  SCM end, port;
  long len;
  long alen = ilength(args);
  ASRTER(1 <= alen && alen <= 2, args, WNA, s_sub_wr);
  end = CAR(args);
  port = (2==alen) ? CAR(CDR(args)) : cur_outp;
  ASRTER(NIMP(sstr) && STRINGP(sstr), sstr, ARG1, s_sub_wr);
  ASRTER(INUMP(start), start, ARG2, s_sub_wr);
  ASRTER(INUMP(end), end, ARG3, s_sub_wr);
  ASRTER(NIMP(port) && OPOUTFPORTP(port), port, ARG4, s_sub_wr);
  len = LENGTH(sstr);
  start = INUM(start);
  end = INUM(end);
  ASRTER(0 <= start && start <= len, MAKINUM(start), OUTOFRANGE, s_sub_wr);
  ASRTER(0 <= end && end <= len, MAKINUM(end), OUTOFRANGE, s_sub_wr);
  if (start==end) return INUM0;
  if (start < end) {
    long ans;
    SYSCALL(ans = lfwrite(CHARS(sstr)+start,
			  (sizet)1,
			  (sizet)(sizet)(end - start),
			  port););
    return MAKINUM(ans);
  }
  else {
    long idx = start;
    while (end <= --idx) {
      if (feof(STREAM(port))) return MAKINUM(start - idx - 1);
      lputc(CHARS(sstr)[idx], port);
    }
    return MAKINUM(start - end);
  }
}
