void
drawShadedArea (Context)
Sundata * Context;
{
     int size;

     if ((Context->flags.colorlevel<FULLCOLORS) || 
         (Context->flags.colorscale == 1)) {
        if (Context->flags.shading) {
           initShading(Context);
           moveNightArea(Context);
        } else {
           clearNightArea(Context);
           if (Context->tr1) {
              free(Context->tr1);
              Context->tr1 = NULL;
           }
        }
     } else {
        size = Context->xim->bytes_per_line*Context->xim->height;
        memcpy(Context->xim->data, Context->ximdata, size); 
        initShading(Context);
     }
}
