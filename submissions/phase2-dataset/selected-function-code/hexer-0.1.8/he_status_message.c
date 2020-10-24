  void
he_status_message(verbose)
  int verbose;
  /* display name and size of the current buffer.  if `verbose' is set,
   * the current position is also displayed.
   */
{
  struct he_s *hedit = current_buffer->hedit;

  if (hedit->buffer->size) {
    if (verbose) {
      he_message(0, "\"%s\" %s%sat 0x%lx (%li) of 0x%lx (%li) bytes  (%li %%)",
                 hedit->buffer_name,
                 hedit->buffer->modified ? "[modified] " : "",
                 hedit->read_only ? "[readonly] " : "",
                 hedit->position, hedit->position,
                 hedit->buffer->size, hedit->buffer->size,
                 (hedit->position * 100) / hedit->buffer->size);
    } else {
    if (hedit->buffer->size)
      he_message(0, "\"%s\" %s%s0x%lx (%li) bytes",
                 hedit->buffer_name,
                 hedit->buffer->modified ? "[modified] " : "",
                 hedit->read_only ? "[readonly] " : "",
                 hedit->buffer->size, hedit->buffer->size);
  else
    he_message(0, "\"%s\" %s%s(empty)", hedit->buffer_name,
               hedit->buffer->modified ? "[modified] " : "",
               hedit->read_only ? "[readonly] " : "");
    }
  }
}
