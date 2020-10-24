/* sc_inventory_weapon_paint_gtk
   Paint the weapons window pane.  */

   int count;                       /* Number of weapons to display */
   int index;                       /* Current weapon index/iterator */
   sc_weapon_config *wc;            /* Weapon configuration data */
   const sc_weapon_info *info;      /* Data on the current weapon */
   char buf[SC_GTK_STRING_BUFFER];  /* Arbitrary text buffer */
   char less, great;                /* Can buy/sell indicators */

   /* Get the weapon lists */
   wc = inv->w->c->weapons;

   /* Deactivate any highlighting in the weapon panel */
   sc_console_highlight_detach_all(SC_CONSOLE(inv->weaponpane));

   /* Iterate through the list of weapons */
   count = sc_weapon_count(wc, SC_WEAPON_LIMIT_ALL);
   info = sc_weapon_first(wc, SC_WEAPON_LIMIT_ALL);
   for(index = 0; index < count; ++index) {
      /* Get data about this weapon */
      less = (sc_inventory_can_sell_weapon(inv->p, info) ? '<' : ' ');
      great = (sc_inventory_can_buy_weapon(inv->p, info, SC_INVENTORY_INFINITE) ? '>' : ' ');
      sbprintf(buf, sizeof(buf), " %c %-17s %2d/$%-7d  %2d %c",
               less, info->name, info->bundle, info->price,
               info->inventories[inv->p->index], great);

      /* Display the weapon data. If the player's budget currently
         does not allow them to buy or sell, or the inventory constraints
         kick in, then disable the item. */
      sc_console_write_line(SC_CONSOLE(inv->weaponpane), 0, index, buf);
      if(less == ' ' && great == ' ') {
         sc_console_highlight_attach_disabled(SC_CONSOLE(inv->weaponpane), 0, index,
                                              sc_console_get_width(SC_CONSOLE(inv->weaponpane)), 1);
      } /* Disabling the item? */
      info = sc_weapon_next(wc, info, SC_WEAPON_LIMIT_ALL);
   } /* Iterate through weapons */

}
