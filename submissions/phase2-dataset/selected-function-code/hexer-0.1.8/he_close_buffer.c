he_close_buffer(name)
  char *name;
  /* Close the buffer named `name'. If `name == 0', the current buffer
   * is closed.  The return value is 0 if all goes well, 1 if the named
   * buffer doesn't exist and -1 if the `buffer_list' is empty.
   */
{
  struct buffer_s *i, *j;
  int empty = 0;
  int buffer_switched = 0;

  if (!buffer_list) return -1;
  if (!name)
    i = current_buffer;
  else {
    for (i = buffer_list; i; i = i->next)
      if (!strcmp(i->hedit->buffer_name, name)) break;
    if (!i) return -1;
  }
  if (i != buffer_list) {
    for (j = buffer_list; j->next != i; j = j->next);
    if (alternate_buffer
        ? !strcmp(alternate_buffer, i->hedit->buffer_name) : 0)
      alternate_buffer = 0;
    j->next = i->next;
    if (i == current_buffer) {
      if (i->next)
        he_select_buffer_(i->next);
      else
        he_select_buffer_(j);
      buffer_switched = 1;
    }
  } else
    if (!(buffer_list = current_buffer = i->next))
      empty = 1;
    else {
      he_select_buffer_(buffer_list);
      buffer_switched = 1;
    }
  if (i->hedit->buffer_name) free((char *)i->hedit->buffer_name);
  if (i->hedit->buffer) delete_buffer(i->hedit->buffer);
  if (i->hedit->swapping) {
    fclose(i->hedit->undo.swapfile);
    unlink(i->hedit->swapfile);
  } else
    if (i->hedit->undo.list) he_free_command(i->hedit->undo.list);
  free((char *)i->hedit);
  if (i->path) free((char *)i->path);
  free((char *)i);
  if (empty) buffer_list = 0, current_buffer = 0;
  if (buffer_switched) {
    alternate_buffer = 0;
    he_refresh_all(current_buffer->hedit);
  }
  return empty ? -1 : 0;
}
