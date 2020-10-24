  int
he_open_buffer(name, path)
  char *name;
  char *path;
{
  struct buffer_s *buffer;
  int no_file_f = 0, read_only = 0;
  struct buffer_s *i;
  FILE *fp;
  char cwd[PATH_MAX + 1];

  if (path) {
    /* check out the read/write permissions */
    if (access(path, R_OK))
      switch (errno) {
      case ENOENT: /* file doesn't exist */
        no_file_f = 1;
        break;
      default:
        he_message(1, "`%s': @Ab%s@~", path, strerror(errno));
        return -1;
      }
    if (!no_file_f ? access(path, W_OK) : 0)
      switch (errno) {
      case EACCES: /* write permission denied */
        read_only = 1;
        break;
      default:
        he_message(1, "`%s': @Ab%s@~", path, strerror(errno));
        return -1;
      }
  }
  *(buffer = (struct buffer_s *)malloc(sizeof(struct buffer_s))) = NO_BUFFER;
  buffer->hedit = (struct he_s *)malloc(sizeof(struct he_s));
  memset(buffer->hedit, 0, sizeof (struct he_s));
  buffer->hedit->begin_selection = -1;
  buffer->hedit->end_selection = -1;
  buffer->hedit->insert_position = -1;
  buffer->hedit->buffer_name = strdup(name);
  if (path && !no_file_f) {
    if (!(fp = fopen(path, "r"))) {
      he_message(1, "`%s': @Ab%s@~", path, strerror(errno));
      free((char *)buffer->hedit->buffer_name);
      free((char *)buffer->hedit);
      free((char *)buffer);
      return -1;
    } else
      fclose(fp);
  } else {
    buffer->hedit->buffer = new_buffer(0);
    buffer->loaded_f = 1;
    buffer->visited_f = 1;
  }
  if (path) {
    buffer->path = strdup(path);
    if (!getcwd(cwd, PATH_MAX)) {
      he_message(0, "@Abcan't get cwd: %s@~", strerror(errno));
      buffer->fullpath = strdup(path);
    } else {
      buffer->fullpath =
        (char *)malloc(strlen(path) + strlen(cwd) + 2);
      sprintf(buffer->fullpath, "%s/%s", cwd, path);
    }
  }
  buffer->hedit->read_only = read_only;
  if (!buffer_list)
    current_buffer = buffer_list = buffer;
  else {
    for (i = buffer_list; i->next; i = i->next);
    i->next = buffer;
  }
  return 0;
}
