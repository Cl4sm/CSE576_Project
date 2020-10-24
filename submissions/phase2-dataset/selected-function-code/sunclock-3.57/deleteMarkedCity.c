void
deleteMarkedCity()
{
     City *c, *cp = NULL;

     if (!do_urban || !UrbanCaller) return;

     c = cityroot;
     while (c) {
        if (c == UrbanCaller->mark1.city) {
	   UrbanCaller->mark1.city = NULL;
	   if (cp) {
	      cp->next = c->next;
	   } else
	      cityroot = c->next;
	   if (c == citylast) citylast = cp;
	   if (c == UrbanCaller->lastmarked) UrbanCaller->lastmarked = NULL;
           freecity(c);
	   return;
	}
        cp = c;
        c = c->next;
     }
}
