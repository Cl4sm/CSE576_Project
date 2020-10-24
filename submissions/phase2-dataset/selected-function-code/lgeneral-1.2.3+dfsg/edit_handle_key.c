void edit_handle_key( Edit *edit, int keysym, int modifier, int unicode )
{
    int i, changed = 0;
    switch ( keysym ) {
        case SDLK_RIGHT:
            if ( edit->cursor_pos < strlen( edit->text ) ){
                changed = 1;
                edit->cursor_pos++;
            }
            break;
        case SDLK_LEFT:
            if ( edit->cursor_pos > 0 ) {
                changed = 1;
                edit->cursor_pos--;
            }
            break;
        case SDLK_HOME:
            changed = 1;
            edit->cursor_pos = 0;
            break;
        case SDLK_END:
            changed = 1;
            edit->cursor_pos = strlen( edit->text );
            break;
        case SDLK_BACKSPACE:
            if ( edit->cursor_pos > 0 ) {
                --edit->cursor_pos;
                for ( i = edit->cursor_pos; i < strlen( edit->text ) - 1; i++ )
                    edit->text[i] = edit->text[i + 1];
                edit->text[i] = 0;
                changed = 1;
            }
            break;
        case SDLK_DELETE:
            if ( edit->cursor_pos < strlen( edit->text ) ) {
                for ( i = edit->cursor_pos; i < strlen( edit->text ) - 1; i++ )
                    edit->text[i] = edit->text[i + 1];
                edit->text[i] = 0;
                changed = 1;
            }
            break;
        default:
            /* control commands */
            if ( ( modifier & KMOD_LCTRL ) || ( modifier & KMOD_RCTRL ) ) {
                switch ( keysym ) {
                    case SDLK_u: /* CTRL+U: delete line */
                        edit->text[0] = '\0';
                        edit->cursor_pos = 0;
                        changed = 1;
                        break;
                }
                break;
            }

            if ( unicode >= 32 && edit->cursor_pos < edit->limit && strlen( edit->text ) < edit->limit ) {
                for ( i = edit->limit - 1; i > edit->cursor_pos; i-- )
                    edit->text[i] = edit->text[i - 1];
                edit->text[edit->cursor_pos++] = unicode;
                changed = 1;
            }
        break;
    }
    if ( changed ) {
        edit->delay = keysym != edit->last_sym ? 750 : 100;
        edit_show( edit, 0 );
        edit->last_sym = keysym;
        edit->last_mod = modifier;
        edit->last_uni = unicode;
    }
    else
        edit->last_sym = -1;
}