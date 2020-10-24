                     GdkFont *font, GdkFont *boldfont) {
/* sc_console_init
   Initialize a console.  */

   g_return_if_fail(IS_SC_CONSOLE(cons));
   g_return_if_fail(x >= 0 && y >= 0);
   g_return_if_fail(width > 0 && height > 0);
   g_return_if_fail(font != NULL && boldfont != NULL);

   _sc_console_destroy_data(cons);
   _sc_console_set_fonts(cons, font, boldfont);

   cons->text.vieww   = width;
   cons->text.viewh   = height;
   cons->text.dispx   = x;
   cons->text.dispy   = y;
   sc_console_buffer_size(cons, width, height);

   cons->style = style;

   _sc_console_init_dimensions(cons);

   gtk_widget_set_sensitive(GTK_WIDGET(cons), FALSE);

   sc_console_clear(cons);

}
