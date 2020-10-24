/* sc_console_set_fonts
   Update the fonts installed for this console.  Neither font pointer
   should be NULL.  The currently-installed fonts will be unreferenced,
   the new fonts will have their reference counts incremented by 1, and
   the entire console will be resized, repositioned, and redrawn.  */

   #if SC_GTK_DEBUG_GTK
      printf("sc_console_set_fonts:  installing new console fonts for %p:  %p %p\n", 
             (void *)cons, (void *)font, (void *)boldfont);
   #endif /* SC_GTK_DEBUG_GTK */

   g_return_if_fail(IS_SC_CONSOLE(cons));
   g_return_if_fail(font != NULL && boldfont != NULL);

   _sc_console_set_fonts(cons, font, boldfont);
   _sc_console_init_dimensions(cons);

   #if SC_GTK_DEBUG_GTK
      printf("sc_console_set_fonts:  finished installing new console fonts for %p\n", 
             (void *)cons);
   #endif /* SC_GTK_DEBUG_GTK */

}
