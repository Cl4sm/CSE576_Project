static int make_fmt_string(struct since_state *sn)
{
  int result;

  sn->s_fmt_used = 0;
  sn->s_fmt_output = 0;
  sn->s_fmt_prefix = 0;

  result = 0;

  result += make_fmt_field(sn, sn->s_disk_device, sn->s_arch_device, ':');
  result += make_fmt_field(sn, sn->s_disk_inode, sn->s_arch_inode, ':');
  /* meh, we wanted to use \n, but snprintf runs over the edge */
  result += make_fmt_field(sn, sn->s_disk_size, sn->s_arch_size, '\0');

  if(result){
    fprintf(stderr, "since: internal logic failure generating format string\n");
    return -1;
  }

#ifdef DEBUG
  fprintf(stderr, "since: fmt string is %s", sn->s_fmt);
#endif

  sn->s_fmt_prefix = (2 * sn->s_disk_device) + 1 + (2 * sn->s_disk_inode);
  sn->s_fmt_output = sn->s_fmt_prefix + 1 + (2 * sn->s_disk_size); /* WARNING: missing \n, added manually */

  if((sn->s_fmt_output + 1) >= MAX_FMT){
    fprintf(stderr, "since: oversize fields: limit=%d, wanted=%d\n", MAX_FMT, sn->s_fmt_output + 1);
    return -1;
  }

  return 0;
}