City *
searchCityLocation(params)
char *params;
{
     City *c;
     char name[80], lat[80], lon[80];
     double dlat=0.0, dlon=0.0;
     int i, l, complete = 0;

     if (!params || !*params) return NULL;

     if (index(params, '|')) {
        l =strlen(params);
        for (i=0; i<l; i++) {
            if (params[i] == ' ') params[i]= '\037';
            if (params[i] == '|') params[i]= ' ';
	}
        sscanf(params, "%s %s %s", name, lat, lon);
        l = strlen(lat); lat[l-1] = '\0';
        l = strlen(lon); lon[l-1] = '\0';
	dlat = dms2decim(lat);
	dlon = dms2decim(lon);
        l = strlen(name);
	while (l>0 && name[l-1]=='\037') {
	   --l;
	   name[l] = '\0';
	}
        for (i=0; i<l; i++) if (name[i] == '\037') name[i] = ' '; 
        complete = 1;
     } else
        strncpy(name, params, 80);

     c = cityroot;
     while (c) {
        if ((!*name || !strcasecmp(c->name, name)) &&
            (!complete || (fabs(c->lon-dlon)<0.5 && fabs(c->lat-dlat)<0.5)))
	   return c;
        c = c->next;
     }
     return NULL;
}
