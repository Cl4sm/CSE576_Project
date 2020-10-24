char* get_map_name(pid_t pid)
{
  FILE *fd;
  char mfile[64];
  char mfield[5][25];
  char mbuf[PATH_MAX+8+(25*5)+1];
  static char map_name[PATH_MAX+8];
 
  snprintf(mfile, sizeof mfile, "/proc/%d/maps", pid);

  if ((fd = fopen(mfile, "r")) == NULL)
    return NULL;

  for (;;) {

    memset(map_name, '\0', sizeof map_name);
    memset(mbuf, '\0', sizeof mbuf);
 
    if (fgets(mbuf, sizeof mbuf, fd) == NULL)
      return NULL;

    sscanf(mbuf, "%24s %24s %24s %24s %24s %s", mfield[0], mfield[1],
       mfield[2], mfield[3], mfield[4], map_name);

    if (!map_name) {
      /* return NULL; */
      continue;
    }

    if (!strncmp(mfield[0], get_map_exec_offset(), 8))
      return map_name;

  }

  return NULL;
}