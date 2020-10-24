/* shashlist_find_prev
   Find the previous shashlist_item after item in the specified class.
   Passing item == NULL is a slang way of asking for the last item. */

   if(list == NULL)
      return(NULL);

   /* Get the first candidate. */
   if(item == NULL)
      item = list->tail;
   else
      item = item->prev;

   /* Find the first item meeting class requirements. */
   while(item != NULL && !(class == SHASHLIST_ALL_CLASSES || item->class == class))
      item = item->prev;

   return(item);

}
