static gboolean _sc_inventory_weapon_button_gtk(__libj_unused ScActiveConsole *cons,
                                                ScActiveConsoleSpot *spot,
                                                GdkEventButton *event, sc_inventory_gtk *inv) {
/* sc_inventory_weapon_button_gtk
   User clicked in the weapon panel; process it and return
   TRUE if the key has been processed by this function. */

   sc_weapon_info *info = (sc_weapon_info *)spot->data;

   /* Make sure this is a SINGLE click event */
   if(event->type != GDK_BUTTON_PRESS) return(FALSE);

   switch(event->button) {
      case 1:  /* Left mouse */
         sc_inventory_buy_weapon(inv->p, info);
         _sc_inventory_info_paint_gtk(inv);
         _sc_inventory_weapon_paint_gtk(inv);
         _sc_inventory_accessory_paint_gtk(inv);
         return(TRUE);

      case 3:  /* Right mouse */
         sc_inventory_sell_weapon(inv->p, info);
         _sc_inventory_info_paint_gtk(inv);
         _sc_inventory_weapon_paint_gtk(inv);
         _sc_inventory_accessory_paint_gtk(inv);
         return(TRUE);
   }

   return(FALSE);

}
