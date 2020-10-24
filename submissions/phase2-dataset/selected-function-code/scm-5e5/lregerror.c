SCM lregerror(scode)
     SCM scode;
{
  int code;
  /* added by Denys Duchier: conditional declaration */
#ifdef __REGEXP_LIBRARY_H__
  int len;
#endif
  SCM str;
  ASRTER(INUMP(scode), scode, ARG1, s_regerror);
  code = INUM(scode);
  if (code < 0)
    return makfromstr("Invalid code", sizeof("Invalid code")-1);
  /* XXX - is regerror posix or not? */
#ifdef __REGEXP_LIBRARY_H__
  /* XXX - gnu regexp doesn't use the re parameter, so we will
     ignore it in a very untidy way. */
  len = regerror(code, 0L, 0L, 0);
  str = makstr(len-1);
  regerror(code, 0L, CHARS(str), len);
#else
  str = makfromstr(s_error, (sizet)5);
#endif
  return str;
}
