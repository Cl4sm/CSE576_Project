void init_types()
{
  sizet j = num_protects;
  while(j) sys_protects[--j] = UNDEFINED;

  /* We need to set up tmp_errp before any errors may be
     thrown, the port_table index will be zero, usable by
     all ports that don't care about their table entries. */
  tmp_errp = PTR2SCM(CELL_UP(&tmp_errpbuf[0]));
  CAR(tmp_errp) = tc16_fport | OPN | WRTNG;
  /*  CAR(tmp_errp) = scm_port_entry(tc16_fport, OPN|WRTNG); */
  SETSTREAM(tmp_errp, stderr);
  cur_errp = def_errp = sys_safep = tmp_errp;

  /* subrs_gra is trimmed to actual used by scm_init_extensions() */
  scm_init_gra(&subrs_gra, sizeof(subr_info), 420 , 0, "subrs");
  scm_init_gra(&ptobs_gra, sizeof(ptobfuns), 8, 255, "ptobs");
  /* These newptob calls must be done in this order */
  /* tc16_fport = */ newptob(&fptob);
  /* tc16_pipe = */ newptob(&pipob);
  /* tc16_strport = */ newptob(&stptob);
  /* tc16_sfport = */ newptob(&sfptob);
  tc16_clport = newptob(&clptob);
  tc16_sysport = newptob(&sysptob);
  tc16_safeport = newptob(&safeptob);
  scm_init_gra(&smobs_gra, sizeof(smobfuns), 16, 255, "smobs");
  /* These newsmob calls must be done in this order */
  newsmob(&freecell);
  newsmob(&flob);
  newsmob(&bigob);
  newsmob(&bigob);
  scm_init_gra(&finals_gra, sizeof(void (*)()), 4, 0, s_final);
}
