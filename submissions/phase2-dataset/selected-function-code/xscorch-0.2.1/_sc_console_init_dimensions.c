/* sc_console_init_dimensions
   Initialize console dimensions based on the current font metrics, and set
   the console's usize request.  */

   int x;
   int y;
   int width;
   int height;

   assert(IS_SC_CONSOLE(cons));

   x      = cons->text.dispx;
   y      = cons->text.dispy;
   width  = cons->text.vieww;
   height = cons->text.viewh;

   if(cons->style != CONSOLE_BORDERLESS) {
      x -= SC_CONSOLE_BORDER_WIDTH_CHARS;
      y -= SC_CONSOLE_BORDER_HEIGHT_CHARS;
      width += SC_CONSOLE_BORDER_WIDTH_CHARS * 2;
      height+= SC_CONSOLE_BORDER_HEIGHT_CHARS * 2;
   }

   cons->req_alloc.x = _sc_console_char_x(cons, x);
   cons->req_alloc.y = _sc_console_char_y(cons, y);
   cons->req_alloc.width = width * _sc_console_char_width(cons);
   cons->req_alloc.height= height* _sc_console_char_height(cons);

   gtk_widget_set_size_request(GTK_WIDGET(cons), cons->req_alloc.width, cons->req_alloc.height);

}
