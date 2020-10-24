static void _sc_inventory_accessory_paint_gtk(sc_inventory_gtk *inv) {
/* sc_inventory_accessory_paint_gtk
   Paints the accessories pane. */

   int count;                       /* Number of accessories to display */
   int index;                       /* Current accessory index/iterator */
   sc_accessory_config *ac;         /* Accessory configuration data */
   const sc_accessory_info *info;   /* Data on the current accessory */
   char buf[SC_GTK_STRING_BUFFER];  /* Arbitrary text buffer */
   char less, great;                /* Can buy/sell indicators */

   /* Get the accessory lists */
   ac = inv->w->c->accessories;

   /* Deactivate any highlighting in the accessory panel */
   sc_console_highlight_detach_all(SC_CONSOLE(inv->accessorypane));

   /* Iterate through the list of accessories */
   count = sc_accessory_count(ac, SC_ACCESSORY_LIMIT_ALL);
   info = sc_accessory_first(ac, SC_ACCESSORY_LIMIT_ALL);
   for(index = 0; index < count; ++index) {
      /* Get data about this accessory */
      less = (sc_inventory_can_sell_accessory(inv->p, info) ? '<' : ' ');
      great = (sc_inventory_can_buy_accessory(inv->p, info, SC_INVENTORY_INFINITE) ? '>' : ' ');
      sbprintf(buf, sizeof(buf), " %c %-17s %2d/$%-7d  %2d %c",
               less, info->name, info->bundle, info->price,
               info->inventories[inv->p->index], great);

      /* Display the accessory data. If the player's budget currently
         does not allow them to buy or sell, or the inventory constraints
         kick in, then disable the item. */
      sc_console_write_line(SC_CONSOLE(inv->accessorypane), 0, index, buf);
      if(less == ' ' && great == ' ') {
         sc_console_highlight_attach_disabled(SC_CONSOLE(inv->accessorypane), 0, index,
                                              sc_console_get_width(SC_CONSOLE(inv->accessorypane)), 1);
      } /* Disabling the item? */
      info = sc_accessory_next(ac, info, SC_ACCESSORY_LIMIT_ALL);
   } /* Iterate through accessories */

}
