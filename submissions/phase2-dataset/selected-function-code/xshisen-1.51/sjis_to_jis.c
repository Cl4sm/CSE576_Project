*sjis_to_jis(const char *str)
{
    int len, i, p=0, kanji=0;

    len = strlen(str);
    for(i=0;i<len && p<MAX_CHAR;i++){
        if (Csj_h[(unsigned char)str[i]]){
            if (!kanji){
                buffer[p++] = ESC;
                buffer[p++] = '$';
                buffer[p++] = 'B';
                kanji = 1;
            }
            if (0x80&Csj_l[(unsigned char)str[i+1]]){
                buffer[p++] = Csj_h[(unsigned char)str[i]]+1;
                buffer[p++] = (Csj_l[(unsigned char)str[i+1]] & 0x7f);
            }
            else{
                buffer[p++] = Csj_h[(unsigned char)str[i]];
                buffer[p++] = Csj_l[(unsigned char)str[i+1]];
            }
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
