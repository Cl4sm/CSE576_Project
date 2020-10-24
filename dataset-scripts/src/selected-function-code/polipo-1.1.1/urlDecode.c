AtomListPtr
urlDecode(char *buf, int n)
{
    char mybuf[500];
    int i, j = 0;
    AtomListPtr list;
    AtomPtr atom;

    list = makeAtomList(NULL, 0);
    if(list == NULL)
        return NULL;

    i = 0;
    while(i < n) {
        if(buf[i] == '%') {
            int a, b;
            if(i + 3 > n)
                goto fail;
            a = h2i(buf[i + 1]);
            b = h2i(buf[i + 2]);
            if(a < 0 || b < 0)
                goto fail;
            mybuf[j++] = (char)((a << 4) | b);
            i += 3;
            if(j >= 500) goto fail;
        } else if(buf[i] == '&') {
            atom = internAtomN(mybuf, j);
            if(atom == NULL)
                goto fail;
            atomListCons(atom, list);
            j = 0;
            i++;
        } else {
            mybuf[j++] = buf[i++];
            if(j >= 500) goto fail;
        }
    }

    atom = internAtomN(mybuf, j);
    if(atom == NULL)
        goto fail;
    atomListCons(atom, list);
    return list;

 fail:
    destroyAtomList(list);
    return NULL;
}