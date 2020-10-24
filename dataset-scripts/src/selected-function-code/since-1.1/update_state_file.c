static int update_state_file(struct since_state *sn)
{
  /* WARNING: this invalidates the structure, forces a forget */
  char *tmp, *target;
  int i, j, result, redo, changed;
  struct data_file *df;

  if(sn->s_readonly){
    if(sn->s_verbose > 2){
      fprintf(stderr, "since: readonly, not updatating %s\n", sn->s_name);
    }
    return 1;
  }

  changed = 0;
  redo = sn->s_atomic;

  for(i = 0; i < sn->s_data_count; i++){
    df = &(sn->s_data_files[i]);
    if(df->d_write){
      changed = 1;
      if(df->d_offset < 0){
        /* awkwardly block duplicate entries */
        for(j = sn->s_data_count - 1; j > i; j--){
          if((sn->s_data_files[j].d_ino == df->d_ino) &&
             (sn->s_data_files[j].d_dev == df->d_dev) &&
             (sn->s_data_files[j].d_offset < 0) &&
             sn->s_data_files[j].d_write){
            break;
          }
        }
        if(i != j){
          continue; /* will append later, don't do it for this entry */
        }
        tmp = realloc(sn->s_append, sn->s_add + sn->s_fmt_output + 1);
        if(tmp == NULL){
          fprintf(stderr, "since: unable to allocate %d bytes for append buffer \n", sn->s_add + sn->s_fmt_output + 1);
          return -1;
        }
        sn->s_append = tmp;
        target = sn->s_append + sn->s_add;
        sn->s_add += sn->s_fmt_output + 1;
      } else {
        target = sn->s_buffer + df->d_offset;
        redo = sn->s_ismap ? sn->s_atomic : 1; /* rewrite file completely if not mmaped and we modify existing entries */
      }

      result = snprintf(target, sn->s_fmt_output + 1, sn->s_fmt, df->d_dev, df->d_ino, df->d_pos);
      target[sn->s_fmt_output] = '\n'; /* replace \0 with newline */
      if(result != sn->s_fmt_output){
        fprintf(stderr, "since: logic failure: wanted to print %d, got %d\n", sn->s_fmt_output, result);
        return -1;
      }
    }
  }

  if(changed){
    if(redo){
      result = tmp_state_file(sn, &internal_update_state_file);
    } else {
      result = append_state_file(sn);
    }
  } else {
    result = 0;
  }

  forget_state_file(sn);

  return result;
}