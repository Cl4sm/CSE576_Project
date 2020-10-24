int
parseUrl(const char *url, int len,
         int *x_return, int *y_return, int *port_return, int *z_return)
{
    int x, y, z, port = -1, i = 0;

    if(len >= 7 && lwrcmp(url, "http://", 7) == 0) {
        x = 7;
        if(x < len && url[x] == '[') {
            /* RFC 2732 */
            for(i = x + 1; i < len; i++) {
                if(url[i] == ']') {
                    i++;
                    break;
                }
                if((url[i] != ':') && !letter(url[i]) && !digit(url[i]))
                    break;
            }
        } else {
            for(i = x; i < len; i++)
                if(url[i] == ':' || url[i] == '/')
                    break;
        }
        y = i;

        if(i < len && url[i] == ':') {
            int j;
            j = atoi_n(url, i + 1, len, &port);
            if(j < 0) {
                port = 80;
            } else {
                    i = j;
            }
        } else {
            port = 80;
        }
    } else {
        x = -1;
        y = -1;
    }

    z = i;

    *x_return = x;
    *y_return = y;
    *port_return = port;
    *z_return = z;
    return 0;
}