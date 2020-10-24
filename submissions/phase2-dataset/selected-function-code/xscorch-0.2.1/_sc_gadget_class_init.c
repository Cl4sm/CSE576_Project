static void _sc_gadget_class_init(ScGadgetClass *klass) {

   GtkObjectClass *object_class = (GtkObjectClass *)klass;

   gadget_parent_class = g_type_class_peek(gtk_object_get_type());

   _sc_gadget_signals[G_PAINT_SIGNAL] =
      g_signal_new("paint",                        /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScGadgetClass, paint),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   g_cclosure_marshal_VOID__VOID,
                                                   /* Marshal function for this signal */
                   G_TYPE_NONE,                    /* Return type for the marshaller */
                   0                               /* Number of extra parametres to pass */
                  );
   _sc_gadget_signals[G_SELECT_SPOT_SIGNAL] =
      g_signal_new("select_spot",                  /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScGadgetClass, select_spot),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   _sc_marshal_BOOLEAN__VOID,      /* Marshal function for this signal */
                   G_TYPE_BOOLEAN,                 /* Return type for the marshaller */
                   0                               /* Number of extra parametres to pass */
                  );
   _sc_gadget_signals[G_ENTER_SPOT_SIGNAL] =
      g_signal_new("enter_spot",                   /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScGadgetClass, enter_spot),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   _sc_marshal_BOOLEAN__VOID,      /* Marshal function for this signal */
                   G_TYPE_BOOLEAN,                 /* Return type for the marshaller */
                   0                               /* Number of extra parametres to pass */
                  );
   _sc_gadget_signals[G_LEAVE_SPOT_SIGNAL] =
      g_signal_new("leave_spot",                   /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScGadgetClass, leave_spot),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   _sc_marshal_BOOLEAN__VOID,      /* Marshal function for this signal */
                   G_TYPE_BOOLEAN,                 /* Return type for the marshaller */
                   0                               /* Number of extra parametres to pass */
                  );
   _sc_gadget_signals[G_BUTTON_PRESS_SPOT_SIGNAL] =
      g_signal_new("button_press_spot",            /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScGadgetClass, button_press_spot),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   _sc_marshal_BOOLEAN__POINTER,   /* Marshal function for this signal */
                   G_TYPE_BOOLEAN,                 /* Return type for the marshaller */
                   1,                              /* Number of extra parametres to pass */
                   G_TYPE_POINTER                  /* Type of first argument */
                  );
   _sc_gadget_signals[G_BUTTON_RELEASE_SPOT_SIGNAL] =
      g_signal_new("button_release_spot",          /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScGadgetClass, button_release_spot),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   _sc_marshal_BOOLEAN__POINTER,   /* Marshal function for this signal */
                   G_TYPE_BOOLEAN,                 /* Return type for the marshaller */
                   1,                              /* Number of extra parametres to pass */
                   G_TYPE_POINTER                  /* Type of first argument */
                  );
   _sc_gadget_signals[G_KEY_PRESS_SPOT_SIGNAL] =
      g_signal_new("key_press_spot",               /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScGadgetClass, key_press_spot),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   _sc_marshal_BOOLEAN__POINTER,   /* Marshal function for this signal */
                   G_TYPE_BOOLEAN,                 /* Return type for the marshaller */
                   1,                              /* Number of extra parametres to pass */
                   G_TYPE_POINTER                  /* Type of first argument */
                  );
   _sc_gadget_signals[G_KEY_RELEASE_SPOT_SIGNAL] =
      g_signal_new("key_release_spot",             /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScGadgetClass, key_release_spot),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   _sc_marshal_BOOLEAN__POINTER,   /* Marshal function for this signal */
                   G_TYPE_BOOLEAN,                 /* Return type for the marshaller */
                   1,                              /* Number of extra parametres to pass */
                   G_TYPE_POINTER                  /* Type of first argument */
                  );

   /* Setup new signal default handlers */
   klass->paint               = NULL;
   klass->select_spot         = NULL;
   klass->enter_spot          = NULL;
   klass->leave_spot          = NULL;
   klass->button_press_spot   = NULL;
   klass->button_release_spot = NULL;
   klass->key_press_spot      = NULL;
   klass->key_release_spot    = NULL;

}
