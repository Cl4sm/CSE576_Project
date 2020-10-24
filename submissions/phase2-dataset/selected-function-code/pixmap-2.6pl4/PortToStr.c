char *PortToStr( p )
    Port *p;
{
    char id_str[40];
    char x_str[40];
    char y_str[40];
    char *str = NULL;

    if (p != NULL) {
        sprintf( id_str, "%d", p->id );
        sprintf( x_str,  "%d", p->x );
        sprintf( y_str,  "%d", p->y );
	str = XtMalloc( strlen(x_str) + strlen(y_str) + strlen(id_str)
                        + strlen(p->info) + 4 );
        str[0] = '\0';
        strcat( str, id_str  );
        strcat( str, " "     );
        strcat( str, x_str   );
        strcat( str, " "     );
        strcat( str, y_str   );
        strcat( str, " "     );
        strcat( str, p->info );
    }
    return str;
}
