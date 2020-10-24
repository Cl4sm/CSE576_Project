/* sc_console_class_init
   Initialise the console class.  */

   GtkObjectClass *object_class = (GtkObjectClass *)klass;

   /* Determine parent class */
   parent_class = g_type_class_peek(sc_drawbuf_get_type());

   /* Construct new signals */
   _sc_console_signals[PAINT_REGION_SIGNAL] =
      g_signal_new("paint_region",                 /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScConsoleClass, paint_region),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   g_cclosure_marshal_VOID__POINTER,
                                                   /* Marshal function for this signal */
                   G_TYPE_NONE,                    /* Return type for the marshaller */
                   1,                              /* Number of extra parametres to pass */
                   G_TYPE_POINTER                  /* Type of first parametre to marshaller */
                  );

   /* Attach default signal handlers */
   klass->paint_region                          = NULL;
   GTK_OBJECT_CLASS(klass)->destroy             = _sc_console_destroy;
   GTK_WIDGET_CLASS(klass)->configure_event     = _sc_console_configure;
   GTK_WIDGET_CLASS(klass)->focus_in_event      = _sc_console_draw_focus;
   GTK_WIDGET_CLASS(klass)->focus_out_event     = _sc_console_undraw_focus;
   GTK_WIDGET_CLASS(klass)->button_press_event  = _sc_console_button_press;
   GTK_WIDGET_CLASS(klass)->button_release_event= _sc_console_button_release;
   GTK_WIDGET_CLASS(klass)->key_press_event     = _sc_console_key_press;
   GTK_WIDGET_CLASS(klass)->key_release_event   = _sc_console_key_release;

}
