static int setup_data(struct since_state *sn, char *name)
{
  struct data_file *tmp;
  struct stat st;
  int fd, i;
  char *suffix;

  if(sn->s_nozip){
    suffix = strrchr(name, '.');
    if(suffix){
      for(i = 0; ignore_suffix[i]; i++){
        if(!strcmp(suffix, ignore_suffix[i])){
          if(sn->s_verbose > 4){
            fprintf(stderr, "since: not displaying presumed compressed file %s\n", name);
          }
          return 0;
        }
      }
    }
  }

  fd = open(name, O_RDONLY);
  if(fd < 0){
    fprintf(stderr, "since: unable to open %s: %s\n", name, strerror(errno));
    return 1;
  }

  if(fstat(fd, &st)){
    fprintf(stderr, "since: unable to fstat %s: %s\n", name, strerror(errno));
    return 1;
  }

  if(!(S_IFREG & st.st_mode)){
    fprintf(stderr, "since: unable to handle special file %s\n", name);
    return 1;
  }

  tmp = realloc(sn->s_data_files, sizeof(struct data_file) * (sn->s_data_count + 1));
  if(tmp == NULL){
    fprintf(stderr, "since: unable to allocate %u bytes for %s\n", sizeof(struct data_file) * (sn->s_data_count * 1), name);
    return -1;
  }

  sn->s_data_files = tmp;
  tmp = &(sn->s_data_files[sn->s_data_count]);
  sn->s_data_count++;

  tmp->d_name = name;
  tmp->d_fd = fd;

  tmp->d_dev = st.st_dev;
  tmp->d_ino = st.st_ino;

  tmp->d_had = 0;
  tmp->d_now = st.st_size;
  tmp->d_pos = 0;

  tmp->d_write = 0;
  tmp->d_jump = 0;
  tmp->d_deleted = 0;
  tmp->d_replaced = 0;
  tmp->d_moved = 0;
  tmp->d_notable = 1;

  tmp->d_offset = (-1);
  tmp->d_notify = (-1);

  return 0;
}