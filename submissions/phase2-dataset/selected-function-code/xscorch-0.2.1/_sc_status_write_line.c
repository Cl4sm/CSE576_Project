                                  const char *text, sc_status_state state) {

   switch(state) {
   case SC_STATUS_NORMAL:
      break;
   case SC_STATUS_PLAYER:
      sc_console_highlight_attach(SC_CONSOLE(w->status),
                                  &w->colormap->pcolors[p->index], NULL,
                                  FALSE, x, y, strlenn(text), 1);
      break;
   case SC_STATUS_BOLD:
      sc_console_highlight_attach(SC_CONSOLE(w->status),
                                  &w->colormap->white, NULL,
                                  TRUE, x, y, strlenn(text), 1);
      break;
   } /* what highlight mode? */
   sc_console_write_line(SC_CONSOLE(w->status), x, y, text);

}
