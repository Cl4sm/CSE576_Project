static void _sc_active_console_class_init(ScActiveConsoleClass *klass) {

   GtkObjectClass *object_class = (GtkObjectClass *)klass;

   parent_class = g_type_class_peek(sc_console_get_type());

   _sc_active_console_signals[ACTIVATE_SIGNAL] =
      g_signal_new("activate",                     /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
                                                   /* Signal flags; run this signal first */
                   offsetof(ScActiveConsoleClass, activate),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   g_cclosure_marshal_VOID__VOID,
                                                   /* Marshal function for this signal */
                   G_TYPE_NONE,                    /* Return type for the marshaller */
                   0                               /* Number of extra parametres to pass */
                  );
   _sc_active_console_signals[SELECT_SPOT_SIGNAL] =
      g_signal_new("select_spot",                  /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScActiveConsoleClass, select_spot),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   _sc_marshal_BOOLEAN__POINTER,   /* Marshal function for this signal */
                   G_TYPE_BOOLEAN,                 /* Return type for the marshaller */
                   1,                              /* Number of extra parametres to pass */
                   G_TYPE_POINTER                  /* Type of first argument */
                  );
   _sc_active_console_signals[ENTER_SPOT_SIGNAL] =
      g_signal_new("enter_spot",                   /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScActiveConsoleClass, enter_spot),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   _sc_marshal_BOOLEAN__POINTER,   /* Marshal function for this signal */
                   G_TYPE_BOOLEAN,                 /* Return type for the marshaller */
                   1,                              /* Number of extra parametres to pass */
                   G_TYPE_POINTER                  /* Type of first argument */
                  );
   _sc_active_console_signals[LEAVE_SPOT_SIGNAL] =
      g_signal_new("leave_spot",                   /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScActiveConsoleClass, leave_spot),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   _sc_marshal_BOOLEAN__POINTER,   /* Marshal function for this signal */
                   G_TYPE_BOOLEAN,                 /* Return type for the marshaller */
                   1,                              /* Number of extra parametres to pass */
                   G_TYPE_POINTER                  /* Type of first argument */
                  );
   _sc_active_console_signals[BUTTON_PRESS_SPOT_SIGNAL] =
      g_signal_new("button_press_spot",            /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScActiveConsoleClass, button_press_spot),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   _sc_marshal_BOOLEAN__POINTER_POINTER,
                                                   /* Marshal function for this signal */
                   G_TYPE_BOOLEAN,                 /* Return type for the marshaller */
                   2,                              /* Number of extra parametres to pass */
                   G_TYPE_POINTER,                 /* Type of first argument */
                   G_TYPE_POINTER                  /* Type of second argument */
                  );
   _sc_active_console_signals[BUTTON_RELEASE_SPOT_SIGNAL] =
      g_signal_new("button_release_spot",          /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScActiveConsoleClass, button_release_spot),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   _sc_marshal_BOOLEAN__POINTER_POINTER,
                                                   /* Marshal function for this signal */
                   G_TYPE_BOOLEAN,                 /* Return type for the marshaller */
                   2,                              /* Number of extra parametres to pass */
                   G_TYPE_POINTER,                 /* Type of first argument */
                   G_TYPE_POINTER                  /* Type of second argument */
                  );
   _sc_active_console_signals[KEY_PRESS_SPOT_SIGNAL] =
      g_signal_new("key_press_spot",               /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScActiveConsoleClass, key_press_spot),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   _sc_marshal_BOOLEAN__POINTER_POINTER,
                                                   /* Marshal function for this signal */
                   G_TYPE_BOOLEAN,                 /* Return type for the marshaller */
                   2,                              /* Number of extra parametres to pass */
                   G_TYPE_POINTER,                 /* Type of first argument */
                   G_TYPE_POINTER                  /* Type of second argument */
                  );
   _sc_active_console_signals[KEY_RELEASE_SPOT_SIGNAL] =
      g_signal_new("key_release_spot",             /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScActiveConsoleClass, key_release_spot),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   _sc_marshal_BOOLEAN__POINTER_POINTER,
                                                   /* Marshal function for this signal */
                   G_TYPE_BOOLEAN,                 /* Return type for the marshaller */
                   2,                              /* Number of extra parametres to pass */
                   G_TYPE_POINTER,                 /* Type of first argument */
                   G_TYPE_POINTER                  /* Type of second argument */
                  );

   ((GtkWidgetClass *)klass)->activate_signal = _sc_active_console_signals[ACTIVATE_SIGNAL];

   /* Setup new signal default handlers */
   klass->activate            = _sc_active_console_activate;
   klass->select_spot         = NULL;
   klass->enter_spot          = NULL;
   klass->leave_spot          = NULL;
   klass->button_press_spot   = NULL;
   klass->button_release_spot = NULL;
   klass->key_press_spot      = NULL;
   klass->key_release_spot    = NULL;

   /* Install signals from parent class */
   SC_CONSOLE_CLASS(klass)->paint_region        = _sc_active_console_paint_region;
   GTK_WIDGET_CLASS(klass)->key_press_event     = _sc_active_console_key_press;
   GTK_WIDGET_CLASS(klass)->key_release_event   = _sc_active_console_key_release;
   GTK_WIDGET_CLASS(klass)->button_press_event  = _sc_active_console_button_press;
   GTK_WIDGET_CLASS(klass)->button_release_event= _sc_active_console_button_release;
   GTK_WIDGET_CLASS(klass)->focus_in_event      = _sc_active_console_focus_in;
   GTK_WIDGET_CLASS(klass)->focus_out_event     = _sc_active_console_focus_out;
   GTK_OBJECT_CLASS(klass)->destroy             = _sc_active_console_destroy;

}
