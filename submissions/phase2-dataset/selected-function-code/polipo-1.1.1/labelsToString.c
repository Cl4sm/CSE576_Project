static int
labelsToString(char *buf, int offset, int n, char *d, int m, int *j_return)
{
    int i = offset, j, k;
    int ll, rc;

    j = 0;
    while(1) {
        if(i >= n) return -1;
        ll = *(unsigned char*)&buf[i]; i++;
        if(ll == 0) {
            break;
        }
        if((ll & (3 << 6)) == (3 << 6)) {
            /* RFC 1035, 4.1.4 */
            int o;
            if(i >= n) return -1;
            o = (ll & ~(3 << 6)) << 8 | *(unsigned char*)&buf[i];
            i++;
            rc = labelsToString(buf, o, n, &d[j], m - j, &k);
            if(rc < 0)
                return -1;
            j += k;
            break;
        } else if((ll & (3 << 6)) == 0) {
            for(k = 0; k < ll; k++) {
                if(i >= n || j >= m) return -1;
                d[j++] = buf[i++];
            }
            if(i >= n) return -1;
            if(buf[i] != '\0') {
                if(j >= m) return -1;
                d[j++] = '.';
            }
        } else {
            return -1;
        }
    }
    *j_return = j;
    return i;
}