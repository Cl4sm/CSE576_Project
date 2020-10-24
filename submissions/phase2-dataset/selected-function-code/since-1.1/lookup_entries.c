static int lookup_entries(struct since_state *sn)
{
  char line[MAX_FMT], *end;
  int i, j, result;
  struct data_file *df;

  if((sn->s_buffer == NULL) || (sn->s_size == 0)){ /* file empty, nothing to look up */
    return 0;
  }

  /* could sort/index stuff to do better than O(n^2) */
  for(i = 0; i < sn->s_data_count; i++){
    df = &(sn->s_data_files[i]);
    result = snprintf(line, MAX_FMT, sn->s_fmt, df->d_dev, df->d_ino, 0);
    if(result != sn->s_fmt_output){
      fprintf(stderr, "since: logic problem: expected state line to be %d bytes, printed %d\n", sn->s_fmt_output, result);
      return -1;
    }
    for(j = 0; j < sn->s_size; j += (sn->s_fmt_output + 1)){
      if(!memcmp(sn->s_buffer + j, line, sn->s_fmt_prefix)){
        df->d_offset = j;
        if(sn->s_arch_size > sizeof(unsigned long)){
          df->d_had = strtoull(sn->s_buffer + j + sn->s_fmt_prefix + 1, &end, 16);
        } else {
          df->d_had = strtoul(sn->s_buffer + j + sn->s_fmt_prefix + 1, &end, 16);
        }
        if(end[0] != '\n'){
          fprintf(stderr, "since: parse problem: unable to convert value at offset %d to number\n", j + sn->s_fmt_prefix + 1);
          return -1;
        }

        if(df->d_had > df->d_now){
          fprintf(stderr, "since: considering %s to be truncated, displaying from start\n", df->d_name);
          df->d_had = 0;
          df->d_write = 1;
        }

        if(df->d_pos != df->d_had){
          /* pos is the value which gets saved */
          df->d_pos = df->d_had;
          df->d_jump = 1;
        }

        if(sn->s_verbose > 3){
          /* this seems a bit risky, what if longs are bigger than wordsize ? */
#if _FILE_OFFSET_BITS > __WORDSIZE
          fprintf(stderr, "since: found record for %s at offset %d, now=%Lu, had=%Lu\n", df->d_name, j, df->d_now, df->d_had);
#else
          fprintf(stderr, "since: found record for %s at offset %d, now=%ld, had=%ld\n", df->d_name, j, df->d_now, df->d_had);
#endif
        }
        break;
      }
    }
  }

  return 0;
}