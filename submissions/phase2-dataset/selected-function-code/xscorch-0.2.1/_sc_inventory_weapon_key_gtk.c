                                             ScActiveConsoleSpot *spot,
                                             GdkEventKey *event, sc_inventory_gtk *inv) {
/* sc_inventory_weapon_key_gtk
   User hit a key in the weapons panel; process it and return
   TRUE if the key has been processed by this function. */

   sc_weapon_info *info = (sc_weapon_info *)spot->data;

   switch(event->keyval) {
      case GDK_Right:
      case GDK_KP_Right:
         sc_inventory_buy_weapon(inv->p, info);
         _sc_inventory_info_paint_gtk(inv);
         _sc_inventory_weapon_paint_gtk(inv);
         _sc_inventory_accessory_paint_gtk(inv);
         return(TRUE);

      case GDK_Left:
      case GDK_KP_Left:
         sc_inventory_sell_weapon(inv->p, info);
         _sc_inventory_info_paint_gtk(inv);
         _sc_inventory_weapon_paint_gtk(inv);
         _sc_inventory_accessory_paint_gtk(inv);
         return(TRUE);
   }

   return(FALSE);

}
