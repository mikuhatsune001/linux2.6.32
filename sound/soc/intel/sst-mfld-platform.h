/*
 *  sst_mfld_platform.h - Intel MID Platform driver header file
 *
 *  Copyright (C) 2010 Intel Corp
 *  Author: Vinod Koul <vinod.koul@intel.com>
 *  Author: Harsha Priya <priya.harsha@intel.com>
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#ifndef __SST_PLATFORMDRV_H__
#define __SST_PLATFORMDRV_H__

#include "sst-mfld-dsp.h"

extern struct sst_device *sst;

#define SST_MONO		1
#define SST_STEREO		2
#define SST_MAX_CAP		5

#define SST_MAX_BUFFER		(800*1024)
#define SST_MIN_BUFFER		(800*1024)
#define SST_MIN_PERIOD_BYTES	32
#define SST_MAX_PERIOD_BYTES	SST_MAX_BUFFER
#define SST_MIN_PERIODS		2
#define SST_MAX_PERIODS		(1024*2)
#define SST_FIFO_SIZE		0

struct pcm_stream_info {
	int str_id;
	void *arg;
	void (*period_elapsed) (void *arg);
	unsigned long long buffer_ptr;
	unsigned long long pcm_delay;
	int sfreq;
};

enum sst_drv_status {
	SST_PLATFORM_INIT = 1,
	SST_PLATFORM_STARTED,
	SST_PLATFORM_RUNNING,
	SST_PLATFORM_PAUSED,
	SST_PLATFORM_DROPPED,
};

enum sst_controls {
	SST_SND_ALLOC =			0x00,
	SST_SND_PAUSE =			0x01,
	SST_SND_RESUME =		0x02,
	SST_SND_DROP =			0x03,
	SST_SND_FREE =			0x04,
	SST_SND_BUFFER_POINTER =	0x05,
	SST_SND_STREAM_INIT =		0x06,
	SST_SND_START	 =		0x07,
	SST_SET_BYTE_STREAM =           0x100A,
	SST_GET_BYTE_STREAM =           0x100B,
	SST_MAX_CONTROLS = SST_GET_BYTE_STREAM,
};

enum sst_stream_ops {
	STREAM_OPS_PLAYBACK = 0,
	STREAM_OPS_CAPTURE,
};

enum sst_audio_device_type {
	SND_SST_DEVICE_HEADSET = 1,
	SND_SST_DEVICE_IHF,
	SND_SST_DEVICE_VIBRA,
	SND_SST_DEVICE_HAPTIC,
	SND_SST_DEVICE_CAPTURE,
	SND_SST_DEVICE_COMPRESS,
};

/* PCM Parameters */
struct sst_pcm_params {
	u16 codec;	/* codec type */
	u8 num_chan;	/* 1=Mono, 2=Stereo */
	u8 pcm_wd_sz;	/* 16/24 - bit*/
	u32 reserved;	/* Bitrate in bits per second */
	u32 sfreq;	/* Sampling rate in Hz */
	u32 ring_buffer_size;
	u32 period_count;	/* period elapsed in samples*/
	u32 ring_buffer_addr;
};

struct sst_stream_params {
	u32 result;
	u32 stream_id;
	u8 codec;
	u8 ops;
	u8 stream_type;
	u8 device_type;
	struct sst_pcm_params sparams;
};

struct sst_compress_cb {
	void *param;
	void (*compr_cb)(void *param);
	void *drain_cb_param;
	void (*drain_notify)(void *param);
};

struct compress_sst_ops {
	const char *name;
	int (*open) (struct snd_sst_params *str_params,
			struct sst_compress_cb *cb);
	int (*control) (unsigned int cmd, unsigned int str_id);
	int (*tstamp) (unsigned int str_id, struct snd_compr_tstamp *tstamp);
	int (*ack) (unsigned int str_id, unsigned long bytes);
	int (*close) (unsigned int str_id);
	int (*get_caps) (struct snd_compr_caps *caps);
	int (*get_codec_caps) (struct snd_compr_codec_caps *codec);
	int (*set_metadata) (unsigned int str_id,
			struct snd_compr_metadata *mdata);

};

struct sst_ops {
	int (*open) (struct snd_sst_params *str_param);
	int (*device_control) (int cmd, void *arg);
	int (*set_generic_params)(enum sst_controls cmd, void *arg);
	int (*close) (unsigned int str_id);
};

struct sst_runtime_stream {
	int     stream_status;
	unsigned int id;
	size_t bytes_written;
	struct pcm_stream_info stream_info;
	struct sst_ops *ops;
	struct compress_sst_ops *compr_ops;
	spinlock_t	status_lock;
};

struct sst_device {
	char *name;
	struct device *dev;
	struct sst_ops *ops;
	struct platform_device *pdev;
	struct compress_sst_ops *compr_ops;
};

struct sst_data;
void sst_set_stream_status(struct sst_runtime_stream *stream, int state);
int sst_fill_stream_params(void *substream, const struct sst_data *ctx,
			   struct snd_sst_params *str_params, bool is_compress);

struct sst_algo_int_control_v2 {
	struct soc_mixer_control mc;
	u16 module_id; /* module identifieer */
	u16 pipe_id; /* location info: pipe_id + instance_id */
	u16 instance_id;
	unsigned int value; /* Value received is stored here */
};
struct sst_data {
	struct platform_device *pdev;
	struct sst_platform_data *pdata;
	struct mutex lock;
};
int sst_register_dsp(struct sst_device *sst);
int sst_unregister_dsp(struct sst_device *sst);
#endif
