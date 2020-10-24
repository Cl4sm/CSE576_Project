int
compare_hostaddr(const void *av, const void *bv)
{
    const HostAddressRec *a = av, *b = bv;
    int r;
    if(a->af == 4) {
        if(b->af == 4)
            r = 0;
        else
            r = -1;
    } else {
        if(b->af == 6)
            r = 0;
        else
            r = 1;
    }
    if(dnsQueryIPv6 >= 2)
        return -r;
    else
        return r;
}