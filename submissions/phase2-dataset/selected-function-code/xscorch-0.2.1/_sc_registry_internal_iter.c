inline shashlist_item *_sc_registry_internal_iter(shashlist *list, shashlist_item *item, int class, sc_registry_direction direction, sc_registry_test_func function, long arg) {
/* _sc_registry_internal_iter
   Local function to find the next or prev entry in a set. */

   switch(direction) {
      case SC_REGISTRY_FORWARD:
         do {
            item = shashlist_find_next(list, item, class);
         } while(item != NULL && function != NULL && !function(item->data, arg));
         break;
      case SC_REGISTRY_REVERSE:
         do {
            item = shashlist_find_prev(list, item, class);
         } while(item != NULL && function != NULL && !function(item->data, arg));
         break;
   }

   return(item);

}
