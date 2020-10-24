static int make_fmt_field(struct since_state *sn, int disk, int arch, int suffix)
{
  int have, pad, i, result;

  have = MAX_FMT - sn->s_fmt_used;

  if(disk < arch){
    return -1;
  }

  if(disk > arch){
    pad = (disk - arch) * 2;
    if(pad > have){
      return -1;
    }
    for(i = 0; i < pad; i++){
      sn->s_fmt[sn->s_fmt_used++] = '0';
    }
    have -= pad;
  }

  for(i = 0; fmt_table[i].f_bytes > 0; i++){
    if(fmt_table[i].f_bytes == arch){
      result = snprintf(sn->s_fmt + sn->s_fmt_used, have, "%%0%d%s%c", arch * 2, fmt_table[i].f_string, suffix);
      if((result < 0) || (result >= have)){
        return -1;
      }

      sn->s_fmt_used += result;
      return 0;
    }
  }

  return -1;
}