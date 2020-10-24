  static int
he_command(struct he_s *hedit, int key, long count)
  /* Execute a command-mode command.  If `key' is an unknown command,
   * the return value is 0.  If `key' is a quit command, the return value
   * is -1.  In all other cases the return value is 1.
   */
{
  int rval = 1;
  int redisplay = 0;
  char *cmd;

  he_update_screen(hedit);
  switch (key) {
  case 'i': /* enter insert mode */
    he_cancel_selection(hedit);
    he_message(0, "insert at position 0x%lx (%li)", hedit->position,
               hedit->position);
    he_insert_mode(hedit, 0, count);
    break;
  case 'R': /* enter replace mode */
    he_cancel_selection(hedit);
    he_message(0, "replace at position 0x%lx (%li)",
               hedit->position, hedit->position);
    he_insert_mode(hedit, 1, count);
    break;
  case 'r': /* replace `count' characters */
    if (hedit->begin_selection >= 0
        && hedit->end_selection >= hedit->begin_selection) {
      hedit->position = hedit->begin_selection;
      he_update_screen(hedit);
      count = hedit->end_selection - hedit->begin_selection + 1;
    }
    if (count > 0)
      he_message(0, "replace 0x%lx (%li) bytes at position 0x%lx (%li)",
                 count, count, hedit->position, hedit->position);
    else
      he_message(0, "replace byte at position 0x%lx (%li)",
                 hedit->position, hedit->position);
    he_insert_mode(hedit, 2, count);
    hedit->begin_selection = -1;
    break;
  case 'u': /* undo */
    if (count < 0) count = 1;
    while (--count) {
      he_undo(hedit);
      he_update_screen(hedit);
    }
    he_undo(hedit);
    break;
  case 'r' & 0x1f: /* C-r, redo */
    if (count < 0) count = 1;
    while (--count) {
      he_redo(hedit);
      he_update_screen(hedit);
    }
    he_redo(hedit);
    break;
  case 'v': /* enter visual mode for selecting text */
    he_visual_mode(hedit);
    break;
  case 'G': /* goto */
    if (count < 0)
      hedit->position = hedit->buffer->size;
    else {
      if (count > hedit->buffer->size) {
        count = hedit->buffer->size;
        he_message(0, "only 0x%lx (%li) bytes", count, count);
      }
      hedit->position = count;
    }
    break;
  case 'x' & 0x1f: /* C-x, delete (cut) */
    he_delete(hedit, count, 0);
    break;
  case 'x': /* delete */
    he_delete(hedit, count, 1);
    break;
  case 'y': /* yank */
    he_yank(hedit, count);
    break;
  case 'o' & 0x1f: /* paste over */
    he_paste_over(hedit, count);
    break;
  case 'p': /* paste */
    he_paste(hedit, count);
    break;
  case '.': /* again */
    he_again(hedit, hedit->position);
    break;
  case 'g' & 0x1f: /* C-g */
    he_status_message(1);
    break;
  case '/':  /* regex search forward. */
    cmd = he_query_command("/", "", 2);
    redisplay = rl_redisplay;
    if (cmd) he_search_command(hedit, cmd, 1);
    break;
  case '?':  /* regex search backward. */
    cmd = he_query_command("?", "", 2);
    if (cmd) he_search_command(hedit, cmd, -1);
    break;
  case '%':  /* calculator. */
    cmd = he_query_command("%", "", 3);
    he_message(0, "calc: %s", calculator(cmd));
    redisplay = rl_redisplay;
    break;
  case 'l' & 0x1f: /* C-l */
    he_refresh_screen(hedit);
    break;
  default:
    rval = 0;
  }
  if (redisplay) he_refresh_all(hedit);

  return rval;
}
