read_lba(int fd, uint64_t lba, void *buffer, size_t bytes)
{
	int sector_size = get_sector_size(fd);
	off_t offset = lba * sector_size;
	uint64_t lastlba;
        ssize_t bytesread;

	lseek(fd, offset, SEEK_SET);
	bytesread = read(fd, buffer, bytes);

	lastlba = last_lba(fd);
	if (!lastlba)
		return bytesread;

        /* Kludge.  This is necessary to read/write the last
           block of an odd-sized disk, until Linux 2.5.x kernel fixes.
           This is only used by gpt.c, and only to read
           one sector, so we don't have to be fancy.
        */
        if (!bytesread && !(lastlba & 1) && lba == lastlba) {
                bytesread = read_lastoddsector(fd, lba, buffer, bytes);
        }
        return bytesread;
}
