void sc_console_buffer_size(ScConsole *cons, gint width, gint height) {
/* sc_console_buffer_size
   This resets the console size.  The buffer text is cleared.  This does not
   automatically redraw the console; it is generally used as an initialization
   event only.  This does NOT update the viewport dimensions, although it will
   reset the viewport position to the top-left of the buffer.  */

   if(width <= 0 || height <= 0) {
      printf("Let me get this straight. You want to create an empty buffer?\n");
      printf("... oh, you are SO going to regret this operation.  The console\n");
      printf("... library will make you SUFFER HORRIBLY for this transgression.\n");
      printf("(this is Jacob's honorary bug!)\n");
   }
   g_return_if_fail(width > 0 && height > 0);

   if(width < cons->text.vieww) width = cons->text.vieww;
   if(height< cons->text.viewh) height= cons->text.viewh;
   cons->text.buffer = (char *)realloc(cons->text.buffer, width * height);
   if(cons->text.buffer != NULL) {
      memset(cons->text.buffer, ' ', width * height);
   }

   cons->text.viewx   = 0;
   cons->text.viewy   = 0;
   cons->text.bufferw = width;
   cons->text.bufferh = height;
   cons->text.scrollx = (cons->text.vieww < width);
   cons->text.scrolly = (cons->text.viewh < height);

}
