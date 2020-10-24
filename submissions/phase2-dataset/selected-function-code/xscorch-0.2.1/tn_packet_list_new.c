tn_packet_list *tn_packet_list_new(void) {
/*
 * tn_packet_list_new
 * Create a new packet list item.
 */

   tn_packet_list *item;

   /* Allocate memory, failure return NULL. */
   item = (tn_packet_list *)malloc(sizeof(tn_packet_list));
   if(item == NULL) return(NULL);

   /* Tack a packet onto the item. */
   item->packet = tn_packet_new();
   if(item->packet == NULL) {
      free(item);
      return(NULL);
   }

   /* Set defaults on the item. */
   item->next     = NULL;
   item->size     = 0;
   item->complete = false;

   return(item);

}
