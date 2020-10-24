
   double dr;
   double dg;
   double db;
   int count;
   int i;
   
   dr = (fr - r) / SC_MAX_GRADIENT_SIZE;
   dg = (fg - g) / SC_MAX_GRADIENT_SIZE;
   db = (fb - b) / SC_MAX_GRADIENT_SIZE;

   for(count = 0, i = 0; i < SC_MAX_GRADIENT_SIZE; ++i) {
      if(_sc_colormap_set_gtk(w, &w->colormap->gradient[gradidx][count], r, g, b)) { 
         if(count == 0 || w->colormap->gradient[gradidx][count].pixel != w->colormap->gradient[gradidx][count - 1].pixel) {
            ++count;
         }
      }
      r += dr;
      g += dg;
      b += db;
   }
   w->c->colors->gradsize[gradidx] = count;

}
