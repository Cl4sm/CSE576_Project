void
scanPosition(string, city)
char * string;
City * city;
{
int i, l;
char lat[80], lon[80];

     if (!string || !*string) return; 
     
     l = strlen(string);
     for (i=0; i<l; i++) if (string[i] == '|') string[i] = ' ';
     
     if (sscanf(string, "%s %s", lat, lon)<2) {
        city->lat = -100.0;
        return;
     }
     city->lat = dms2decim(lat);
     if (fabs(city->lat)>90.0) city->lat = -100.0;
     city->lon = dms2decim(lon);
     city->lon = fixangle(city->lon + 180.0) - 180.0;
}
