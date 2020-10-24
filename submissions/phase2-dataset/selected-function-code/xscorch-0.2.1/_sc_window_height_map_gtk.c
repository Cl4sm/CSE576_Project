                                      const int *ylist, int size) {
/* sc_window_height_map_gtk
   Creates a heightmap for the list of coordinates, given.  If there is no
   data point for a particular value of x, the cooresponding height will be
   set to -1.  On success, a structure is returned which must be released
   manually, using free().  */

   const int *pxlist;      /* Pointer into X coord array */
   const int *pylist;      /* Pointer into Y coord array */
   int *heightmap;         /* Data structure storing hmap */
   int *pmap;              /* Pointer into the map */
   int x;                  /* X coordinate */
   int y;                  /* Y coordiante */
   int i;                  /* Iterator */

   /* Create storage for the heightmap */
   heightmap = (int *)malloc(sizeof(int) * c->fieldwidth);
   if(heightmap == NULL) return(NULL);

   /* Initialise the heightmap */
   pmap = heightmap;
   for(x = 0; x < c->fieldwidth; ++x) {
      *(pmap++) = -1;
   } /* Initialising map array */

   /* Construct datavalues for the heightmap, as needed */
   pxlist = xlist;
   pylist = ylist;
   for(i = 0; i < size; ++i) {
      /* Get the raw X,Y coordinates */
      x = *(pxlist++);
      y = *(pylist++);

      /* Translate to `proper' screen coordinates, if possible. */
      if(sc_land_translate_xy(c->land, &x, &y)) {
         /* Was this point higher than previously known height? */
         if(heightmap[x] < y) heightmap[x] = y;
      }
   }

   /* Successful return */
   return(heightmap);

}
