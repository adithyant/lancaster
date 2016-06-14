/* watch a storage's change queue and stream its changed values */

#ifndef STREAMER_H
#define STREAMER_H

#include "clock.h"
#include "storage.h"

#ifdef __cplusplus
extern "C" {
#endif

struct streamer;
typedef struct streamer *streamer_handle;

status streamer_create(streamer_handle *pstream, storage_handle store);
status streamer_destroy(streamer_handle *pstream);

status streamer_read_value(streamer_handle stream, identifier id, void* buf,
						   size_t buf_size, microsec *when);

status streamer_read_next_value(streamer_handle stream, void *buf,
								size_t buf_size, microsec *when);

#ifdef __cplusplus
}
#endif

#endif
