shashlist_item *shashlist_find_by_string(const shashlist *list, const char *key) {
/* shashlist_find_by_string
   Find a shashlist item by string key. */

   int hashindex;
   shashlist_item *item;

   if(list == NULL || key == NULL)
      return(NULL);

   /* Locate the item using hash lookup. */
   hashindex = shash_string(list->hashbits, key);
   item = list->hash[hashindex];

   /* Scan the hash chain, as needed. */
   while(item != NULL && strcomp(item->keystr, key) != 0)
      item = item->chain;

   /* item will be either the right item or NULL, at this point. */
   return(item);

}
