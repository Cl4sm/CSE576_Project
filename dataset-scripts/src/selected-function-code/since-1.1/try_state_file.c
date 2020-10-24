static int try_state_file(struct since_state *sn, char *path, char *append, int more)
{
  int flags, mode, plen, alen, result;
  char *tmp;

  if(path == NULL){
    return -1;
  }

  if(append){
    plen = strlen(path);
    alen = strlen(append);
    tmp = malloc(plen + alen + 2);
    if(tmp == NULL){
      fprintf(stderr, "since: unable to allocate %d bytes\n", plen + alen + 2);
      return -1;
    }
    if((plen > 0) && (path[plen - 1] == '/')){
      plen--;
    }
    strcpy(tmp, path);
    tmp[plen] = '/';
    strcpy(tmp + plen + 1, append);
  } else {
    tmp = strdup(path);
    if(tmp == NULL){
      fprintf(stderr, "since: unable to duplicate %s\n", path);
      return -1;
    }
  }
  sn->s_name = tmp;

  flags = (sn->s_readonly ? O_RDONLY : (O_RDWR | O_CREAT)) | more;

  mode = SINCE_MASK;

  if(sn->s_verbose > 2){
    fprintf(stderr, "since: attempting to open %s\n", sn->s_name);
  }

  sn->s_fd = open(sn->s_name, flags, mode);
  if(sn->s_fd >= 0){
    if(sn->s_verbose > 1){
      fprintf(stderr, "since: opened %s\n", sn->s_name);
    }
    return 0;
  }

  result = -1;

  if(errno == ENOENT){
    if(sn->s_readonly){
      result = 1;
    }
  }

  if(result < 0){
    fprintf(stderr, "since: unable to open %s: %s\n", sn->s_name, strerror(errno));
  }

  free(sn->s_name);
  sn->s_name = NULL;

  return result;
}