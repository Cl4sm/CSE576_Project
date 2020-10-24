		      int drive,
		      enum dir direction,
		      int cylinder, int head, 
		      int sector, int size, /* address */
		      char *data, 
		      int bytes,
		      struct floppy_raw_cmd *raw_cmd,
		      int rate)
{
    raw_cmd->data = data;
    raw_cmd->length = bytes;
    
    raw_cmd->rate = rate;
    raw_cmd->flags = FD_RAW_INTR | FD_RAW_NEED_SEEK | 
	FD_RAW_NEED_DISK | FD_RAW_MORE;

    raw_cmd->cmd_count = 9;  
    
    if (direction == READ_) {
	raw_cmd->cmd[0] = FD_READ & ~0x80;
	raw_cmd->flags |= FD_RAW_READ;
    } else {
	raw_cmd->cmd[0] = FD_WRITE & ~0x80;
	raw_cmd->flags |= FD_RAW_WRITE;
    }

    raw_cmd->cmd[1] = (drive & 3) | (head << 2);
    
    raw_cmd->cmd[2] = cylinder;
    raw_cmd->cmd[3] = head;
    raw_cmd->cmd[4] = sector;
    raw_cmd->cmd[5] = size;
    
    raw_cmd->cmd[6] = sector + (bytes >> (size + 7)) - 1;
    raw_cmd->cmd[7] = 0x1b;
    raw_cmd->cmd[8] = 0xff;

    raw_cmd->track = cylinder;
    return 0;
}
