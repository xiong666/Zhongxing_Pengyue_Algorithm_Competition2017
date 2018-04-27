#ifndef __LIB_IO_H__
#define __LIB_IO_H__

#define MAX_EDGE_NUM    (2000 * 20)

extern int read_file(char ** const buff, const unsigned int spec, const char * const filename);

extern void write_result(const char * const buff,const char * const filename);

extern void release_buff(char ** const buff, const int valid_item_num);

#endif

