static gint _sc_ac_toggle_key(ScGadget *gadget, GdkEventKey *event) {
/* _sc_ac_toggle_key
   Key press events for the toggle. */

   ScACToggle *toggle = SC_AC_TOGGLE(gadget);

   /* Try out parent handler first */
   if(parent_class->key_press_spot != NULL) {
      if(parent_class->key_press_spot(gadget, event)) {
         /* Crap. The signal's already been handled */
         return(TRUE);
      } /* Signal processed? */
   } /* Signal handler available? */

   #if SC_GTK_DEBUG_GTK && __debugging_macros
      SC_DEBUG_ENTER("will process key %d press", event->keyval);
   #endif /* debug */

   switch(event->keyval) {
      case GDK_Return:
      case GDK_KP_Enter:
      case GDK_space:
      case GDK_KP_Space:
         toggle->state = toggle->state ? FALSE : TRUE;
         _sc_ac_toggle_emit_paint(toggle);
         return(TRUE);
   }

   /* Fallthrough */
   return(FALSE);

}
