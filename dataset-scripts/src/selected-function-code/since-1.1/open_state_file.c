static int open_state_file(struct since_state *sn, char *name)
{
  char *ptr;
  int more;
  struct passwd *pw;
  int result;

  if(name){
    return try_state_file(sn, name, NULL, 0);
  }

  ptr = getenv("SINCE");
  if((result = try_state_file(sn, ptr, NULL, 0)) >= 0){
    return result;
  }

  ptr = getenv("HOME");
  if(ptr == NULL){
    pw = getpwuid(getuid());
    if(pw){
      ptr = pw->pw_dir;
    }
  }
  if((result = try_state_file(sn, ptr, ".since", 0)) >= 0){
    return result;
  }

#ifdef O_NOFOLLOW
  more = O_NOFOLLOW;
#else
  more = 0;
#endif
  if((result = try_state_file(sn, SINCE_FALLBACK, NULL, more)) >= 0){
    if(sn->s_verbose > 0){
      fprintf(stderr, "since: using fallback %s\n", SINCE_FALLBACK);
    }
    return result;
  }

  fprintf(stderr, "since: unable to open any state file\n");
  return -1;
}