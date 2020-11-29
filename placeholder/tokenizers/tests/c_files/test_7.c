__int64 banishment()
{
  int v1; // eax
  char v2[24]; // [rsp+0h] [rbp-30h] BYREF
  __int64 v3; // [rsp+18h] [rbp-18h]
  __int64 v4; // [rsp+20h] [rbp-10h]
  int v5; // [rsp+28h] [rbp-8h]
  int i; // [rsp+2Ch] [rbp-4h]

  v5 = 0;
  if ( (unsigned __int8)get_com("Choose a monster race (by symbol) to banish: ", v2) != 1 )
    return 0LL;
  for ( i = 1; ; ++i )
  {
    v1 = cave_monster_max(cave);
    if ( v1 <= i )
      break;
    v4 = cave_monster(cave, i);
    v3 = r_info + 176LL * *(__int16 *)(v4 + 8);
    if ( *(_WORD *)(v4 + 8)
      && !(unsigned __int8)flag_has_dbg(v3 + 106, 13LL, 1LL, "r_ptr->flags", "RF_UNIQUE")
      && *(_BYTE *)(v3 + 151) == v2[4] )
    {
      delete_monster_idx(i);
      v5 += Rand_div(4LL) + 1;
    }
  }
  take_hit((__int64)p_ptr, v5, (__int64)"the strain of casting Banishment");
  *((_DWORD *)p_ptr + 139) |= 0x1000000u;
  return 1LL;
}
