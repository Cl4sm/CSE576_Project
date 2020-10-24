void
removeCity(c)
City *c;
{
     City *cp, *cn;

     if (!c) return;  
     cn = c->next;
     freecity(c);

     if (c == cityroot) {
        cityroot = cn;
	return;
     }

     cp = cityroot;
     while (cp) {
        if (cp->next == c) {
	    cp->next = cn;
            if (c == citylast) citylast = cp;
	    return;
	}  
	cp = cp->next;
     }
}
