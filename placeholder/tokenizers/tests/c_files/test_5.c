void __fastcall main(unsigned int a1, char **a2, char **a3, double a4, __m128i a5)
{
  size_t v5; // rbx
  int v6; // eax
  char *v7; // rax
  size_t v8; // rbx
  unsigned int v9; // eax
  struct sigaction s; // [rsp+10h] [rbp-2D0h] BYREF
  char nptr[256]; // [rsp+B0h] [rbp-230h] BYREF
  char v12[264]; // [rsp+1B0h] [rbp-130h] BYREF
  const char *v13; // [rsp+2B8h] [rbp-28h]
  const char *v14; // [rsp+2C0h] [rbp-20h]
  const char *v15; // [rsp+2C8h] [rbp-18h]

  v15 = 0LL;
  v14 = 0LL;
  v13 = 0LL;
  bzero(&s, 0x98uLL);
  bzero(v12, 0xFFuLL);
  snprintf(v12, 0xFFuLL, "%s", *a2);
  dword_603D9C = 1099956224;
  dword_603D8C = 1099956224;
  dword_603C90 = 20000;
  dword_603BC4 = 20000;
  byte_603C00[0] = 0;
  byte_603D00[0] = 0;
  while ( 1 )
  {
    dword_603B80 = getopt(a1, a2, "m:vhr:q:t:n:w:d:c:kl");
    if ( dword_603B80 == -1 )
      break;
    nptr[0] = 0;
    switch ( dword_603B80 )
    {
      case 0:
        continue;
      case 99:
        v14 = optarg;
        break;
      case 100:
        if ( *optarg != 44 && *optarg != 46 || strlen(optarg) <= 1 )
          sub_4013AA(v12);
        dword_603BC8 = *optarg;
        while ( 1 )
        {
          v7 = optarg++;
          if ( !*v7 )
            break;
          v6 = dword_603B84++;
          nptr[v6] = *optarg;
        }
        nptr[dword_603B84] = 0;
        snprintf(&needle, 0x40uLL, "%s", nptr);
        break;
      case 107:
        dword_603B88 = 1;
        break;
      case 108:
        dword_603BC0 = 1;
        break;
      case 109:
        snprintf(nptr, 0xFFuLL, "%s", optarg);
        for ( dword_603B84 = 0; ; ++dword_603B84 )
        {
          v5 = (unsigned int)dword_603B84;
          if ( v5 >= strlen(nptr) )
            break;
          if ( ((*__ctype_b_loc())[nptr[dword_603B84]] & 0x800) == 0 )
            sub_4013AA(v12);
        }
        dword_603C8C = atoi(nptr);
        break;
      case 110:
        snprintf(byte_603D00, 0x89uLL, "%s", optarg);
        break;
      case 113:
        snprintf(nptr, 0xFFuLL, "%s", optarg);
        for ( dword_603B84 = 0; ; ++dword_603B84 )
        {
          v8 = (unsigned int)dword_603B84;
          if ( v8 >= strlen(nptr) )
            break;
          if ( ((*__ctype_b_loc())[nptr[dword_603B84]] & 0x800) == 0 )
            sub_4013AA(v12);
        }
        seconds = atoi(nptr);
        break;
      case 114:
        v13 = optarg;
        break;
      case 116:
        v15 = optarg;
        break;
      case 118:
        fprintf(stdout, "%s\n%s\n", "randtype 1.13", "Copyright (C) 1999-2001 bjk <bjk@arbornet.org>");
        exit(0);
      case 119:
        snprintf(byte_603C00, 0x89uLL, "%s", optarg);
        break;
      default:
        sub_4013AA(v12);
    }
  }
  if ( v13 && (unsigned int)sub_4020C5(v13) )
    sub_4013AA(v12);
  if ( v15 && (unsigned int)sub_40197A(v15, &dword_603D8C, &dword_603BC4, a4, a5) )
    sub_4013AA(v12);
  if ( !dword_603BC0 )
  {
    if ( v14 && byte_603C00[0] && (unsigned int)sub_40197A(v14, &dword_603D9C, &dword_603C90, a4, a5) )
      sub_4013AA(v12);
    setvbuf(stdout, 0LL, 2, 0x2000uLL);
  }
  sigemptyset(&s.sa_mask);
  sigfillset(&s.sa_mask);
  s.sa_handler = (__sighandler_t)sub_401360;
  sigdelset(&s.sa_mask, 15);
  sigdelset(&s.sa_mask, 14);
  sigdelset(&s.sa_mask, 2);
  sigdelset(&s.sa_mask, 20);
  sigprocmask(0, &s.sa_mask, 0LL);
  sigaction(2, &s, 0LL);
  sigaction(15, &s, 0LL);
  sigaction(14, &s, 0LL);
  v9 = getpid();
  srandom(v9);
  alarm(seconds);
  if ( a1 == optind )
  {
    status |= sub_401C77((__int64)"-", dword_603B88);
  }
  else
  {
    for ( dword_603B84 = optind; a1 > dword_603B84; ++dword_603B84 )
      status |= sub_401C77((__int64)a2[dword_603B84], dword_603B88);
  }
  sub_4012F6();
  exit(status);
}
