void
updateUrbanEntries(Context, city)
Sundata * Context;
City * city;
{
char *ptr;
int w;

    text_input = NULL_INPUT;
    if (city == NULL) return;
    if (city->name && city!=&Context->pos1) {
       w = (urban_w[0]/ 
           XTextWidth(UrbanCaller->gdata->font[MENUFONT],"_",1))-2;
       if (strlen(city->name)<w) w = strlen(city->name);
       strncpy(urban_entry[0].string, city->name, w);
       urban_entry[0].string[w] = '\0';
       urban_entry[0].caret = strlen(urban_entry[0].string);
    } else {
       urban_entry[0].string[0]='\0';
       urban_entry[0].caret = 0;
    }
    if (city->tz) {
       w = (urban_w[1]/ 
           XTextWidth(UrbanCaller->gdata->font[MENUFONT],"_",1))-2;
       if (strlen(city->tz)<w) w = strlen(city->tz);
       strncpy(urban_entry[1].string, city->tz, w);
       urban_entry[1].string[w] = '\0';
       urban_entry[1].caret = strlen(urban_entry[1].string);
    }
    ptr = num2str(city->lat, urban_entry[2].string, Context->flags.dms);
    urban_entry[2].caret = strlen(ptr);
    ptr = num2str(city->lon, urban_entry[3].string, Context->flags.dms);
    urban_entry[3].caret = strlen(ptr);
    sprintf(urban_entry[4].string, "%d", city->size);
    urban_entry[4].caret = 1;
}
