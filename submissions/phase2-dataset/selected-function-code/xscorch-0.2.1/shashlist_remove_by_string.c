shashlist_item *shashlist_remove_by_string(shashlist *list, const char *key) {
/* shashlist_remove_item_by_string
   Remove the shashlist data item referenced by this key string. */

   int hashindex;
   shashlist_item *item, *temp;

   if(list == NULL || key == NULL)
      return(NULL);

   /* Locate the item using hash lookup. */
   hashindex = shash_string(list->hashbits, key);
   item = list->hash[hashindex];
   if(item == NULL)
      return(NULL);

   /* Scan the collision chain as needed and remove the item from the hash. */
   if(strcomp(item->keystr, key) == 0) {
      list->hash[hashindex] = item->chain;
   } else {
      temp = item;
      while(temp->chain != NULL && strcomp(temp->chain->keystr, key) != 0)
         temp = temp->chain;
      if(temp->chain == NULL)
         return(NULL);
      item = temp->chain;
      temp->chain = temp->chain->chain;
   }

   /* Remove the item from the linked list. */
   if(item->prev == NULL) {
      list->head = item->next;
   } else {
      item->prev->next = item->next;
   }
   if(item->next == NULL) {
      list->tail = item->prev;
   } else {
      item->next->prev = item->prev;
   }

   /* Return the orphaned item. */
   return(item);

}
