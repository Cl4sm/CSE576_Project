void shashlist_free(shashlist **list) {
/* shashlist_free
   Destroy and deallocate an entire hash list. */

   if(list == NULL || *list == NULL) return;

   /* Destroy the individual items first. */
   while((*list)->head != NULL) {
      (*list)->tail = (*list)->head;
      (*list)->head = (*list)->head->next;
      shashlist_item_free(&(*list)->tail);
   }

   free((*list)->hash);
   (*list)->hash = (shashlist_item **)0xdeadbeef;

   free(*list);
   *list = NULL;

}
