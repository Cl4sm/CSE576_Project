static gint _sc_ac_toggle_button(ScGadget *gadget, GdkEventButton *event) {
/* _sc_ac_toggle_button
   Button press events for the toggle. */

   ScACToggle *toggle = SC_AC_TOGGLE(gadget);

   /* Try out parent handler first */
   if(parent_class->button_press_spot != NULL) {
      if(parent_class->button_press_spot(gadget, event)) {
         /* Crap. The signal's already been handled */
         return(TRUE);
      } /* Signal processed? */
   } /* Signal handler available? */

   #if SC_GTK_DEBUG_GTK && __debugging_macros
      SC_DEBUG_ENTER("will process button %d press", event->button);
   #endif /* debug */

   switch(event->button) {
      case 1:  /* Left mouse */
         toggle->state = toggle->state ? FALSE : TRUE;
         _sc_ac_toggle_emit_paint(toggle);
         return(TRUE);
   }

   /* Fallthrough */
   return(FALSE);

}
