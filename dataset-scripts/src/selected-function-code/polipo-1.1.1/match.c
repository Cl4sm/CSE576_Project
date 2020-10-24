static int
match(int af, unsigned char *data, NetAddressPtr list)
{
    int i;
#ifdef HAVE_IPv6
    static const unsigned char v6mapped[] =
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF };
#endif

    i = 0;
    while(list[i].af != 0) {
        if(af == 4 && list[i].af == 4) {
            if(bitmatch(data, list[i].data,
                        list[i].prefix >= 0 ? list[i].prefix : 32))
                return 1;
#ifdef HAVE_IPv6
        } else if(af == 6 && list[i].af == 6) {
            if(bitmatch(data, list[i].data,
                        list[i].prefix >= 0 ? list[i].prefix : 128))
                return 1;
        } else if(af == 6 && list[i].af == 4) {
            if(bitmatch(data, v6mapped, 96)) {
                if(bitmatch(data + 12, list[i].data,
                            list[i].prefix >= 0 ? list[i].prefix : 32))
                    return 1;
            }
        } else if(af == 4 && list[i].af == 6) {
            if(bitmatch(list[i].data, v6mapped, 96)) {
                if(bitmatch(data, list[i].data + 12,
                            list[i].prefix >= 96 ?
                            list[i].prefix - 96 : 32))
                    return 1;
            }
#endif
        } else {
            abort();
        }
        i++;
    }
    return 0;
}