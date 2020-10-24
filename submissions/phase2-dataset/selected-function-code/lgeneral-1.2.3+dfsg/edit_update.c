int edit_update( Edit *edit, int ms )
{
    edit->blink_time += ms;
    if ( edit->blink_time > 500 ) {
        edit->blink_time = 0;
        edit->blink = !edit->blink;
    }
    if ( edit->last_sym != -1 ) {
        if ( !event_check_key( edit->last_sym ) )
            edit->last_sym = -1;
        else {
            edit->delay -= ms;
            if ( edit->delay <= 0 ) {
                edit->delay = 0;
                edit_handle_key( edit, edit->last_sym, edit->last_mod, edit->last_uni );
                return 1;
            }
        }
    }
    return 0;
}