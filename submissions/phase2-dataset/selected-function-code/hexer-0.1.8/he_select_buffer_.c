  int
he_select_buffer_(buffer)
  struct buffer_s *buffer;
  /* Set `current_buffer' to `buffer'.  The file for `buffer' is loaded if
   * nessecarry.
   */
{
  struct buffer_s *i;
  char swap_template[8];
  
  strcpy(swap_template, ".XXXXXX");
  for (i = buffer_list; i != buffer && i; i = i->next);
  if (!i) return -1;
  current_buffer = i;
  if (!i->loaded_f) {
    i->hedit->buffer = new_buffer(0);
    /* read the file */
    if (b_read_buffer_from_file(i->hedit->buffer, i->path) < 0) {
      delete_buffer(i->hedit->buffer);
      i->hedit->buffer = 0;
      he_close_buffer(0);
      return -1;
    }
    /* check out if we can open a swapfile */
    i->hedit->swapfile =
      (char *)malloc(strlen(hexer_ext) + strlen(i->path) + 1);
    strcpy(i->hedit->swapfile, i->path);
    strcat(i->hedit->swapfile, hexer_ext);
    if (access(i->hedit->swapfile, R_OK)) {
      if (errno == ENOENT) /* swapfile doesn't exist -- fine */
        if (access(i->hedit->swapfile, W_OK)) {
          if (errno == ENOENT) {
            if ((i->hedit->undo.swapfile = fopen(i->hedit->swapfile, "w+")))
              i->hedit->swapping = 1;
          } else
            he_message(0, "@Abno swapfile@~");
        }
    } else {
      /* a swapfile does exist */
      int swapfd;
      he_message(1, "@Abwarning: swapfile@~ `%s' @Abexists@~",
                 i->hedit->swapfile);
      i->hedit->swapfile =
        (char *)realloc(i->hedit->swapfile,
                      strlen(i->hedit->swapfile) + strlen(swap_template) + 1);
        strcat(i->hedit->swapfile, swap_template);
      if ((swapfd = mkstemp(i->hedit->swapfile)) < 0)
        he_message(0, "@Abno swapfile@~");
      else {
        i->hedit->undo.swapfile = fdopen(swapfd, "r+");
        i->hedit->swapping = 1;
        he_message(0, "@Abswapping to@~ `%s'", i->hedit->swapfile);
      }
    }
    if (i->hedit->swapping) {
      /* write the swap-header to the swapfile */
      size_t len;
      char *buf;

      len = 6 + strlen(HEXER_VERSION) + 1 + strlen(i->fullpath) + 1 + 4;
      buf = alloca(len + 1);
      if (snprintf(buf, len + 1, "hexer %s\n%s\n%c%c%c%c",
	  HEXER_VERSION, i->fullpath, 0, 0, 0, 0) != len ||
	  fwrite(buf, 1, len, i->hedit->undo.swapfile) != len ||
	  fflush(i->hedit->undo.swapfile) == EOF) {
	/* TODO: some kind of error output */
	fclose(i->hedit->undo.swapfile);
	i->hedit->swapping = 0;
      }
    }
    i->hedit->buffer->modified = 0;
    i->loaded_f = 1;
    i->visited_f = 1;
  }
  return 0;
}
