#ifndef EMS_UPDATE_H
#define EMS_UPDATE_H

#include "ems.h"
#include "image.h"
#include "queue.h"

#define UPDATE_NBSLOTS 3

/*
 * struct updates: list of I/O commands generated by image_update()
 *
 * Commands:
 *    writef
 *       Write the file of "romsize" bytes represented by "fileinfo" to the
 *       offset "dstofs".
 *
 *    move
 *       Move a ROM of "romsize" bytes from offset "srcofs" to offset "dstofs".
 * 
 *    read
 *       Read a ROM of "romsize" bytes located at offset "srcofs" in the
 *       temporary location "slot". "slot" is a number between 0 and 2. The ROM
 *       size will not be higher than 64 KB.
 *
 *    write
 *       Write a ROM of "romsize" bytes saved in the temporary location "slot"
 *       to offset "dstofs".
 *
 *    erase
 *       Erase the erase-block starting at offset "dstofs". This will write
 *       some bytes (32 or 64) at the beginning of the block.
 */

SIMPLEQ_HEAD(updates, update);

struct update {
    SIMPLEQ_ENTRY(update) updates;

    enum {
        UPDATE_CMD_WRITEF, UPDATE_CMD_MOVE, UPDATE_CMD_WRITE,
        UPDATE_CMD_READ, UPDATE_CMD_ERASE
    } cmd;

    struct rom *rom;

    union {
        int slot;
        ems_size_t offset;
    } u;

#define update_writef_fileinfo rom->source.u.fileinfo
#define update_writef_dstofs rom->offset
#define update_writef_size rom->romsize

#define update_move_srcofs rom->source.u.origoffset
#define update_move_dstofs rom->offset
#define update_move_size rom->romsize

#define update_read_srcofs rom->source.u.origoffset
#define update_read_dstslot u.slot
#define update_read_size rom->romsize

#define update_write_dstofs rom->offset
#define update_write_srcslot u.slot
#define update_write_size rom->romsize

#define update_erase_dstofs u.offset
};

#define updates_foreach(us, u) SIMPLEQ_FOREACH(u, us, updates)
#define updates_insert_tail(us, u) SIMPLEQ_INSERT_TAIL(us, u, updates)
#define updates_next(u) SIMPLEQ_NEXT(u, updates)
#define updates_init(us) SIMPLEQ_INIT(us)

int image_update(struct image*, struct updates**);

#endif /* EMS_UPDATE_H */
