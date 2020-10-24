hexer()
{
  int key;
  char *cmd;
  char dfl[256], buf[256];
  long begin, end;
  int anchor, anchor_f = 0;
  int redisplay;

  tio_winch = rl_winch = he_refresh;
  he_refresh_part(current_buffer->hedit, 0, -1);
  for (; buffer_list;) {
    key = he_mainloop(current_buffer->hedit);
    redisplay = 0;
    switch (key) {
    case '!':  /* shell-command. */
      he_cancel_selection(current_buffer->hedit);
      he_update_screen(current_buffer->hedit);
      cmd = he_query_command("!", "", 1);
      redisplay = rl_redisplay;
      if (cmd) {
        strcpy(buf + 1, cmd);
        *buf = '!';
        exh_command(current_buffer->hedit, buf);
      }
      break;
    case ':':  /* exh command. */
      begin = current_buffer->hedit->begin_selection;
      end = current_buffer->hedit->end_selection;
      anchor = current_buffer->hedit->anchor_selection;
      if (begin >= 0 && end >= begin) {
        anchor_f = 1;
	sprintf(dfl, "0x%lx,0x%lx ", begin, end);
      } else {
	*dfl = 0;
        anchor_f = 0;
      }
      he_cancel_selection(current_buffer->hedit);
      cmd = he_query_command(":", dfl, 0);
      redisplay = rl_redisplay;
      if (cmd ? *cmd : 0)
        exh_command(current_buffer->hedit, cmd);
      if (current_buffer && anchor_f)
        if (current_buffer->hedit->begin_selection >= 0)
          current_buffer->hedit->anchor_selection = anchor;
      break;
    case '^' & 0x1f: /* C-^ - switch to alternate buffer */
      he_cancel_selection(current_buffer->hedit);
      if (he_alternate_buffer() < 0) 
	he_message(0, "no alternate buffer");
      else {
	he_refresh_part(current_buffer->hedit, 0, -1);
	tio_ungetch('g' & 0x1f);  /* FIXME */
      }
      break;
    default:
      he_cancel_selection(current_buffer->hedit);
      he_update_screen(current_buffer->hedit);
    }
    if (current_buffer) {
      if (redisplay) he_refresh_all(current_buffer->hedit);
      he_update_screen(current_buffer->hedit);
    }
  }
  return 0;
}
