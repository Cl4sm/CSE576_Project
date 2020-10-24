*euc_to_jis(const char *str)
{
    int len, i, p=0, kanji=0;

    len = strlen(str);
    for(i=0;i<len && p<MAX_CHAR;i++){
        if (str[i]&0x80){
            if (!kanji){
                buffer[p++] = ESC;
                buffer[p++] = '$';
                buffer[p++] = 'B';
                kanji = 1;
            }
            buffer[p++] = (0x7f&str[i]);
            buffer[p++] = (0x7f&str[i+1]);
            i++;
        }
        else{
            if (kanji){
                buffer[p++] = ESC;
                buffer[p++] = '(';
                buffer[p++] = 'J';
                kanji = 0;
            }
            buffer[p++] = str[i];
        }
    }
    if (kanji){
        buffer[p++] = ESC;
        buffer[p++] = '(';
        buffer[p++] = 'J';
        kanji = 0;
    }
    buffer[p]='\0';
    return buffer;
}
