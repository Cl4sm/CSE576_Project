static int
parseState(char *buf, int offset, int kind)
{
    int i = offset;
    int n;
    int state = -1;

    while(letter(buf[i]))
        i++;
    for(n = 0; n < sizeof(states) / sizeof(states[0]); n++) {
        if(strlen(states[n].name) == i - offset &&
           lwrcmp(buf + offset, states[n].name, i - offset) == 0) {
            state = states[n].value;
            break;
        }
    }
    if(state < 0)
        return -1;

    switch(kind) {
    case CONFIG_BOOLEAN:
        if(state == 0) return 0;
        else if(state == 4) return 1;
        else return -1;
        break;
    case CONFIG_TRISTATE:
        if(state == 0) return 0;
        else if(state == 2) return 1;
        else if(state == 4) return 2;
        else return -1;
        break;
    case CONFIG_TETRASTATE:
        if(state == 0) return 0;
        else if(state == 1) return 1;
        else if(state == 3) return 2;
        else if(state == 4) return 3;
        else return -1;
        break;
    case CONFIG_PENTASTATE:
        return state;
        break;
    default:
        abort();
    }
}