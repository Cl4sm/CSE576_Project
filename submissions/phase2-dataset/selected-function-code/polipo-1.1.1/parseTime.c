static int
parseTime(char *line, int i, int *value_return)
{
    int v = 0, w;
    while(1) {
        if(!digit(line[i]))
            break;
        w = atoi(line + i);
        while(digit(line[i])) i++;
        switch(line[i]) {
        case 'd': v += w * 24 * 3600; i++; break;
        case 'h': v += w * 3600; i++; break;
        case 'm': v += w * 60; i++; break;
        case 's': v += w; i++; break;
        default: v += w; goto done;
        }
    }
 done:
    *value_return = v;
    return i;
}