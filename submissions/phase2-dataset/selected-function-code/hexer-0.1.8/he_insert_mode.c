  static void
he_insert_mode(hedit, replace_mode, count)
  struct he_s *hedit;
  int replace_mode;
  long count;
  /* `replace_mode == 0':  Normal insert mode.
   * `replace_mode == 1':  Replace mode.  Characters are appended if the
   *                       end of the buffer is reached.
   * `replace_mode == 2':  Replace a single character.
   */
{
  Buffer *insert = new_buffer(0);
  Buffer *replace = new_buffer(0);
  char *data;
  int insert_state = 0;
  int insert_write = 0;
  unsigned char hx_insert_character;
  long position = hedit->position;
  long size = hedit->buffer->size;
  long i;
  int key, x = 0;
  char map_string[256];

  if (!count) count = 1;
  if (he_refresh_check(hedit)) tio_ungetch(HXKEY_NONE);
  for (;;) {
    key = tio_mgetch(MAP_INSERT, map_string);
    switch (key) {
    HE_CASE_MOTION:
      if (!(count > 2 && replace_mode == 2) && !insert_state) {
        i = hedit->position;
        he_motion(hedit, key, count);
        if (i != hedit->position) count = 1;
          /* Moving the cursor eliminates the counter.
           */
        if (i != hedit->position && insert->size) {
          data = (char *)malloc(insert->size);
          b_read(insert, data, 0, insert->size);
          if (replace_mode && replace->size) {
            char *data_replace;
            data_replace = (char *)malloc(replace->size);
            b_read(replace, data_replace, 0, replace->size);
            he_subcommand(hedit, 0, position, replace->size, data_replace);
            b_clear(replace);
          }
          he_subcommand(hedit, 1, position, insert->size, data);
          he_subcommand(hedit, -1, 0, 0, 0);
          b_clear(insert);
        }
        position = hedit->position;
      }
      break;
    HE_CASE_COMMAND_INSERT:
      he_command(hedit, key, -1);
      break;
    case HXKEY_ERROR:
      if (window_changed) he_refresh_screen(hedit);
      if (strlen(map_string)) {
        tio_move(hx_lines, 40);
        tio_raw_printf("%s", map_string);
      }
      break;
    case HXKEY_BACKSPACE:
    case HXKEY_DELETE:
      if (replace_mode)
        if (hedit->text_mode)
          if (insert->size) {
            --hedit->position;
            if (hedit->position >= size)
              b_set_size(hedit->buffer, hedit->position);
            else {
              b_copy(hedit->buffer, replace,
                     hedit->position, replace->size - 1, 1);
              b_set_size(replace, replace->size - 1);
            }
            b_set_size(insert, insert->size - 1);
            he_refresh_part(hedit, hedit->position, hedit->position);
          } else
            tio_bell();
        else /* (!hedit->text_mode) */
          if (insert_state) {
            insert_state = 0;
            hedit->insert_position = -1;
            b_copy(hedit->buffer, replace,
                   hedit->position, replace->size - 1, 1);
            if (replace->size > 0)
                b_set_size(replace, replace->size - 1);
            if (insert->size > 0)
                b_set_size(insert, insert->size - 1);
            he_refresh_part(hedit, hedit->position, hedit->position);
          } else
            if (insert->size) {
              insert_state = 1;
              b_read(insert, (char *)&hx_insert_character, insert->size - 1, 1);
              hx_insert_character &= (char)0xf0;
              x = (unsigned char)hx_insert_character;
              key = (unsigned char)hx_insert_character >> 4;
              --hedit->position;
              hedit->insert_position = hedit->position;
              he_refresh_part(hedit, hedit->position, hedit->position);
            } else
              tio_bell();
      else /* (!replace_mode) */
        if (hedit->text_mode)
          if (insert->size) {
            --hedit->position;
            b_delete(hedit->buffer, hedit->position, 1);
            b_set_size(insert, insert->size - 1);
            he_refresh_part(hedit, hedit->position, hedit->buffer->size - 1);
          } else
            tio_bell();
        else
          if (insert_state) {
            insert_state = 0;
            hedit->insert_position = -1;
            b_delete(hedit->buffer, hedit->position, 1);
            if (insert->size > 0)
              b_set_size(insert, insert->size - 1);
            he_refresh_part(hedit, hedit->position, hedit->buffer->size - 1);
          } else
            if (insert->size) {
              insert_state = 1;
              b_read(insert, (char *)&hx_insert_character, insert->size - 1, 1);
              hx_insert_character &= (char)0xf0;
              x = (unsigned char)hx_insert_character >> 4;
              --hedit->position;
              hedit->insert_position = hedit->position;
              he_refresh_part(hedit, hedit->position, hedit->position);
            } else
              tio_bell();
      break;
    case HXKEY_ESCAPE:
      goto hx_exit_insert_mode;
        /* The refresh has to be performed by the calling function.
         */
    case 'v' & 0x1f: /* C-v */
      if (hedit->text_mode) key = he_verbatim(hedit);
      goto Default;
    case HXKEY_RETURN:
      if (key == (int)HXKEY_RETURN) key = 0x0a;
      /* fall through */
    default: Default:
      if (!key || key > 0xff) break;
      if (key == (int)HXKEY_NULL) key = 0;
      insert_write = 0;
      if (hedit->text_mode) {
        hx_insert_character = (unsigned char)key;
        if (!replace_mode) {
          b_insert(hedit->buffer, hedit->position, 1);
          he_refresh_part(hedit, hedit->position, hedit->buffer->size - 1);
        } else
          he_refresh_part(hedit, hedit->position, hedit->position);
        insert_write = 1;
      } else {
        x = (unsigned char)tolower(key);
        if (isdigit(x) || (x >= 'a' && x <= 'f')) {
          x -= isdigit(x) ? '0' : ('a' - 0xa);
          if (!insert_state) {
            hx_insert_character = (unsigned char)x << 4;
            if (!replace_mode) {
              b_insert(hedit->buffer, hedit->position, 1);
              hedit->insert_position = hedit->position;
              he_refresh_part(hedit, hedit->position, hedit->buffer->size - 1);
            } else
              he_refresh_part(hedit, hedit->position, hedit->position);
          } else {
            hx_insert_character |= (unsigned char)x;
            insert_write = 1;
            hedit->insert_position = -1;
            he_refresh_part(hedit, hedit->position, hedit->position);
          }
          insert_state = !insert_state;
        } else
          continue;
      }
      if (insert_write) {
        if (replace_mode) {
          if (hedit->position < size) {
            char c;
            b_read(hedit->buffer, &c, hedit->position, 1);
            b_append(replace, &c, 1);
          }
        }
        b_write_append(hedit->buffer, (char *)&hx_insert_character, hedit->position, 1);
        b_append(insert, (char *)&hx_insert_character, 1);
        if (replace_mode == 2) goto hx_exit_insert_mode;
        ++hedit->position;
      }
    } /* switch */
    if (he_clear_get(0)) continue;
    if (!he_update_screen(hedit) && replace_mode != 2) {
      if (replace_mode)
        he_message(0, "replace");
      else
        he_message(0, "insert");
      he_update_screen(hedit);
    }
    if (insert_state) tio_printf("%x", (unsigned)x);
  } /* for */

hx_exit_insert_mode:
  if (insert->size) {
    char *data_replace;
    long c;
    if (count > 1) {
      /* If there's a counter > 1, we'll undo the changes made and then
       * redo them `count' times.  This way it is easier to create a single
       * entry in the undo list.
       */
      long replace_size2 = 0;
      b_delete(hedit->buffer, position, insert->size);
      if (replace_mode) {
        b_insert(hedit->buffer, position, replace->size);
        b_copy(hedit->buffer, replace, position, 0, replace->size);
        data_replace = (char *)malloc(replace->size * count);
        replace_size2 =
          b_read(hedit->buffer, data_replace, position, replace->size * count);
        if (replace_size2 < replace->size * count)
          /* We don't want to eat up more memory than needed.
           */
          data_replace = (char *)realloc(data_replace, replace_size2);
        b_delete(hedit->buffer, position, replace_size2);
        he_subcommand(hedit, 0, position, replace_size2, data_replace);
      }
      data = (char *)malloc(insert->size);
      b_read(insert, data, 0, insert->size);
      data = (char *)realloc(data, insert->size * count);
      for (c = insert->size * (count - 1); c > 0; c -= insert->size)
        memcpy(data + c, data, insert->size);
      b_insert(hedit->buffer, position, insert->size * count);
      b_write(hedit->buffer, data, position, insert->size * count);
      he_subcommand(hedit, 1, position, insert->size * count, data);
      
      if (replace_mode) {
        he_refresh_part(hedit, hedit->position,
                        hedit->position + replace_size2);
      } else
        he_refresh_part(hedit, hedit->position, hedit->buffer->size - 1);
    } else {
      data = (char *)malloc(insert->size);
      b_read(insert, data, 0, insert->size);
      if (replace_mode)
        if (replace->size) {
          data_replace = (char *)malloc(replace->size);
          b_read(replace, data_replace, 0, replace->size);
          he_subcommand(hedit, 0, position, replace->size, data_replace);
        }
      he_subcommand(hedit, 1, position, insert->size, data);
      if (insert_state && !replace_mode) {
        b_delete(hedit->buffer, hedit->position, 1);
        hedit->insert_position = -1;
        he_refresh_part(hedit, hedit->position, hedit->buffer->size - 1);
      } else
        he_refresh_part(hedit, hedit->position, hedit->position);
    }
    he_subcommand(hedit, -1, 0, 0, 0);
  }
  delete_buffer(insert);
  delete_buffer(replace);
  tio_goto_line(hx_lines - 1);
  tio_return();
  tio_clear_to_eol();
}
