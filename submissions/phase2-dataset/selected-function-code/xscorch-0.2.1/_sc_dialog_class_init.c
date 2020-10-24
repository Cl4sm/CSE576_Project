static void _sc_dialog_class_init(ScDialogClass *klass) {
/* sc_dialog_class_init
   Initialise the dialog class.  */

   GtkObjectClass *object_class = (GtkObjectClass *)klass;

   /* Get parent class */
   parent_class = g_type_class_peek(gtk_window_get_type());

   _sc_dialog_signals[APPLY_SIGNAL] =
      g_signal_new("apply",                        /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScDialogClass, apply),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   g_cclosure_marshal_VOID__VOID,  /* Marshal function for this signal */
                   G_TYPE_NONE,                    /* Return type for the marshaller */
                   0                               /* Number of extra parametres to pass */
                  );

   /* Setup new signals */
   klass->apply = NULL;

   /* Setup signals from parent */
   GTK_OBJECT_CLASS(klass)->destroy = _sc_dialog_destroy;
   GTK_WIDGET_CLASS(klass)->key_press_event = _sc_dialog_key_press;

}
