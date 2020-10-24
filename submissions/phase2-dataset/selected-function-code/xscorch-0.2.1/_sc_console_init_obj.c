/* sc_console_init_obj
   Initialize a new instance of a console.  */

   /* Clear the text buffer */
   cons->text.scrollx = 0;
   cons->text.scrolly = 0;
   cons->text.buffer  = NULL;
   _sc_console_init_fonts(cons);
   cons->cursor.highlighted = FALSE;
   cons->highlights = NULL;
   cons->ignorerelease = FALSE;

   /* Reset the cursor */
   cons->cursor.x = 0;
   cons->cursor.y = 0;
   cons->cursor.width = 0;
   cons->cursor.height= 0;

   /* Initialize console colours */
   gdk_color_parse("#000000", &cons->colors.background);
   gdk_color_parse("#c0c0c0", &cons->colors.foreground);
   gdk_color_parse("#701010", &cons->colors.backcursor);
   gdk_color_parse("#ffffff", &cons->colors.forecursor);
   gdk_color_parse("#202020", &cons->colors.backshadow);
   gdk_color_parse("#e0e0e0", &cons->colors.foreshadow);
   gdk_color_parse("#101060", &cons->colors.backscroll);
   gdk_color_parse("#7090a0", &cons->colors.forescroll);
   gdk_color_parse("#a02020", &cons->colors.backlight);
   gdk_color_parse("#ffffff", &cons->colors.forelight);
   gdk_color_parse("#000000", &cons->colors.backdisabled);
   gdk_color_parse("#606060", &cons->colors.foredisabled);
   gdk_color_parse("#000000", &cons->colors.backstandard);
   gdk_color_parse("#c0c0c0", &cons->colors.forestandard);
   cons->colors.colors_alloc = FALSE;
   cons->colors.bold = FALSE;

   /* Make sure the console is focusable. */
   gtk_widget_set_can_focus(GTK_WIDGET(cons), TRUE);

}
