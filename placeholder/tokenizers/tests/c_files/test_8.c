__int64 __fastcall tcfdbiterinit2(__int64 a1, __int64 a2)
{
  __int64 result; // rax
  __int64 v3; // [rsp+0h] [rbp-20h]
  unsigned __int8 v4; // [rsp+1Fh] [rbp-1h]

  v3 = a2;
  if ( !a1 )
    __assert_fail("fdb", "tcfdb.c", 0x530u, "tcfdbiterinit2");
  if ( *(_QWORD *)a1 && (unsigned __int8)sub_72DDA(a1, 1LL) != 1 )
    return 0LL;
  if ( *(int *)(a1 + 96) >= 0 )
  {
    if ( a2 == -1 )
    {
      v3 = *(_QWORD *)(a1 + 120);
    }
    else if ( a2 == -3 )
    {
      v3 = *(_QWORD *)(a1 + 128);
    }
    if ( v3 > 0 && (unsigned __int64)v3 <= *(_QWORD *)(a1 + 80) )
    {
      v4 = sub_72BDE(a1, v3);
      if ( *(_QWORD *)a1 )
        sub_72E77(a1);
      result = v4;
    }
    else
    {
      tcfdbsetecode(a1, 2LL, "tcfdb.c", 1341LL, "tcfdbiterinit2");
      if ( *(_QWORD *)a1 )
        sub_72E77(a1);
      result = 0LL;
    }
  }
  else
  {
    tcfdbsetecode(a1, 2LL, "tcfdb.c", 1331LL, "tcfdbiterinit2");
    if ( *(_QWORD *)a1 )
      sub_72E77(a1);
    result = 0LL;
  }
  return result;
}
