static void control(unsigned int btn, int x, int y, unsigned int state) {
    switch (btn) {
        case 1:
            if (time_mode == CLOCK) {
                if ( (x >= style.aposx) && (x <= style.aposx + 12) &&
                     (y >= style.aposy) && (y <= style.aposy + 7) ) {
                    h12 = !h12;
                    break;
                } else if ( (x >= style.pposx) && (x <= style.pposx + 12) &&
                            (y >= style.pposy) && (y <= style.pposy + 7) ) {
                    h12 = !h12;
                    break;
                } else if ( (x >= style.lposx) && (x <= style.lposx + 22) &&
                            (y >= style.lposy) && (y <= style.lposy + 7) ) {
                    if (alarms) switch_alarms(alarms);
                    break;
                }
            }
            if (state & ControlMask) {
                time_mode += 1;
                if (time_mode == 3) time_mode = CLOCK;
                time_update();
                draw_dockapp();
            } else {
                switch_light();
            }
            break;
        case 2:
            if (state & ControlMask) {
                my_system("wmclockmon-config -f", config_file);
            } else {
                rotate_style();
                load_style(style_name);
                graphics_init();
                draw_dockapp();
            }
            break;
        case 3:
            if (state & ControlMask) {
                my_system("wmclockmon-cal", NULL);
            } else {
                switch_authorized = !switch_authorized;
            }
            break;
        default: break;
    }
}