*sjis_to_euc(const char *str)
{
    int len, i, p=0, kanji=0;

    len = strlen(str);
    for(i=0;i<len && p<MAX_CHAR;i++){
        if (Csj_h[(unsigned char)str[i]]){
            if (0x80&Csj_l[(unsigned char)str[i+1]]){
                buffer[p++] = ((Csj_h[(unsigned char)str[i]]+1) | 0x80);
                buffer[p++] = (Csj_l[(unsigned char)str[i+1]] | 0x80);
            }
            else{
                buffer[p++] = (Csj_h[(unsigned char)str[i]] | 0x80);
                buffer[p++] = (Csj_l[(unsigned char)str[i+1]] | 0x80);
            }
            i++;
        }
        else{
            buffer[p++] = str[i];
        }
    }
    buffer[p]='\0';
    return buffer;
}
