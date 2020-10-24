shashlist_item *shashlist_find_next(const shashlist *list, shashlist_item *item, int class) {
/* shashlist_find_next
   Find the next shashlist_item after item that is in the specified class.
   Passing item == NULL is a slang way of asking for the first item. */

   if(list == NULL)
      return(NULL);

   /* Get the first candidate. */
   if(item == NULL)
      item = list->head;
   else
      item = item->next;

   /* Find the first item meeting class requirements. */
   while(item != NULL && !(class == SHASHLIST_ALL_CLASSES || item->class == class))
      item = item->next;

   return(item);

}
