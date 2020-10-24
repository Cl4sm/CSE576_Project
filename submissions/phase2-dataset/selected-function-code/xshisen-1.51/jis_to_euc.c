*jis_to_euc(const char *str)
{
    int len, i, p=0, kanji=0;

    len = strlen(str);
    for(i=0;i<len && p<MAX_CHAR;i++){
        if (str[i]==ESC && str[i+1]=='$' &&
            (str[i+2]=='B' || str[i+2]=='@')){
            kanji = 1;
            i += 2;
        }
        else if (str[i]==ESC && str[i+1]=='(' &&
                 (str[i+2]=='B' || str[i+2]=='J')){
            kanji = 0;
            i += 2;
        }
        else if (kanji){
            buffer[p++] = ( str[i] | 0x80 );
            buffer[p++] = ( str[i+1] | 0x80 );
            i++;
        }
        else buffer[p++]=str[i];
    }
    buffer[p]='\0';
    return buffer;
}