/*
 * Copyright (c) 2005-2009 Brocade Communications Systems, Inc.
 * All rights reserved
 * www.brocade.com
 *
 * Linux driver for Brocade Fibre Channel Host Bus Adapter.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License (GPL) Version 2 as
 * published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#ifndef __BFA_DEFS_PPORT_H__
#define __BFA_DEFS_PPORT_H__

#include <bfa_os_inc.h>
#include <protocol/fc.h>
#include <defs/bfa_defs_types.h>
#include <defs/bfa_defs_qos.h>
#include <cna/pstats/phyport_defs.h>

/* Modify char* port_stt[] in bfal_port.c if a new state was added */
enum bfa_pport_states {
	BFA_PPORT_ST_UNINIT 		= 1,
	BFA_PPORT_ST_ENABLING_QWAIT 	= 2,
	BFA_PPORT_ST_ENABLING 		= 3,
	BFA_PPORT_ST_LINKDOWN 		= 4,
	BFA_PPORT_ST_LINKUP 		= 5,
	BFA_PPORT_ST_DISABLING_QWAIT 	= 6,
	BFA_PPORT_ST_DISABLING		= 7,
	BFA_PPORT_ST_DISABLED 		= 8,
	BFA_PPORT_ST_STOPPED 		= 9,
	BFA_PPORT_ST_IOCDOWN 		= 10,
	BFA_PPORT_ST_IOCDIS 		= 11,
	BFA_PPORT_ST_FWMISMATCH		= 12,
	BFA_PPORT_ST_MAX_STATE,
};

/**
 * 	Port speed settings. Each specific speed is a bit field. Use multiple
 *      bits to specify speeds to be selected for auto-negotiation.
 */
enum bfa_pport_speed {
	BFA_PPORT_SPEED_UNKNOWN = 0,
	BFA_PPORT_SPEED_1GBPS 	= 1,
	BFA_PPORT_SPEED_2GBPS 	= 2,
	BFA_PPORT_SPEED_4GBPS 	= 4,
	BFA_PPORT_SPEED_8GBPS 	= 8,
	BFA_PPORT_SPEED_10GBPS 	= 10,
	BFA_PPORT_SPEED_AUTO =
		(BFA_PPORT_SPEED_1GBPS | BFA_PPORT_SPEED_2GBPS |
		 BFA_PPORT_SPEED_4GBPS | BFA_PPORT_SPEED_8GBPS),
};

/**
 * 		Port operational type (in sync with SNIA port type).
 */
enum bfa_pport_type {
	BFA_PPORT_TYPE_UNKNOWN = 1,	/*  port type is unkown */
	BFA_PPORT_TYPE_TRUNKED = 2,	/*  Trunked mode */
	BFA_PPORT_TYPE_NPORT   = 5,	/*  P2P with switched fabric */
	BFA_PPORT_TYPE_NLPORT  = 6,	/*  public loop */
	BFA_PPORT_TYPE_LPORT   = 20,	/*  private loop */
	BFA_PPORT_TYPE_P2P     = 21,	/*  P2P with no switched fabric */
	BFA_PPORT_TYPE_VPORT   = 22,	/*  NPIV - virtual port */
};

/**
 * 		Port topology setting. A port's topology and fabric login status
 * 		determine its operational type.
 */
enum bfa_pport_topology {
	BFA_PPORT_TOPOLOGY_NONE = 0,	/*  No valid topology */
	BFA_PPORT_TOPOLOGY_P2P  = 1,	/*  P2P only */
	BFA_PPORT_TOPOLOGY_LOOP = 2,	/*  LOOP topology */
	BFA_PPORT_TOPOLOGY_AUTO = 3,	/*  auto topology selection */
};

/**
 * 		Physical port loopback types.
 */
enum bfa_pport_opmode {
	BFA_PPORT_OPMODE_NORMAL   = 0x00, /*  normal non-loopback mode */
	BFA_PPORT_OPMODE_LB_INT   = 0x01, /*  internal loop back */
	BFA_PPORT_OPMODE_LB_SLW   = 0x02, /*  serial link wrapback (serdes) */
	BFA_PPORT_OPMODE_LB_EXT   = 0x04, /*  external loop back (serdes) */
	BFA_PPORT_OPMODE_LB_CBL   = 0x08, /*  cabled loop back */
	BFA_PPORT_OPMODE_LB_NLINT = 0x20, /*  NL_Port internal loopback */
};

#define BFA_PPORT_OPMODE_LB_HARD(_mode)			\
	((_mode == BFA_PPORT_OPMODE_LB_INT) ||		\
     (_mode == BFA_PPORT_OPMODE_LB_SLW) ||		\
     (_mode == BFA_PPORT_OPMODE_LB_EXT))

/**
		Port State (in sync with SNIA port state).
 */
enum bfa_pport_snia_state {
	BFA_PPORT_STATE_UNKNOWN  = 1,	/*  port is not initialized */
	BFA_PPORT_STATE_ONLINE   = 2,	/*  port is ONLINE */
	BFA_PPORT_STATE_DISABLED = 3,	/*  port is disabled by user */
	BFA_PPORT_STATE_BYPASSED = 4,	/*  port is bypassed (in LOOP) */
	BFA_PPORT_STATE_DIAG     = 5,	/*  port diagnostics is active */
	BFA_PPORT_STATE_LINKDOWN = 6,	/*  link is down */
	BFA_PPORT_STATE_LOOPBACK = 8,	/*  port is looped back */
};

/**
 * 		Port link state
 */
enum bfa_pport_linkstate {
	BFA_PPORT_LINKUP 	 = 1,	/*  Physical port/Trunk link up */
	BFA_PPORT_LINKDOWN 	 = 2,	/*  Physical port/Trunk link down */
	BFA_PPORT_TRUNK_LINKDOWN = 3,	/*  Trunk link down (new tmaster) */
};

/**
 * 		Port link state event
 */
#define bfa_pport_event_t enum bfa_pport_linkstate

/**
 * 		Port link state reason code
 */
enum bfa_pport_linkstate_rsn {
	BFA_PPORT_LINKSTATE_RSN_NONE		= 0,
	BFA_PPORT_LINKSTATE_RSN_DISABLED 	= 1,
	BFA_PPORT_LINKSTATE_RSN_RX_NOS 		= 2,
	BFA_PPORT_LINKSTATE_RSN_RX_OLS 		= 3,
	BFA_PPORT_LINKSTATE_RSN_RX_LIP 		= 4,
	BFA_PPORT_LINKSTATE_RSN_RX_LIPF7 	= 5,
	BFA_PPORT_LINKSTATE_RSN_SFP_REMOVED 	= 6,
	BFA_PPORT_LINKSTATE_RSN_PORT_FAULT 	= 7,
	BFA_PPORT_LINKSTATE_RSN_RX_LOS 		= 8,
	BFA_PPORT_LINKSTATE_RSN_LOCAL_FAULT 	= 9,
	BFA_PPORT_LINKSTATE_RSN_REMOTE_FAULT 	= 10,
	BFA_PPORT_LINKSTATE_RSN_TIMEOUT 	= 11,



	/* CEE related reason codes/errors */
	CEE_LLDP_INFO_AGED_OUT       = 20,
	CEE_LLDP_SHUTDOWN_TLV_RCVD   = 21,
	CEE_PEER_NOT_ADVERTISE_DCBX  = 22,
	CEE_PEER_NOT_ADVERTISE_PG    = 23,
	CEE_PEER_NOT_ADVERTISE_PFC   = 24,
	CEE_PEER_NOT_ADVERTISE_FCOE  = 25,
	CEE_PG_NOT_COMPATIBLE        = 26,
	CEE_PFC_NOT_COMPATIBLE       = 27,
	CEE_FCOE_NOT_COMPATIBLE      = 28,
	CEE_BAD_PG_RCVD              = 29,
	CEE_BAD_BW_RCVD              = 30,
	CEE_BAD_PFC_RCVD             = 31,
	CEE_BAD_FCOE_PRI_RCVD        = 32,
	CEE_FCOE_PRI_PFC_OFF         = 33,
	CEE_DUP_CONTROL_TLV_RCVD     = 34,
	CEE_DUP_FEAT_TLV_RCVD        = 35,
	CEE_APPLY_NEW_CFG            = 36,	/* reason, not an error */
	CEE_PROTOCOL_INIT            = 37,  /* reason, not an error */
	CEE_PHY_LINK_DOWN            = 38,
	CEE_LLS_FCOE_ABSENT          = 39,
	CEE_LLS_FCOE_DOWN            = 40
};

/**
 *      Default Target Rate Limiting Speed.
 */
#define BFA_PPORT_DEF_TRL_SPEED  BFA_PPORT_SPEED_1GBPS

/**
 *      Physical port configuration
 */
struct bfa_pport_cfg_s {
	u8         topology;	/*  bfa_pport_topology		*/
	u8         speed;		/*  enum bfa_pport_speed	*/
	u8         trunked;	/*  trunked or not		*/
	u8         qos_enabled;	/*  qos enabled or not		*/
	u8         trunk_ports;	/*  bitmap of trunked ports	*/
	u8         cfg_hardalpa;	/*  is hard alpa configured	*/
	u16        maxfrsize;	/*  maximum frame size		*/
	u8         hardalpa;	/*  configured hard alpa	*/
	u8         rx_bbcredit;	/*  receive buffer credits	*/
	u8         tx_bbcredit;	/*  transmit buffer credits	*/
	u8         ratelimit;	/*  ratelimit enabled or not	*/
	u8         trl_def_speed;	/*  ratelimit default speed	*/
	u8		rsvd[3];
	u16   	path_tov;	/*  device path timeout	*/
	u16   	q_depth;	/*  SCSI Queue depth		*/
};

/**
 * 		Port attribute values.
 */
struct bfa_pport_attr_s {
	/*
	 * Static fields
	 */
	wwn_t           nwwn;		/*  node wwn */
	wwn_t           pwwn;		/*  port wwn */
	enum fc_cos     cos_supported;	/*  supported class of services */
	u32        rsvd;
	struct fc_symname_s    port_symname;	/*  port symbolic name */
	enum bfa_pport_speed speed_supported; /*  supported speeds */
	bfa_boolean_t   pbind_enabled;	/*  Will be set if Persistent binding
					 *   enabled. Relevant only in Windows
					 */

	/*
	 * Configured values
	 */
	struct bfa_pport_cfg_s pport_cfg;	/*  pport cfg */

	/*
	 * Dynamic field - info from BFA
	 */
	enum bfa_pport_states 	port_state;	/*  current port state */
	enum bfa_pport_speed 	speed;		/*  current speed */
	enum bfa_pport_topology 	topology;	/*  current topology */
	bfa_boolean_t		beacon;		/*  current beacon status */
	bfa_boolean_t		link_e2e_beacon;/*  set if link beacon on */
	bfa_boolean_t		plog_enabled;	/*  set if portlog is enabled*/

	/*
	 * Dynamic field - info from FCS
	 */
	u32        	pid;		/*  port ID */
	enum bfa_pport_type 	port_type;	/*  current topology */
	u32        	loopback;	/*  external loopback */
	u32		rsvd1;
	u32		rsvd2;		/*  padding for 64 bit */
};

/**
 * 		FC Port statistics.
 */
struct bfa_pport_fc_stats_s {
	u64        secs_reset;	/*  seconds since stats is reset */
	u64        tx_frames;	/*  transmitted frames */
	u64        tx_words;	/*  transmitted words */
	u64        rx_frames;	/*  received frames */
	u64        rx_words;	/*  received words */
	u64        lip_count;	/*  LIPs seen */
	u64        nos_count;	/*  NOS count */
	u64        error_frames;	/*  errored frames (sent?) */
	u64        dropped_frames;	/*  dropped frames */
	u64        link_failures;	/*  link failure count */
	u64        loss_of_syncs;	/*  loss of sync count */
	u64        loss_of_signals;/*  loss of signal count */
	u64        primseq_errs;	/*  primitive sequence protocol */
	u64        bad_os_count;	/*  invalid ordered set */
	u64        err_enc_out;	/*  Encoding error outside frame */
	u64        invalid_crcs;	/*  frames received with invalid CRC*/
	u64	undersized_frm; /*  undersized frames */
	u64	oversized_frm;	/*  oversized frames */
	u64	bad_eof_frm;	/*  frames with bad EOF */
	struct bfa_qos_stats_s	qos_stats;	/*  QoS statistics */
};

/**
 * 		Eth Port statistics.
 */
struct bfa_pport_eth_stats_s {
	u64	secs_reset;	/*  seconds since stats is reset */
	u64	frame_64;      /*  both rx and tx counter */
	u64	frame_65_127;      /* both rx and tx counter */
	u64	frame_128_255;     /* both rx and tx counter */
	u64	frame_256_511;     /* both rx and tx counter */
	u64	frame_512_1023;    /* both rx and tx counter */
	u64	frame_1024_1518;   /* both rx and tx counter */
	u64	frame_1519_1522;   /* both rx and tx counter */

	u64	tx_bytes;
	u64	tx_packets;
	u64	tx_mcast_packets;
	u64	tx_bcast_packets;
	u64	tx_control_frame;
	u64	tx_drop;
	u64	tx_jabber;
	u64	tx_fcs_error;
	u64	tx_fragments;

	u64	rx_bytes;
	u64	rx_packets;
	u64	rx_mcast_packets;
	u64	rx_bcast_packets;
	u64	rx_control_frames;
	u64	rx_unknown_opcode;
	u64	rx_drop;
	u64	rx_jabber;
	u64	rx_fcs_error;
	u64	rx_alignment_error;
	u64	rx_frame_length_error;
	u64	rx_code_error;
	u64	rx_fragments;

	u64	rx_pause; /* BPC */
	u64	rx_zero_pause; /*  BPC Pause cancellation */
	u64	tx_pause;      /* BPC */
	u64	tx_zero_pause; /*  BPC Pause cancellation */
	u64	rx_fcoe_pause; /* BPC */
	u64	rx_fcoe_zero_pause; /*  BPC Pause cancellation */
	u64	tx_fcoe_pause;      /* BPC */
	u64	tx_fcoe_zero_pause; /*  BPC Pause cancellation */
};

/**
 * 		Port statistics.
 */
union bfa_pport_stats_u {
	struct bfa_pport_fc_stats_s	fc;
	struct bfa_pport_eth_stats_s 	eth;
};

/**
 *              Port FCP mappings.
 */
struct bfa_pport_fcpmap_s {
	char		osdevname[256];
	u32	bus;
	u32        target;
	u32        oslun;
	u32        fcid;
	wwn_t           nwwn;
	wwn_t           pwwn;
	u64        fcplun;
	char		luid[256];
};

/**
 *              Port RNID info.
 */
struct bfa_pport_rnid_s {
	wwn_t             wwn;
	u32          unittype;
	u32          portid;
	u32          attached_nodes_num;
	u16          ip_version;
	u16          udp_port;
	u8           ipaddr[16];
	u16          rsvd;
	u16          topologydiscoveryflags;
};

/**
 * 		Link state information
 */
struct bfa_pport_link_s {
	u8         linkstate;	/*  Link state bfa_pport_linkstate */
	u8         linkstate_rsn;	/*  bfa_pport_linkstate_rsn_t */
	u8         topology;	/*  P2P/LOOP bfa_pport_topology */
	u8         speed;		/*  Link speed (1/2/4/8 G) */
	u32        linkstate_opt;	/*  Linkstate optional data (debug) */
	u8         trunked;	/*  Trunked or not (1 or 0) */
	u8         resvd[3];
	struct bfa_qos_attr_s  qos_attr;   /* QoS Attributes */
	struct bfa_qos_vc_attr_s qos_vc_attr;  /*  VC info from ELP */
	union {
		struct {
			u8         tmaster;/*  Trunk Master or
						 *    not (1 or 0) */
			u8         tlinks;	/*  Trunk links bitmap
						 *    (linkup) */
			u8         resv1;	/*  Reserved */
		} trunk_info;

		struct {
			u8         myalpa;	   /*  alpa claimed */
			u8         login_req; /*  Login required or
						    *    not (1 or 0) */
			u8         alpabm_val;/*  alpa bitmap valid
						    *    or not (1 or 0) */
			struct fc_alpabm_s     alpabm;	   /*  alpa bitmap */
		} loop_info;
	} tl;
};

#endif /* __BFA_DEFS_PPORT_H__ */
