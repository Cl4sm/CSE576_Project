int bh_readcrc(void)
{
  unsigned short check;
  unsigned char word[2];

  check = crc_binh(zero, 2, crc);

  if (bh_read(word, 2) < 2)
    return -1;

  crc = (word[0] << 8) |
        (word[1] << 0);

  if (crc != check)
    {
      ERROR(EINVAL, "CRC checksum error");
      return -1;
    }

  crc = 0x0000;

  return 0;
}
