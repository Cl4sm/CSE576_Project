SCM l_connect (sockpt, address, arg)
     SCM sockpt, address, arg;
{
  long flags;
  int sts;
  ASRTER(NIMP(sockpt) && SOCKP(sockpt), sockpt, ARG1, s_connect);
  switch SOCKTYP(sockpt) {
  default:
    ASRTER(0, sockpt, s_unkfam, s_connect);
  case AF_INET:
    ASRTER(NIMP(arg) && CONSP(arg) && NULLP(CDR(arg)), arg, WNA, s_connect);
    arg = CAR(arg);
    ASRTER(INUMP(arg), arg, ARG3, s_connect);
    {
      struct sockaddr_in soka;
      soka.sin_addr.s_addr =
	htonl(num2ulong(address, (char *)ARG2, s_connect));
      soka.sin_family = AF_INET;
      soka.sin_port = htons(INUM(arg));
      SYSCALL(sts = connect(fileno(STREAM(sockpt)),
			    (struct sockaddr*)&soka, sizeof(soka)););
    }
    break;
  case AF_UNIX:
    ASRTER(NULLP(arg), arg, WNA, s_connect);
    ASRTER(NIMP(address) && STRINGP(address), address, ARG2, s_connect);
    {
      struct sockaddr_un soka;
      soka.sun_family = AF_UNIX;
      memcpy(&soka.sun_path, CHARS(address), 1+LENGTH(address));
      SYSCALL(sts = connect(fileno(STREAM(sockpt)),
			    (struct sockaddr*)&soka, sizeof(soka)););
    }
    break;
  }
  if (sts) return BOOL_F;
  flags = tc16_fport | mode_bits("r+0", (char *)0);
  SCM_PORTFLAGS(sockpt) = flags;
  SCM_SETFLAGS(sockpt, flags);
  SCM_PORTDATA(sockpt) = cons(address, arg);
  return sockpt;
}
