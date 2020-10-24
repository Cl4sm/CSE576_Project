                                               ScActiveConsoleSpot *spot,
                                               sc_auto_def_gtk *adg) {
/* _sc_autodef_guidance_enter_gtk
   Move the selection to a new item in the list. */

   if(adg == NULL || spot == NULL) return(FALSE);

   if(!adg->ready) return(FALSE);

   #if SC_GTK_DEBUG_GTK && __debugging_macros
      SC_DEBUG_ENTER("got autodef guidance entry%s", "");
   #endif /* debug */

   /* Perform the selection settings. */
   if(spot->y) {
      /* The spots are offset +1 to allow for the (none) selection. */
      adg->auto_guidance = sc_accessory_lookup(adg->w->c->accessories,
                                               adg->set_guidance.items[spot->y - 1],
                                               SC_ACCESSORY_LIMIT_ALL);
   } else {
      adg->auto_guidance = NULL;
   }

   return(TRUE);

}
