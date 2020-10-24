Boolean StrToPort( str, p )
    char *str;
    Port *p;
{
    char *s, *t;
    int  x, y;

    if (str != NULL && p != NULL) {

        sscanf( str, "%d%d%d",&p->id, &x, &y );
        p->x = (Position) x;
        p->y = (Position) y;
        p->info[0] = '\0';

	s = XtMalloc( strlen(str) + 1 );	
        strcpy( s, str );
        t = strtok( s, " ," ); 		/* first token  id */
        t = strtok( NULL, " ," );	/* second token x  */
        t = strtok( NULL, " ," );	/* third token  y  */

        if (t != NULL && t+strlen(t)+1 < s+strlen(str)) 
            strcpy( p->info, t+strlen(t)+1 );/* stuff after tokens */
        
        return True;

    } else 
        return False;
}
