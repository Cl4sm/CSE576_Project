addCity(longparams)
char *longparams;
{
     City *city;
     char name[80], lat[80], lon[80], tz[80];
     char params[256];
     int i, size;

     i = 0;
     if (longparams) {
        while (longparams[i] != '\0') {
           if (longparams[i]==' ') longparams[i] = '\037';
           if (longparams[i]=='|') longparams[i] = ' ';
           ++i;
        }
        if (sscanf(longparams,"%d %s %s %s %s", &size, name, lat, lon, tz)<5) {
           fprintf(stderr, "Incorrect number of parameters in -addcity\n");
	   return NULL;
	}
        i = 0;
        while (name[i] != '\0') {
           if (name[i]=='\037') name[i] = ' ';
           ++i;
        }
     } else {
       if (do_urban) {
          if (!*urban_entry[0].string) {
	     strcpy(urban_entry[0].string, "???");
	     return NULL;
	  }
          strcpy(name, urban_entry[0].string);
          if (!*urban_entry[1].string) {
	     strcpy(urban_entry[1].string, "???");
	     return NULL;
	  }
          strcpy(tz, urban_entry[1].string);
          if (!*urban_entry[2].string) return NULL;
          strcpy(lat, urban_entry[2].string);
          if (!*urban_entry[3].string) return NULL;
          strcpy(lon, urban_entry[3].string);
          if (!*urban_entry[4].string) return NULL;
          if (!sscanf(urban_entry[4].string, "%d", &size)) {
	     strcpy(urban_entry[4].string, "?");
             return NULL;
	  }
       } else
	  return NULL;
     }
     
     if (citycheck || runlevel>READSYSRC) {
        sprintf(params, "%s\037|%s\037|%s\037", name, lat, lon);
	if ((city=searchCityLocation(params))) {
	   sprintf(params, Label[L_CITYWARNING1], name, lat, lon, "\n  ");
	   if (verbose || runlevel != READUSERRC)
	      fprintf(stderr, "%s\n", params);
	   if (do_urban) {
	      urban_newhint = '?';
	      showUrbanHint(params);
	      return NULL;
	   }
	   if (verbose || runlevel != READUSERRC) {
	      sprintf(params, Label[L_CITYWARNING2], name);
	      fprintf(stderr, "%s\n", params);
	   }
           removeCity(city);
	}
     }

     /* Create the record for the city */
     if ((city = (City *) calloc(1, sizeof(City))) == NULL) return NULL;     

     /* Set up the record */

     city->name = strdup(name);
     city->lat = dms2decim(lat);
     city->lon = dms2decim(lon);
     city->size = size;
     city->tz = strdup(tz);

     /* Link it into the list */

     if (!cityroot) 
        cityroot = citylast = city;
     else {
        citylast->next = city;
	citylast = city;
     }
     city ->next = NULL;
     return city;
}
