void key(int keycode, int charcode, const char *key, const char *chr,
         int shift, int ctrl)
{
    int keyevent = -1;

    if (!strnullcmp(key, "Backspace") || !strnullcmp(key, "Del") ||
        keycode == 8 || keycode == 46) {
        keyevent = 127;                /* Backspace / Delete */
    } else if (!strnullcmp(key, "Enter") || keycode == 13) {
        keyevent = 13;             /* return */
    } else if (!strnullcmp(key, "Left") || keycode == 37) {
        keyevent = CURSOR_LEFT;
    } else if (!strnullcmp(key, "Up") || keycode == 38) {
        keyevent = CURSOR_UP;
    } else if (!strnullcmp(key, "Right") || keycode == 39) {
        keyevent = CURSOR_RIGHT;
    } else if (!strnullcmp(key, "Down") || keycode == 40) {
        keyevent = CURSOR_DOWN;
    } else if (!strnullcmp(key, "End") || keycode == 35) {
        /*
         * We interpret Home, End, PgUp and PgDn as numeric keypad
         * controls regardless of whether they're the ones on the
         * numeric keypad (since we can't tell). The effect of
         * this should only be that the non-numeric-pad versions
         * of those keys generate directions in 8-way movement
         * puzzles like Cube and Inertia.
         */
        keyevent = MOD_NUM_KEYPAD | '1';
    } else if (!strnullcmp(key, "PageDown") || keycode==34) {
        keyevent = MOD_NUM_KEYPAD | '3';
    } else if (!strnullcmp(key, "Home") || keycode==36) {
        keyevent = MOD_NUM_KEYPAD | '7';
    } else if (!strnullcmp(key, "PageUp") || keycode==33) {
        keyevent = MOD_NUM_KEYPAD | '9';
    } else if (chr && chr[0] && !chr[1]) {
        keyevent = chr[0] & 0xFF;
    } else if (keycode >= 96 && keycode < 106) {
        keyevent = MOD_NUM_KEYPAD | ('0' + keycode - 96);
    } else if (keycode >= 65 && keycode <= 90) {
        keyevent = keycode + (shift ? 0 : 32);
    } else if (keycode >= 48 && keycode <= 57) {
        keyevent = keycode;
    }

    if (keyevent >= 0) {
        if (shift && keyevent >= 0x100)
            keyevent |= MOD_SHFT;

        if (ctrl) {
            if (keyevent >= 0x100)
                keyevent |= MOD_CTRL;
            else
                keyevent &= 0x1F;
        }

        midend_process_key(me, 0, 0, keyevent);
        update_undo_redo();
    }
}