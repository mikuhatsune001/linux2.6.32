/*
 * Au12x0/Au1550 PSC ALSA ASoC audio support.
 *
 * (c) 2007-2008 MSC Vertriebsges.m.b.H.,
 *	Manuel Lauss <mano@roarinelk.homelinux.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Au1xxx-PSC I2S glue.
 *
 * NOTE: all of these drivers can only work with a SINGLE instance
 *	 of a PSC. Multiple independent audio devices are impossible
 *	 with ASoC v1.
 * NOTE: so far only PSC slave mode (bit- and frameclock) is supported.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/suspend.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/initval.h>
#include <sound/soc.h>
#include <asm/mach-au1x00/au1000.h>
#include <asm/mach-au1x00/au1xxx_psc.h>

#include "psc.h"

/* supported I2S DAI hardware formats */
#define AU1XPSC_I2S_DAIFMT \
	(SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_LEFT_J |	\
	 SND_SOC_DAIFMT_NB_NF)

/* supported I2S direction */
#define AU1XPSC_I2S_DIR \
	(SND_SOC_DAIDIR_PLAYBACK | SND_SOC_DAIDIR_CAPTURE)

#define AU1XPSC_I2S_RATES \
	SNDRV_PCM_RATE_8000_192000

#define AU1XPSC_I2S_FMTS \
	(SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S24_LE)

#define I2SSTAT_BUSY(stype)	\
	((stype) == PCM_TX ? PSC_I2SSTAT_TB : PSC_I2SSTAT_RB)
#define I2SPCR_START(stype)	\
	((stype) == PCM_TX ? PSC_I2SPCR_TS : PSC_I2SPCR_RS)
#define I2SPCR_STOP(stype)	\
	((stype) == PCM_TX ? PSC_I2SPCR_TP : PSC_I2SPCR_RP)
#define I2SPCR_CLRFIFO(stype)	\
	((stype) == PCM_TX ? PSC_I2SPCR_TC : PSC_I2SPCR_RC)


/* instance data. There can be only one, MacLeod!!!! */
static struct au1xpsc_audio_data *au1xpsc_i2s_workdata;

static int au1xpsc_i2s_set_fmt(struct snd_soc_dai *cpu_dai,
			       unsigned int fmt)
{
	struct au1xpsc_audio_data *pscdata = au1xpsc_i2s_workdata;
	unsigned long ct;
	int ret;

	ret = -EINVAL;

	ct = pscdata->cfg;

	ct &= ~(PSC_I2SCFG_XM | PSC_I2SCFG_MLJ);	/* left-justified */
	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_I2S:
		ct |= PSC_I2SCFG_XM;	/* enable I2S mode */
		break;
	case SND_SOC_DAIFMT_MSB:
		break;
	case SND_SOC_DAIFMT_LSB:
		ct |= PSC_I2SCFG_MLJ;	/* LSB (right-) justified */
		break;
	default:
		goto out;
	}

	ct &= ~(PSC_I2SCFG_BI | PSC_I2SCFG_WI);		/* IB-IF */
	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_NF:
		ct |= PSC_I2SCFG_BI | PSC_I2SCFG_WI;
		break;
	case SND_SOC_DAIFMT_NB_IF:
		ct |= PSC_I2SCFG_BI;
		break;
	case SND_SOC_DAIFMT_IB_NF:
		ct |= PSC_I2SCFG_WI;
		break;
	case SND_SOC_DAIFMT_IB_IF:
		break;
	default:
		goto out;
	}

	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBM_CFM:	/* CODEC master */
		ct |= PSC_I2SCFG_MS;	/* PSC I2S slave mode */
		break;
	case SND_SOC_DAIFMT_CBS_CFS:	/* CODEC slave */
		ct &= ~PSC_I2SCFG_MS;	/* PSC I2S Master mode */
		break;
	default:
		goto out;
	}

	pscdata->cfg = ct;
	ret = 0;
out:
	return ret;
}

static int au1xpsc_i2s_hw_params(struct snd_pcm_substream *substream,
				 struct snd_pcm_hw_params *params,
				 struct snd_soc_dai *dai)
{
	struct au1xpsc_audio_data *pscdata = au1xpsc_i2s_workdata;

	int cfgbits;
	unsigned long stat;

	/* check if the PSC is already streaming data */
	stat = au_readl(I2S_STAT(pscdata));
	if (stat & (PSC_I2SSTAT_TB | PSC_I2SSTAT_RB)) {
		/* reject parameters not currently set up in hardware */
		cfgbits = au_readl(I2S_CFG(pscdata));
		if ((PSC_I2SCFG_GET_LEN(cfgbits) != params->msbits) ||
		    (params_rate(params) != pscdata->rate))
			return -EINVAL;
	} else {
		/* set sample bitdepth */
		pscdata->cfg &= ~(0x1f << 4);
		pscdata->cfg |= PSC_I2SCFG_SET_LEN(params->msbits);
		/* remember current rate for other stream */
		pscdata->rate = params_rate(params);
	}
	return 0;
}

/* Configure PSC late:  on my devel systems the codec  is I2S master and
 * supplies the i2sbitclock __AND__ i2sMclk (!) to the PSC unit.  ASoC
 * uses aggressive PM and  switches the codec off  when it is not in use
 * which also means the PSC unit doesn't get any clocks and is therefore
 * dead. That's why this chunk here gets called from the trigger callback
 * because I can be reasonably certain the codec is driving the clocks.
 */
static int au1xpsc_i2s_configure(struct au1xpsc_audio_data *pscdata)
{
	unsigned long tmo;

	/* bring PSC out of sleep, and configure I2S unit */
	au_writel(PSC_CTRL_ENABLE, PSC_CTRL(pscdata));
	au_sync();

	tmo = 1000000;
	while (!(au_readl(I2S_STAT(pscdata)) & PSC_I2SSTAT_SR) && tmo)
		tmo--;

	if (!tmo)
		goto psc_err;

	au_writel(0, I2S_CFG(pscdata));
	au_sync();
	au_writel(pscdata->cfg | PSC_I2SCFG_DE_ENABLE, I2S_CFG(pscdata));
	au_sync();

	/* wait for I2S controller to become ready */
	tmo = 1000000;
	while (!(au_readl(I2S_STAT(pscdata)) & PSC_I2SSTAT_DR) && tmo)
		tmo--;

	if (tmo)
		return 0;

psc_err:
	au_writel(0, I2S_CFG(pscdata));
	au_writel(PSC_CTRL_SUSPEND, PSC_CTRL(pscdata));
	au_sync();
	return -ETIMEDOUT;
}

static int au1xpsc_i2s_start(struct au1xpsc_audio_data *pscdata, int stype)
{
	unsigned long tmo, stat;
	int ret;

	ret = 0;

	/* if both TX and RX are idle, configure the PSC  */
	stat = au_readl(I2S_STAT(pscdata));
	if (!(stat & (PSC_I2SSTAT_TB | PSC_I2SSTAT_RB))) {
		ret = au1xpsc_i2s_configure(pscdata);
		if (ret)
			goto out;
	}

	au_writel(I2SPCR_CLRFIFO(stype), I2S_PCR(pscdata));
	au_sync();
	au_writel(I2SPCR_START(stype), I2S_PCR(pscdata));
	au_sync();

	/* wait for start confirmation */
	tmo = 1000000;
	while (!(au_readl(I2S_STAT(pscdata)) & I2SSTAT_BUSY(stype)) && tmo)
		tmo--;

	if (!tmo) {
		au_writel(I2SPCR_STOP(stype), I2S_PCR(pscdata));
		au_sync();
		ret = -ETIMEDOUT;
	}
out:
	return ret;
}

static int au1xpsc_i2s_stop(struct au1xpsc_audio_data *pscdata, int stype)
{
	unsigned long tmo, stat;

	au_writel(I2SPCR_STOP(stype), I2S_PCR(pscdata));
	au_sync();

	/* wait for stop confirmation */
	tmo = 1000000;
	while ((au_readl(I2S_STAT(pscdata)) & I2SSTAT_BUSY(stype)) && tmo)
		tmo--;

	/* if both TX and RX are idle, disable PSC */
	stat = au_readl(I2S_STAT(pscdata));
	if (!(stat & (PSC_I2SSTAT_TB | PSC_I2SSTAT_RB))) {
		au_writel(0, I2S_CFG(pscdata));
		au_sync();
		au_writel(PSC_CTRL_SUSPEND, PSC_CTRL(pscdata));
		au_sync();
	}
	return 0;
}

static int au1xpsc_i2s_trigger(struct snd_pcm_substream *substream, int cmd,
			       struct snd_soc_dai *dai)
{
	struct au1xpsc_audio_data *pscdata = au1xpsc_i2s_workdata;
	int ret, stype = SUBSTREAM_TYPE(substream);

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
		ret = au1xpsc_i2s_start(pscdata, stype);
		break;
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
		ret = au1xpsc_i2s_stop(pscdata, stype);
		break;
	default:
		ret = -EINVAL;
	}
	return ret;
}

static int au1xpsc_i2s_probe(struct platform_device *pdev,
			     struct snd_soc_dai *dai)
{
	struct resource *r;
	unsigned long sel;
	int ret;

	if (au1xpsc_i2s_workdata)
		return -EBUSY;

	au1xpsc_i2s_workdata =
		kzalloc(sizeof(struct au1xpsc_audio_data), GFP_KERNEL);
	if (!au1xpsc_i2s_workdata)
		return -ENOMEM;

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r) {
		ret = -ENODEV;
		goto out0;
	}

	ret = -EBUSY;
	au1xpsc_i2s_workdata->ioarea =
		request_mem_region(r->start, r->end - r->start + 1,
					"au1xpsc_i2s");
	if (!au1xpsc_i2s_workdata->ioarea)
		goto out0;

	au1xpsc_i2s_workdata->mmio = ioremap(r->start, 0xffff);
	if (!au1xpsc_i2s_workdata->mmio)
		goto out1;

	/* preserve PSC clock source set up by platform (dev.platform_data
	 * is already occupied by soc layer)
	 */
	sel = au_readl(PSC_SEL(au1xpsc_i2s_workdata)) & PSC_SEL_CLK_MASK;
	au_writel(PSC_CTRL_DISABLE, PSC_CTRL(au1xpsc_i2s_workdata));
	au_sync();
	au_writel(PSC_SEL_PS_I2SMODE | sel, PSC_SEL(au1xpsc_i2s_workdata));
	au_writel(0, I2S_CFG(au1xpsc_i2s_workdata));
	au_sync();

	/* preconfigure: set max rx/tx fifo depths */
	au1xpsc_i2s_workdata->cfg |=
			PSC_I2SCFG_RT_FIFO8 | PSC_I2SCFG_TT_FIFO8;

	/* don't wait for I2S core to become ready now; clocks may not
	 * be running yet; depending on clock input for PSC a wait might
	 * time out.
	 */

	return 0;

out1:
	release_resource(au1xpsc_i2s_workdata->ioarea);
	kfree(au1xpsc_i2s_workdata->ioarea);
out0:
	kfree(au1xpsc_i2s_workdata);
	au1xpsc_i2s_workdata = NULL;
	return ret;
}

static void au1xpsc_i2s_remove(struct platform_device *pdev,
			       struct snd_soc_dai *dai)
{
	au_writel(0, I2S_CFG(au1xpsc_i2s_workdata));
	au_sync();
	au_writel(PSC_CTRL_DISABLE, PSC_CTRL(au1xpsc_i2s_workdata));
	au_sync();

	iounmap(au1xpsc_i2s_workdata->mmio);
	release_resource(au1xpsc_i2s_workdata->ioarea);
	kfree(au1xpsc_i2s_workdata->ioarea);
	kfree(au1xpsc_i2s_workdata);
	au1xpsc_i2s_workdata = NULL;
}

static int au1xpsc_i2s_suspend(struct snd_soc_dai *cpu_dai)
{
	/* save interesting register and disable PSC */
	au1xpsc_i2s_workdata->pm[0] =
		au_readl(PSC_SEL(au1xpsc_i2s_workdata));

	au_writel(0, I2S_CFG(au1xpsc_i2s_workdata));
	au_sync();
	au_writel(PSC_CTRL_DISABLE, PSC_CTRL(au1xpsc_i2s_workdata));
	au_sync();

	return 0;
}

static int au1xpsc_i2s_resume(struct snd_soc_dai *cpu_dai)
{
	/* select I2S mode and PSC clock */
	au_writel(PSC_CTRL_DISABLE, PSC_CTRL(au1xpsc_i2s_workdata));
	au_sync();
	au_writel(0, PSC_SEL(au1xpsc_i2s_workdata));
	au_sync();
	au_writel(au1xpsc_i2s_workdata->pm[0],
			PSC_SEL(au1xpsc_i2s_workdata));
	au_sync();

	return 0;
}

static struct snd_soc_dai_ops au1xpsc_i2s_dai_ops = {
	.trigger	= au1xpsc_i2s_trigger,
	.hw_params	= au1xpsc_i2s_hw_params,
	.set_fmt	= au1xpsc_i2s_set_fmt,
};

struct snd_soc_dai au1xpsc_i2s_dai = {
	.name			= "au1xpsc_i2s",
	.probe			= au1xpsc_i2s_probe,
	.remove			= au1xpsc_i2s_remove,
	.suspend		= au1xpsc_i2s_suspend,
	.resume			= au1xpsc_i2s_resume,
	.playback = {
		.rates		= AU1XPSC_I2S_RATES,
		.formats	= AU1XPSC_I2S_FMTS,
		.channels_min	= 2,
		.channels_max	= 8,	/* 2 without external help */
	},
	.capture = {
		.rates		= AU1XPSC_I2S_RATES,
		.formats	= AU1XPSC_I2S_FMTS,
		.channels_min	= 2,
		.channels_max	= 8,	/* 2 without external help */
	},
	.ops = &au1xpsc_i2s_dai_ops,
};
EXPORT_SYMBOL(au1xpsc_i2s_dai);

static int __init au1xpsc_i2s_init(void)
{
	au1xpsc_i2s_workdata = NULL;
	return snd_soc_register_dai(&au1xpsc_i2s_dai);
}

static void __exit au1xpsc_i2s_exit(void)
{
	snd_soc_unregister_dai(&au1xpsc_i2s_dai);
}

module_init(au1xpsc_i2s_init);
module_exit(au1xpsc_i2s_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Au12x0/Au1550 PSC I2S ALSA ASoC audio driver");
MODULE_AUTHOR("Manuel Lauss <mano@roarinelk.homelinux.net>");
