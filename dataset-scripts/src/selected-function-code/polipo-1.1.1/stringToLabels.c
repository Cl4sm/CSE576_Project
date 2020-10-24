static int
stringToLabels(char *buf, int offset, int n, char *string)
{
    int i = offset;
    int j = 0, k = 0;
    while(1) {
        while(string[k] != '.' && string[k] != '\0')
            k++;
        if(k >= j + 256) return -1;
        buf[i] = (unsigned char)(k - j); i++; if(i >= n) return -1;
        while(j < k) {
            buf[i] = string[j]; i++; j++; if(i >= n) return -1;
        }
        if(string[j] == '\0') {
            buf[i] = '\0';
            i++; if(i >= n) return -1;
            break;
        }
        j++; k++;
    }

    return i;
}