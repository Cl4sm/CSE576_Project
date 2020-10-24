SCM l_bind(sockpt, address)
     SCM sockpt, address;
{
  int sts;
  ASRTER(NIMP(sockpt) && SOCKP(sockpt), sockpt, ARG1, s_bind);
  switch SOCKTYP(sockpt) {
  default:
    ASRTER(0, sockpt, s_unkfam, s_bind);
  case AF_UNIX:
    ASRTER(NIMP(address) && STRINGP(address), address, ARG2, s_bind);
    {
      struct sockaddr_un sa_server;
      bzero((char *) &sa_server, sizeof(sa_server));
      sa_server.sun_family = AF_UNIX;
      memcpy(&sa_server.sun_path, CHARS(address), 1+LENGTH(address));
      SYSCALL(sts = bind(fileno(STREAM(sockpt)),
			 (struct sockaddr *)&sa_server, sizeof(sa_server)););
    }
    break;
  case AF_INET:
    ASRTER(INUMP(address), address, ARG2, s_bind);
    {
      struct sockaddr_in sa_server;
      bzero((char *) &sa_server, sizeof(sa_server));
      sa_server.sin_family = AF_INET;
      sa_server.sin_addr.s_addr = htonl(INADDR_ANY);
      sa_server.sin_port = htons(INUM(address));
      SYSCALL(sts = bind(fileno(STREAM(sockpt)),
			 (struct sockaddr *)&sa_server, sizeof(sa_server)););
    }
    break;
  }
  return sts ? BOOL_F : sockpt;
}
