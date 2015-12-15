/*
 * Copyright (C) 2010 The Chromium OS Authors <chromium-os-dev@chromium.org>
 *                    All Rights Reserved.
 *
 * This file is released under the GPL.
 *
 * Provide error types for use when creating a custom error handler.
 * See Documentation/device-mapper/dm-verity.txt
 */
#ifndef DM_VERITY_H
#define DM_VERITY_H

#include <linux/notifier.h>

struct dm_verity_error_state {
	int code;
	int transient;  /* Likely to not happen after a reboot */
	u64 block;
	const char *message;

	sector_t dev_start;
	sector_t dev_len;
	struct block_device *dev;

	sector_t hash_dev_start;
	sector_t hash_dev_len;
	struct block_device *hash_dev;

	/* Final behavior after all notifications are completed. */
	int behavior;
};

/* This enum must be matched to allowed_error_behaviors in dm-verity.c */
enum dm_verity_error_behavior {
	DM_VERITY_ERROR_BEHAVIOR_EIO = 0,
	DM_VERITY_ERROR_BEHAVIOR_PANIC,
	DM_VERITY_ERROR_BEHAVIOR_NONE,
	DM_VERITY_ERROR_BEHAVIOR_NOTIFY
};


int dm_verity_register_error_notifier(struct notifier_block *nb);
int dm_verity_unregister_error_notifier(struct notifier_block *nb);

extern void verity_status(struct dm_target *ti, status_type_t type,
			unsigned status_flags, char *result, unsigned maxlen);
extern int verity_ioctl(struct dm_target *ti, unsigned cmd,
			unsigned long arg);
extern int verity_merge(struct dm_target *ti, struct bvec_merge_data *bvm,
			struct bio_vec *biovec, int max_size);
extern int verity_iterate_devices(struct dm_target *ti,
				iterate_devices_callout_fn fn, void *data);
extern void verity_io_hints(struct dm_target *ti, struct queue_limits *limits);
extern void verity_dtr(struct dm_target *ti);
extern int verity_ctr(struct dm_target *ti, unsigned argc, char **argv);
extern int verity_map(struct dm_target *ti, struct bio *bio);

#endif /* DM_VERITY_H */
