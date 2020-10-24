  static void
he_visual_mode(hedit)
  struct he_s *hedit;
{
  int key;
  long anchor = hedit->position;
  long last_position = hedit->position;
  long count = -1;
  int f = 0;
  char map_string[256];
  int motion_f = 0;

  if (!hedit->buffer->size) {
    he_message(1, "@Abthe buffer is empty@~");
    return;
  }
  if (hedit->position == hedit->buffer->size) {
    --hedit->position;
    he_update_screen(hedit);
  }
  tio_set_cursor(0); /* cursor invisible. */
  if (hedit->begin_selection >= 0
      && hedit->end_selection >= hedit->begin_selection) {
    if (hedit->anchor_selection) {
      last_position = hedit->position = hedit->begin_selection;
      anchor = hedit->end_selection;
    } else {
      last_position = hedit->position = hedit->end_selection;
      anchor = hedit->begin_selection;
    }
    motion_f = 1;
  } else {
    he_begin_selection(hedit);
    he_end_selection(hedit);
    he_message(0, "visual");
  }
  tio_ungetch(0);
  for (;;) {
    key = tio_mgetch(MAP_VISUAL, map_string);
    switch (key) {
    _HE_CASE_COMMAND_NO_V:
    HE_CASE_COMMAND_INSERT:
      he_command(hedit, key, count);
      /* If the command cleared the selection, we'll leave the visual
       * mode.
       */
      if (hedit->begin_selection < 0) goto exit_visual_mode;
      /* fall through */
    HE_CASE_MOTION:
    HE_CASE_MOTION_HJKL:
      he_motion(hedit, key, count);
      if (hedit->position == hedit->buffer->size) --hedit->position;
      he_update_screen(hedit);
      if (hedit->position == last_position) break;
      if (anchor > hedit->position) {
        if (anchor < last_position) {
          long x = hedit->position;
          hedit->position = anchor;
          he_end_selection(hedit);
          hedit->position = x;
          he_update_screen(hedit);
        }
        he_begin_selection(hedit);
      } else if (anchor == hedit->position) {
        he_cancel_selection(hedit);
        he_begin_selection(hedit);
        he_end_selection(hedit);
      } else {
        if (anchor > last_position) he_select(hedit, anchor, anchor);
        he_end_selection(hedit);
      }
      last_position = hedit->position;
      motion_f = 1;
      break;
    HE_CASE_MOTION_SHIFT:
      he_motion(hedit, key, count);
      break;
    case ':':
      tio_ungetch(':');
      hedit->anchor_selection = hedit->begin_selection == hedit->position;
      goto exit_visual_mode2;
    case 'v':
    case HXKEY_ESCAPE:
      goto exit_visual_mode;
    case HXKEY_ERROR:
      if (window_changed) he_refresh_screen(hedit);
      if (strlen(map_string)) {
        tio_move(hx_lines, 40);
        tio_raw_printf("%s", map_string);
      }
      break;
    default:
      if (key > 0xff) break;
      if (isdigit(key)) {
        tio_ungetch(key);
        count = he_get_counter(hedit);
        continue;
      }
    }
    he_clear_get(1);
    count = -1;
    he_update_screen(hedit);
    tio_goto_line(hx_lines - 1);
    tio_return();
    if (motion_f) {
      motion_f = 0;
      tio_clear_to_eol();
      tio_printf(" visual selection:  0x%08lx - 0x%08lx  0x%lx (%li) bytes@r",
                 hedit->begin_selection, hedit->end_selection,
                 hedit->end_selection - hedit->begin_selection + 1,
                 hedit->end_selection - hedit->begin_selection + 1);
    }
    tio_flush();
  }

exit_visual_mode:
  he_cancel_selection(hedit);
exit_visual_mode2:
  tio_set_cursor(1); /* normal cursor. */
  f = hedit->refresh.message_f;
  he_update_screen(hedit);
  if (!f) he_message(0, "");
}
