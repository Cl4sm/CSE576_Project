static int tmp_state_file(struct since_state *sn, int (*call)(struct since_state *sn))
{
  char canon[PATH_MAX], tmp[PATH_MAX], *tptr;
  int result, tfd, nfd, mode, flags;

  if(realpath(sn->s_name, canon) == NULL){
    fprintf(stderr, "since: unable to establish true location of %s: %s\n", sn->s_name, strerror(errno));
    return -1;
  }

  result = snprintf(tmp, PATH_MAX, "%s.%d", canon, getpid());
  if((result < 0) || (result >= PATH_MAX)){
    fprintf(stderr, "since: tmp file of %s exeeded limits\n", canon);
    return -1;
  }

  if(sn->s_verbose > 1){
    fprintf(stderr, "since: creating tmp file %s\n", tmp);
  }

  mode = SINCE_MASK;
  flags = O_RDWR | O_CREAT | O_EXCL;
#ifdef O_NOFOLLOW
  flags |= O_NOFOLLOW;
#endif

  nfd = open(tmp, flags, mode);
  if(nfd < 0){
    fprintf(stderr, "since: unable to create tmp file %s: %s\n", tmp, strerror(errno));
    return -1;
  }

  tfd = sn->s_fd;
  tptr = sn->s_name;

  sn->s_fd = nfd;
  sn->s_name = tmp;

  result = (*call)(sn);

  sn->s_fd = tfd;
  sn->s_name = tptr;

  if(result < 0){
    close(nfd);
    unlink(tmp);
  }

  if(rename(tmp, sn->s_name)){
    fprintf(stderr, "since: unable to rename %s to %s: %s\n", tmp, sn->s_name, strerror(errno));
    close(nfd);
    unlink(tmp);
    return -1;
  }

  sn->s_fd = nfd;
  close(tfd);

  return 0;
}