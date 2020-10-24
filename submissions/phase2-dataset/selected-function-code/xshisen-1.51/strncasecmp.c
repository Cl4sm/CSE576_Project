{
    register int i;
    for(i=0; i<n; i++) {
        register char p1 = toupper(s1[i]), p2 = toupper(s2[i]);
        if (p1 == '\0') {
            if (p2 == '\0')
                return 0;
            else
                return 1;
        } else if (p2 == '\0') {
            return -1;
        } else if (p1 < p2) {
            return 1;
        } else if (p1 > p2) {
            return -1;
        }
    }
    return 0;
}
