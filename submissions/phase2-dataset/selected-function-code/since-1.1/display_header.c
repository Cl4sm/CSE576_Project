static int display_header(struct since_state *sn, struct data_file *df, int single, int chuck)
{
  off_t delta;
  unsigned int value, z;
  int nada;
  char *suffixes[] = { "b", "kb", "Mb", "Gb", "Tb", NULL} ;

  switch(sn->s_verbose){
    case 0 : return 0;
    case 1 : if(single) return 0; /* WARNING: else fall */
    case 2 : if(df->d_notable == 0) return 0;
  }

  df->d_notable = 0;

  /* possibly do unlocked io here */
  fprintf(sn->s_header, "==> %s ", df->d_name);

  nada = 1;

  if(df->d_deleted){
    fprintf(sn->s_header, "[deleted] ");
    nada = 0;
  } else if(df->d_moved){
    fprintf(sn->s_header, "[moved] ");
    nada = 0;
  }

#if 0
  if(df->d_replaced){
    /* we know about this, but won't say anything (yet) */
    fprintf(sn->s_header, "[replaced]");
  }
#endif

  if(df->d_pos != df->d_now){
    if(chuck){
      fprintf(sn->s_header, "[discarded] ");
    }
    if(sn->s_verbose > 2){
      delta = df->d_now - df->d_pos;
      for(z = 0; (suffixes[z + 1]) && (delta > 9999); z++){
        delta /= 1024;
      }
      value = delta;
      fprintf(sn->s_header, "(+%u%s) ", value, suffixes[z]);
    }
    nada = 0;
  }

  if(nada){
    fprintf(sn->s_header, "[nothing new] ");
  }

  fprintf(sn->s_header, "<==\n");
  fflush(sn->s_header);

  return 0;
}