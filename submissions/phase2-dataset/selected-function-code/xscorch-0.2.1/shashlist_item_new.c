/* shashlist_item_new
   Create and initialize a new shashlist_item. */

   shashlist_item *item;
   int len;

   item = (shashlist_item *)malloc(sizeof(shashlist_item));
   if(item == NULL)
      return(NULL);

   /* Set the string key, if any. */
   if(keystr == NULL) {
      item->keystr = NULL;
   } else {
      len = strblenn(keystr, SHASHLIST_MAX_KEYSTR_LEN) + 1;
      item->keystr = (char *)malloc(len);
      if(item->keystr == NULL) {
         free(item);
         return(NULL);
      }
      strcopyb(item->keystr, keystr, len);
   }

   item->class  = class;
   item->data   = data;
   item->key    = key;

   item->chain  = NULL;
   item->next   = NULL;
   item->prev   = NULL;

   return(item);

}
