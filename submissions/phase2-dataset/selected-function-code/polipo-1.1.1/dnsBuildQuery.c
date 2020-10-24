static int
dnsBuildQuery(int id, char *buf, int offset, int n, AtomPtr name, int af)
{
    int i = offset;
    int type;
    switch(af) {
    case 4: type = 1; break;
    case 6: type = 28; break;
    default: return -EINVAL;
    }

    if(i + 12 >= n) return -1;
    DO_HTONS(&buf[i], id); i += 2;
    DO_HTONS(&buf[i], 1<<8); i += 2;
    DO_HTONS(&buf[i], 1); i += 2;
    DO_HTONS(&buf[i], 0); i += 2;
    DO_HTONS(&buf[i], 0); i += 2;
    DO_HTONS(&buf[i], 0); i += 2;

    i = stringToLabels(buf, i, n, name->string);
    if(i < 0) return -ENAMETOOLONG;
    
    if(i + 4 >= n) return -ENAMETOOLONG;
    DO_HTONS(&buf[i], type); i += 2;
    DO_HTONS(&buf[i], 1); i += 2;
    return i;
}