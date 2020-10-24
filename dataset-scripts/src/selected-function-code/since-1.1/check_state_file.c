static int check_state_file(struct since_state *sn)
{
  int i, x, w, d, sep[3];

  sep[0] = ':';
  sep[1] = ':';
  sep[2] = '\n';

  if((sn->s_buffer == NULL) || (sn->s_size == 0)){
    if(sn->s_verbose > 2){
      fprintf(stderr, "since: will not check an empty or nonexistant file\n");
    }
    return 1;
  }

  w = 0;

  for(i = 0, x = 0; (i < sn->s_size) && (x < 3); i++){
    if(isxdigit(sn->s_buffer[i])){
      /* nothing */
    } else if(sn->s_buffer[i] == sep[x]){
      d = (i - w);
      if(d % 2){
        fprintf(stderr, "since: data field has to contain an even number of bytes, not %d\n", d);
        return -1;
      }
      d = d / 2;
#ifdef DEBUG
      fprintf(stderr, "check: field[%d] is %d bytes\n", x, d);
#endif
      switch(x){
        case 0 : sn->s_disk_device = d; break;
        case 1 : sn->s_disk_inode = d; break;
        case 2 : sn->s_disk_size = d; break;
      }
      x++;
      w = i + 1;
    } else {
      fprintf(stderr, "since: corrupt state file %s at %d\n", sn->s_name, i);
      return -1;
    }
  }

  if(x < 3){
    fprintf(stderr, "since: no fields within %d bytes in file %s\n", i, sn->s_name);
    return -1;
  }

  return 0;
}