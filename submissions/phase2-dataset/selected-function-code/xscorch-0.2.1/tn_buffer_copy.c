/*
 * tn_buffer_copy
 * Copy src to dst until cur becomes len unless not enough avl.
 * This has side-effects of increasing src and cur and decreasing avl.
 */

   bool ret = true;
   int toc = len;

   /* Although an int, *avl is required to be >= 0. */
   assert(*avl >= 0);

   /* Zero-copy always succeeds. */
   if(len <= 0) return(true);

   /* Check if we can fill the request. */
   if(toc > (*avl)) {
      toc = (*avl);
      ret = false;
   }

   /* Don't try copies on too little src. */
   if(toc <= 0) return(false);

   /* Perform the copy. */
   memcpy(dst, (*src), toc);

   /* Perform side-effects. */
   (*src) += toc;
   (*cur) += toc;
   (*avl) -= toc;

   /* Return true if we fully filled the request. */
   return(ret);

}
