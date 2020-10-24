void
drawLabels(Context)
struct Sundata * Context;
{
   int ilon, ilat, width, dw = 0;
   struct TextLabel * label;
   char *text, *text0, *ptr;

   if(!Context->wintype) return;
   label = Context->label;
   while (label) if (label->text && *label->text) {
      ilon = int_longitude(Context, label->lon);
      ilat = int_latitude(Context, label->lat);
      text = text0 = strdup(label->text);
      while(text) {
	 ptr = index(text,'\n');
	 if (ptr) *ptr = '\0';
         width = XTextWidth(Context->gdata->font[LABELFONT], 
			    text, strlen(text));
         if (label->position==0) dw = width/2;
         else
	 if (label->position==-1) dw = width;
         XPutStringImage(Context, ilon-dw, ilat, text,
   	                 strlen(text), label->color+3);
	 ilat += Context->gdata->font[LABELFONT]->ascent+
	         Context->gdata->font[LABELFONT]->descent+2;
	 if (ptr) text = ptr+1; else text = NULL;
      }
      free(text0);
      label = label->next;
   }
}
