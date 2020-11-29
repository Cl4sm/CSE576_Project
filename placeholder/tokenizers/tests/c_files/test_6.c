void __fastcall __noreturn main(int a1, char **a2, char **a3)
{
  __int64 *v3; // rbx
  __int64 v4; // rax
  __int64 v5; // rbx
  __int64 v6; // rax
  __int64 v7; // rbx
  __int64 v8; // rax
  __int64 v9; // rbx
  __int64 v10; // rax
  __int64 v11; // rbx
  int *v12; // rax
  char *v13; // rax
  int *v14; // rax
  char *v15; // rax
  int *v16; // rax
  char *v17; // rax
  int *v18; // rax
  char *v19; // rax
  char *v20; // [rsp+18h] [rbp-18h]

  v20 = getenv("HOME");
  setlocale(6, &locale);
  qword_60CA20 = (__int64)&unk_60C2C0;
  sub_402232(a1, a2);
  v3 = (__int64 *)qword_60CA20;
  if ( *(_QWORD *)qword_60CA20 )
    v4 = *(_QWORD *)qword_60CA20;
  else
    v4 = sub_407714(v20, "/.nrss/");
  *v3 = v4;
  v5 = qword_60CA20;
  if ( *(_QWORD *)(qword_60CA20 + 8) )
    v6 = *(_QWORD *)(qword_60CA20 + 8);
  else
    v6 = sub_407714(*(_QWORD *)qword_60CA20, "feeds/");
  *(_QWORD *)(v5 + 8) = v6;
  v7 = qword_60CA20;
  if ( *(_QWORD *)(qword_60CA20 + 16) )
    v8 = *(_QWORD *)(qword_60CA20 + 16);
  else
    v8 = sub_407714(*(_QWORD *)qword_60CA20, "config");
  *(_QWORD *)(v7 + 16) = v8;
  v9 = qword_60CA20;
  if ( *(_QWORD *)(qword_60CA20 + 24) )
    v10 = *(_QWORD *)(qword_60CA20 + 24);
  else
    v10 = sub_407714(*(_QWORD *)qword_60CA20, "log");
  *(_QWORD *)(v9 + 24) = v10;
  v11 = qword_60CA20;
  *(_QWORD *)(v11 + 32) = sub_407714(*(_QWORD *)qword_60CA20, ".cache");
  if ( unlink(*(const char **)(qword_60CA20 + 32)) && *__errno_location() != 2 )
  {
    sub_405A88();
    v12 = __errno_location();
    v13 = strerror(*v12);
    printf("Unlink failed: %s\n", v13);
    exit(-1);
  }
  if ( unlink(*(const char **)(qword_60CA20 + 24)) && *__errno_location() != 2 )
  {
    sub_405A88();
    v14 = __errno_location();
    v15 = strerror(*v14);
    printf("Unlink failed: %s\n", v15);
    exit(-1);
  }
  if ( mkdir(*(const char **)qword_60CA20, 0x1EDu) && *__errno_location() != 17 )
  {
    v16 = __errno_location();
    v17 = strerror(*v16);
    sub_40599D("Couldn't create config dir: %s\n", v17);
    sub_405A88();
    exit(-1);
  }
  if ( mkdir(*(const char **)(qword_60CA20 + 8), 0x1EDu) )
  {
    if ( *__errno_location() != 17 )
    {
      v18 = __errno_location();
      v19 = strerror(*v18);
      sub_40599D("Couldn't create feed dir: %s\n", v19);
      sub_405A88();
      exit(-1);
    }
  }
  sub_40599D("NRSS v%s\n", "0.3.9");
  sub_40874F(*(_QWORD *)(qword_60CA20 + 16), &off_60C280, &qword_60C9E0);
  signal(14, handler);
  signal(28, sub_4071BB);
  signal(2, (__sighandler_t)sub_405AEC);
  signal(17, sub_407265);
  signal(22, (__sighandler_t)1);
  signal(13, sub_4072AA);
  alarm(0x3Cu);
  sub_404614(*(_DWORD *)(qword_60CA20 + 132));
}
