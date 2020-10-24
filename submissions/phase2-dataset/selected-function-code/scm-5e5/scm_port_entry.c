     FILE *stream;
     long ptype, flags;
{
  SCM z;
  sizet nlen;
  int i, j;
  VERIFY_INTS("scm_port_entry", 0L);
  flags = flags | (ptype & ~0xffffL);
  ASRTER(flags, INUM0, ARG1, "scm_port_entry");
  for (i = 1; i < scm_port_table_len; i++)
    if (0L==scm_port_table[i].flags) goto ret;
  if (scm_port_table_len <= SCM_PORTNUM_MAX) {
    nlen = scm_port_table_len + (scm_port_table_len / 2);
    if (nlen >= SCM_PORTNUM_MAX) nlen = (sizet)SCM_PORTNUM_MAX + 1;
    scm_port_table = (port_info *)
      must_realloc((char *)scm_port_table,
		   (long)scm_port_table_len * sizeof(port_info),
		   (long)nlen * sizeof(port_info),
		   s_port_table);
    scm_port_table_len = nlen;
    growth_mon(s_port_table, nlen + 0L, "entries", !0);
    for (j = i; j < scm_port_table_len; j++) {
      scm_port_table[j].flags = 0L;
      scm_port_table[j].data = UNDEFINED;
      scm_port_table[j].port = UNDEFINED;
    }
  }
  else {
    igc(s_port_table, rootcont);
    for (i = 0; i < scm_port_table_len; i++)
      if (0L==scm_port_table[i].flags) goto ret;
    wta(UNDEFINED, s_nogrow, s_port_table);
  }
 ret:
  NEWCELL(z);
  SETSTREAM(z, stream);
  CAR(z) = (((long)i)<<20) | (flags & 0x0f0000) | ptype;
  scm_port_table[i].unread = EOF;
  scm_port_table[i].flags = flags;
  scm_port_table[i].line = 1L;	/* should both be one-based? */
  scm_port_table[i].col = 1;
  scm_port_table[i].data = UNSPECIFIED;
  scm_port_table[i].port = z;
  return z;
}
