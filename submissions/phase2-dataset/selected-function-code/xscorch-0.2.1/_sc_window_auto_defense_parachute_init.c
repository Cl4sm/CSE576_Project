void _sc_window_auto_defense_parachute_init(sc_auto_def_gtk *adg) {
/* _sc_window_auto_defense_parachute_init
   Draw up the console pane for Parachute Configuration.
   There is allocation here so don't call it multiple times. */

   /*
    * TEMP - This is all supposed to be on its own active console.
    *        But to avoid focus issues, not until it's implemented.
    */

   sc_console_write_line(SC_CONSOLE(adg->console_main),
                         48, 12, "Parachutes");
   sc_console_highlight_attach(SC_CONSOLE(adg->console_main), 
                               sc_console_get_color(SC_CONSOLE(adg->console_main), SC_CONSOLE_FORESTANDARD), 
                               NULL, TRUE, 48, 12, 10, 1);

   /*
    * TEMP - We need something different here, probably like
    *        a selection dial or text box in nature. - JL
    *
    * The value to be adjusted here is: adg->chute_height
    */
   sc_console_write_line(SC_CONSOLE(adg->console_main),
                         47, 13, "Unimplemented");
   sc_console_highlight_attach_disabled(SC_CONSOLE(adg->console_main),
                                        47, 13, 13, 1);

}
