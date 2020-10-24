/* sc_ac_toggle_new
   Instantiate a new toggle widget. */

   ScACToggle *toggle;
   ScGadget   *gadget;

   toggle = g_object_new(sc_ac_toggle_get_type(), NULL);
   g_return_val_if_fail(toggle != NULL, NULL);

   gadget = SC_GADGET(toggle);
   gadget->x = x;
   gadget->y = y;
   gadget->width  = width;
   gadget->height = height;
   toggle->state = FALSE;

   return(SC_GADGET(toggle));

}
