#define A_SG_CONTROL 0x0

#define S_CONGMODE    29
#define V_CONGMODE(x) ((x) << S_CONGMODE)
#define F_CONGMODE    V_CONGMODE(1U)

#define S_TNLFLMODE    28
#define V_TNLFLMODE(x) ((x) << S_TNLFLMODE)
#define F_TNLFLMODE    V_TNLFLMODE(1U)

#define S_FATLPERREN    27
#define V_FATLPERREN(x) ((x) << S_FATLPERREN)
#define F_FATLPERREN    V_FATLPERREN(1U)

#define S_DROPPKT    20
#define V_DROPPKT(x) ((x) << S_DROPPKT)
#define F_DROPPKT    V_DROPPKT(1U)

#define S_EGRGENCTRL    19
#define V_EGRGENCTRL(x) ((x) << S_EGRGENCTRL)
#define F_EGRGENCTRL    V_EGRGENCTRL(1U)

#define S_USERSPACESIZE    14
#define M_USERSPACESIZE    0x1f
#define V_USERSPACESIZE(x) ((x) << S_USERSPACESIZE)

#define S_HOSTPAGESIZE    11
#define M_HOSTPAGESIZE    0x7
#define V_HOSTPAGESIZE(x) ((x) << S_HOSTPAGESIZE)

#define S_FLMODE    9
#define V_FLMODE(x) ((x) << S_FLMODE)
#define F_FLMODE    V_FLMODE(1U)

#define S_PKTSHIFT    6
#define M_PKTSHIFT    0x7
#define V_PKTSHIFT(x) ((x) << S_PKTSHIFT)

#define S_ONEINTMULTQ    5
#define V_ONEINTMULTQ(x) ((x) << S_ONEINTMULTQ)
#define F_ONEINTMULTQ    V_ONEINTMULTQ(1U)

#define S_BIGENDIANINGRESS    2
#define V_BIGENDIANINGRESS(x) ((x) << S_BIGENDIANINGRESS)
#define F_BIGENDIANINGRESS    V_BIGENDIANINGRESS(1U)

#define S_ISCSICOALESCING    1
#define V_ISCSICOALESCING(x) ((x) << S_ISCSICOALESCING)
#define F_ISCSICOALESCING    V_ISCSICOALESCING(1U)

#define S_GLOBALENABLE    0
#define V_GLOBALENABLE(x) ((x) << S_GLOBALENABLE)
#define F_GLOBALENABLE    V_GLOBALENABLE(1U)

#define S_AVOIDCQOVFL    24
#define V_AVOIDCQOVFL(x) ((x) << S_AVOIDCQOVFL)
#define F_AVOIDCQOVFL    V_AVOIDCQOVFL(1U)

#define S_OPTONEINTMULTQ    23
#define V_OPTONEINTMULTQ(x) ((x) << S_OPTONEINTMULTQ)
#define F_OPTONEINTMULTQ    V_OPTONEINTMULTQ(1U)

#define S_CQCRDTCTRL    22
#define V_CQCRDTCTRL(x) ((x) << S_CQCRDTCTRL)
#define F_CQCRDTCTRL    V_CQCRDTCTRL(1U)

#define A_SG_KDOORBELL 0x4

#define S_SELEGRCNTX    31
#define V_SELEGRCNTX(x) ((x) << S_SELEGRCNTX)
#define F_SELEGRCNTX    V_SELEGRCNTX(1U)

#define S_EGRCNTX    0
#define M_EGRCNTX    0xffff
#define V_EGRCNTX(x) ((x) << S_EGRCNTX)

#define A_SG_GTS 0x8

#define S_RSPQ    29
#define M_RSPQ    0x7
#define V_RSPQ(x) ((x) << S_RSPQ)
#define G_RSPQ(x) (((x) >> S_RSPQ) & M_RSPQ)

#define S_NEWTIMER    16
#define M_NEWTIMER    0x1fff
#define V_NEWTIMER(x) ((x) << S_NEWTIMER)

#define S_NEWINDEX    0
#define M_NEWINDEX    0xffff
#define V_NEWINDEX(x) ((x) << S_NEWINDEX)

#define A_SG_CONTEXT_CMD 0xc

#define S_CONTEXT_CMD_OPCODE    28
#define M_CONTEXT_CMD_OPCODE    0xf
#define V_CONTEXT_CMD_OPCODE(x) ((x) << S_CONTEXT_CMD_OPCODE)

#define S_CONTEXT_CMD_BUSY    27
#define V_CONTEXT_CMD_BUSY(x) ((x) << S_CONTEXT_CMD_BUSY)
#define F_CONTEXT_CMD_BUSY    V_CONTEXT_CMD_BUSY(1U)

#define S_CQ_CREDIT    20

#define M_CQ_CREDIT    0x7f

#define V_CQ_CREDIT(x) ((x) << S_CQ_CREDIT)

#define G_CQ_CREDIT(x) (((x) >> S_CQ_CREDIT) & M_CQ_CREDIT)

#define S_CQ    19

#define V_CQ(x) ((x) << S_CQ)
#define F_CQ    V_CQ(1U)

#define S_RESPONSEQ    18
#define V_RESPONSEQ(x) ((x) << S_RESPONSEQ)
#define F_RESPONSEQ    V_RESPONSEQ(1U)

#define S_EGRESS    17
#define V_EGRESS(x) ((x) << S_EGRESS)
#define F_EGRESS    V_EGRESS(1U)

#define S_FREELIST    16
#define V_FREELIST(x) ((x) << S_FREELIST)
#define F_FREELIST    V_FREELIST(1U)

#define S_CONTEXT    0
#define M_CONTEXT    0xffff
#define V_CONTEXT(x) ((x) << S_CONTEXT)

#define G_CONTEXT(x) (((x) >> S_CONTEXT) & M_CONTEXT)

#define A_SG_CONTEXT_DATA0 0x10

#define A_SG_CONTEXT_DATA1 0x14

#define A_SG_CONTEXT_DATA2 0x18

#define A_SG_CONTEXT_DATA3 0x1c

#define A_SG_CONTEXT_MASK0 0x20

#define A_SG_CONTEXT_MASK1 0x24

#define A_SG_CONTEXT_MASK2 0x28

#define A_SG_CONTEXT_MASK3 0x2c

#define A_SG_RSPQ_CREDIT_RETURN 0x30

#define S_CREDITS    0
#define M_CREDITS    0xffff
#define V_CREDITS(x) ((x) << S_CREDITS)

#define A_SG_DATA_INTR 0x34

#define S_ERRINTR    31
#define V_ERRINTR(x) ((x) << S_ERRINTR)
#define F_ERRINTR    V_ERRINTR(1U)

#define A_SG_HI_DRB_HI_THRSH 0x38

#define A_SG_HI_DRB_LO_THRSH 0x3c

#define A_SG_LO_DRB_HI_THRSH 0x40

#define A_SG_LO_DRB_LO_THRSH 0x44

#define A_SG_RSPQ_FL_STATUS 0x4c

#define S_RSPQ0DISABLED    8

#define S_FL0EMPTY    16
#define V_FL0EMPTY(x) ((x) << S_FL0EMPTY)
#define F_FL0EMPTY    V_FL0EMPTY(1U)

#define A_SG_EGR_RCQ_DRB_THRSH 0x54

#define S_HIRCQDRBTHRSH    16
#define M_HIRCQDRBTHRSH    0x7ff
#define V_HIRCQDRBTHRSH(x) ((x) << S_HIRCQDRBTHRSH)

#define S_LORCQDRBTHRSH    0
#define M_LORCQDRBTHRSH    0x7ff
#define V_LORCQDRBTHRSH(x) ((x) << S_LORCQDRBTHRSH)

#define A_SG_EGR_CNTX_BADDR 0x58

#define A_SG_INT_CAUSE 0x5c

#define S_HIRCQPARITYERROR    31
#define V_HIRCQPARITYERROR(x) ((x) << S_HIRCQPARITYERROR)
#define F_HIRCQPARITYERROR    V_HIRCQPARITYERROR(1U)

#define S_LORCQPARITYERROR    30
#define V_LORCQPARITYERROR(x) ((x) << S_LORCQPARITYERROR)
#define F_LORCQPARITYERROR    V_LORCQPARITYERROR(1U)

#define S_HIDRBPARITYERROR    29
#define V_HIDRBPARITYERROR(x) ((x) << S_HIDRBPARITYERROR)
#define F_HIDRBPARITYERROR    V_HIDRBPARITYERROR(1U)

#define S_LODRBPARITYERROR    28
#define V_LODRBPARITYERROR(x) ((x) << S_LODRBPARITYERROR)
#define F_LODRBPARITYERROR    V_LODRBPARITYERROR(1U)

#define S_FLPARITYERROR    22
#define M_FLPARITYERROR    0x3f
#define V_FLPARITYERROR(x) ((x) << S_FLPARITYERROR)
#define G_FLPARITYERROR(x) (((x) >> S_FLPARITYERROR) & M_FLPARITYERROR)

#define S_ITPARITYERROR    20
#define M_ITPARITYERROR    0x3
#define V_ITPARITYERROR(x) ((x) << S_ITPARITYERROR)
#define G_ITPARITYERROR(x) (((x) >> S_ITPARITYERROR) & M_ITPARITYERROR)

#define S_IRPARITYERROR    19
#define V_IRPARITYERROR(x) ((x) << S_IRPARITYERROR)
#define F_IRPARITYERROR    V_IRPARITYERROR(1U)

#define S_RCPARITYERROR    18
#define V_RCPARITYERROR(x) ((x) << S_RCPARITYERROR)
#define F_RCPARITYERROR    V_RCPARITYERROR(1U)

#define S_OCPARITYERROR    17
#define V_OCPARITYERROR(x) ((x) << S_OCPARITYERROR)
#define F_OCPARITYERROR    V_OCPARITYERROR(1U)

#define S_CPPARITYERROR    16
#define V_CPPARITYERROR(x) ((x) << S_CPPARITYERROR)
#define F_CPPARITYERROR    V_CPPARITYERROR(1U)

#define S_R_REQ_FRAMINGERROR    15
#define V_R_REQ_FRAMINGERROR(x) ((x) << S_R_REQ_FRAMINGERROR)
#define F_R_REQ_FRAMINGERROR    V_R_REQ_FRAMINGERROR(1U)

#define S_UC_REQ_FRAMINGERROR    14
#define V_UC_REQ_FRAMINGERROR(x) ((x) << S_UC_REQ_FRAMINGERROR)
#define F_UC_REQ_FRAMINGERROR    V_UC_REQ_FRAMINGERROR(1U)

#define S_HICTLDRBDROPERR    13
#define V_HICTLDRBDROPERR(x) ((x) << S_HICTLDRBDROPERR)
#define F_HICTLDRBDROPERR    V_HICTLDRBDROPERR(1U)

#define S_LOCTLDRBDROPERR    12
#define V_LOCTLDRBDROPERR(x) ((x) << S_LOCTLDRBDROPERR)
#define F_LOCTLDRBDROPERR    V_LOCTLDRBDROPERR(1U)

#define S_HIPIODRBDROPERR    11
#define V_HIPIODRBDROPERR(x) ((x) << S_HIPIODRBDROPERR)
#define F_HIPIODRBDROPERR    V_HIPIODRBDROPERR(1U)

#define S_LOPIODRBDROPERR    10
#define V_LOPIODRBDROPERR(x) ((x) << S_LOPIODRBDROPERR)
#define F_LOPIODRBDROPERR    V_LOPIODRBDROPERR(1U)

#define S_RSPQDISABLED    3
#define V_RSPQDISABLED(x) ((x) << S_RSPQDISABLED)
#define F_RSPQDISABLED    V_RSPQDISABLED(1U)

#define S_RSPQCREDITOVERFOW    2
#define V_RSPQCREDITOVERFOW(x) ((x) << S_RSPQCREDITOVERFOW)
#define F_RSPQCREDITOVERFOW    V_RSPQCREDITOVERFOW(1U)

#define S_FLEMPTY    1
#define V_FLEMPTY(x) ((x) << S_FLEMPTY)
#define F_FLEMPTY    V_FLEMPTY(1U)

#define A_SG_INT_ENABLE 0x60

#define A_SG_CMDQ_CREDIT_TH 0x64

#define S_TIMEOUT    8
#define M_TIMEOUT    0xffffff
#define V_TIMEOUT(x) ((x) << S_TIMEOUT)

#define S_THRESHOLD    0
#define M_THRESHOLD    0xff
#define V_THRESHOLD(x) ((x) << S_THRESHOLD)

#define A_SG_TIMER_TICK 0x68

#define A_SG_CQ_CONTEXT_BADDR 0x6c

#define A_SG_OCO_BASE 0x70

#define S_BASE1    16
#define M_BASE1    0xffff
#define V_BASE1(x) ((x) << S_BASE1)

#define A_SG_DRB_PRI_THRESH 0x74

#define A_PCIX_INT_ENABLE 0x80

#define S_MSIXPARERR    22
#define M_MSIXPARERR    0x7

#define V_MSIXPARERR(x) ((x) << S_MSIXPARERR)

#define S_CFPARERR    18
#define M_CFPARERR    0xf

#define V_CFPARERR(x) ((x) << S_CFPARERR)

#define S_RFPARERR    14
#define M_RFPARERR    0xf

#define V_RFPARERR(x) ((x) << S_RFPARERR)

#define S_WFPARERR    12
#define M_WFPARERR    0x3

#define V_WFPARERR(x) ((x) << S_WFPARERR)

#define S_PIOPARERR    11
#define V_PIOPARERR(x) ((x) << S_PIOPARERR)
#define F_PIOPARERR    V_PIOPARERR(1U)

#define S_DETUNCECCERR    10
#define V_DETUNCECCERR(x) ((x) << S_DETUNCECCERR)
#define F_DETUNCECCERR    V_DETUNCECCERR(1U)

#define S_DETCORECCERR    9
#define V_DETCORECCERR(x) ((x) << S_DETCORECCERR)
#define F_DETCORECCERR    V_DETCORECCERR(1U)

#define S_RCVSPLCMPERR    8
#define V_RCVSPLCMPERR(x) ((x) << S_RCVSPLCMPERR)
#define F_RCVSPLCMPERR    V_RCVSPLCMPERR(1U)

#define S_UNXSPLCMP    7
#define V_UNXSPLCMP(x) ((x) << S_UNXSPLCMP)
#define F_UNXSPLCMP    V_UNXSPLCMP(1U)

#define S_SPLCMPDIS    6
#define V_SPLCMPDIS(x) ((x) << S_SPLCMPDIS)
#define F_SPLCMPDIS    V_SPLCMPDIS(1U)

#define S_DETPARERR    5
#define V_DETPARERR(x) ((x) << S_DETPARERR)
#define F_DETPARERR    V_DETPARERR(1U)

#define S_SIGSYSERR    4
#define V_SIGSYSERR(x) ((x) << S_SIGSYSERR)
#define F_SIGSYSERR    V_SIGSYSERR(1U)

#define S_RCVMSTABT    3
#define V_RCVMSTABT(x) ((x) << S_RCVMSTABT)
#define F_RCVMSTABT    V_RCVMSTABT(1U)

#define S_RCVTARABT    2
#define V_RCVTARABT(x) ((x) << S_RCVTARABT)
#define F_RCVTARABT    V_RCVTARABT(1U)

#define S_SIGTARABT    1
#define V_SIGTARABT(x) ((x) << S_SIGTARABT)
#define F_SIGTARABT    V_SIGTARABT(1U)

#define S_MSTDETPARERR    0
#define V_MSTDETPARERR(x) ((x) << S_MSTDETPARERR)
#define F_MSTDETPARERR    V_MSTDETPARERR(1U)

#define A_PCIX_INT_CAUSE 0x84

#define A_PCIX_CFG 0x88

#define S_DMASTOPEN    19
#define V_DMASTOPEN(x) ((x) << S_DMASTOPEN)
#define F_DMASTOPEN    V_DMASTOPEN(1U)

#define S_CLIDECEN    18
#define V_CLIDECEN(x) ((x) << S_CLIDECEN)
#define F_CLIDECEN    V_CLIDECEN(1U)

#define A_PCIX_MODE 0x8c

#define S_PCLKRANGE    6
#define M_PCLKRANGE    0x3
#define V_PCLKRANGE(x) ((x) << S_PCLKRANGE)
#define G_PCLKRANGE(x) (((x) >> S_PCLKRANGE) & M_PCLKRANGE)

#define S_PCIXINITPAT    2
#define M_PCIXINITPAT    0xf
#define V_PCIXINITPAT(x) ((x) << S_PCIXINITPAT)
#define G_PCIXINITPAT(x) (((x) >> S_PCIXINITPAT) & M_PCIXINITPAT)

#define S_64BIT    0
#define V_64BIT(x) ((x) << S_64BIT)
#define F_64BIT    V_64BIT(1U)

#define A_PCIE_INT_ENABLE 0x80

#define S_BISTERR    15
#define M_BISTERR    0xff

#define V_BISTERR(x) ((x) << S_BISTERR)

#define S_TXPARERR    18
#define V_TXPARERR(x) ((x) << S_TXPARERR)
#define F_TXPARERR    V_TXPARERR(1U)

#define S_RXPARERR    17
#define V_RXPARERR(x) ((x) << S_RXPARERR)
#define F_RXPARERR    V_RXPARERR(1U)

#define S_RETRYLUTPARERR    16
#define V_RETRYLUTPARERR(x) ((x) << S_RETRYLUTPARERR)
#define F_RETRYLUTPARERR    V_RETRYLUTPARERR(1U)

#define S_RETRYBUFPARERR    15
#define V_RETRYBUFPARERR(x) ((x) << S_RETRYBUFPARERR)
#define F_RETRYBUFPARERR    V_RETRYBUFPARERR(1U)

#define S_PCIE_MSIXPARERR    12
#define M_PCIE_MSIXPARERR    0x7

#define V_PCIE_MSIXPARERR(x) ((x) << S_PCIE_MSIXPARERR)

#define S_PCIE_CFPARERR    11
#define V_PCIE_CFPARERR(x) ((x) << S_PCIE_CFPARERR)
#define F_PCIE_CFPARERR    V_PCIE_CFPARERR(1U)

#define S_PCIE_RFPARERR    10
#define V_PCIE_RFPARERR(x) ((x) << S_PCIE_RFPARERR)
#define F_PCIE_RFPARERR    V_PCIE_RFPARERR(1U)

#define S_PCIE_WFPARERR    9
#define V_PCIE_WFPARERR(x) ((x) << S_PCIE_WFPARERR)
#define F_PCIE_WFPARERR    V_PCIE_WFPARERR(1U)

#define S_PCIE_PIOPARERR    8
#define V_PCIE_PIOPARERR(x) ((x) << S_PCIE_PIOPARERR)
#define F_PCIE_PIOPARERR    V_PCIE_PIOPARERR(1U)

#define S_UNXSPLCPLERRC    7
#define V_UNXSPLCPLERRC(x) ((x) << S_UNXSPLCPLERRC)
#define F_UNXSPLCPLERRC    V_UNXSPLCPLERRC(1U)

#define S_UNXSPLCPLERRR    6
#define V_UNXSPLCPLERRR(x) ((x) << S_UNXSPLCPLERRR)
#define F_UNXSPLCPLERRR    V_UNXSPLCPLERRR(1U)

#define S_PEXERR    0
#define V_PEXERR(x) ((x) << S_PEXERR)
#define F_PEXERR    V_PEXERR(1U)

#define A_PCIE_INT_CAUSE 0x84

#define S_PCIE_DMASTOPEN    24
#define V_PCIE_DMASTOPEN(x) ((x) << S_PCIE_DMASTOPEN)
#define F_PCIE_DMASTOPEN    V_PCIE_DMASTOPEN(1U)

#define A_PCIE_CFG 0x88

#define S_ENABLELINKDWNDRST    21
#define V_ENABLELINKDWNDRST(x) ((x) << S_ENABLELINKDWNDRST)
#define F_ENABLELINKDWNDRST    V_ENABLELINKDWNDRST(1U)

#define S_ENABLELINKDOWNRST    20
#define V_ENABLELINKDOWNRST(x) ((x) << S_ENABLELINKDOWNRST)
#define F_ENABLELINKDOWNRST    V_ENABLELINKDOWNRST(1U)

#define S_PCIE_CLIDECEN    16
#define V_PCIE_CLIDECEN(x) ((x) << S_PCIE_CLIDECEN)
#define F_PCIE_CLIDECEN    V_PCIE_CLIDECEN(1U)

#define S_CRSTWRMMODE    0
#define V_CRSTWRMMODE(x) ((x) << S_CRSTWRMMODE)
#define F_CRSTWRMMODE    V_CRSTWRMMODE(1U)

#define A_PCIE_MODE 0x8c

#define S_NUMFSTTRNSEQRX    10
#define M_NUMFSTTRNSEQRX    0xff
#define V_NUMFSTTRNSEQRX(x) ((x) << S_NUMFSTTRNSEQRX)
#define G_NUMFSTTRNSEQRX(x) (((x) >> S_NUMFSTTRNSEQRX) & M_NUMFSTTRNSEQRX)

#define A_PCIE_PEX_CTRL0 0x98

#define S_NUMFSTTRNSEQ    22
#define M_NUMFSTTRNSEQ    0xff
#define V_NUMFSTTRNSEQ(x) ((x) << S_NUMFSTTRNSEQ)
#define G_NUMFSTTRNSEQ(x) (((x) >> S_NUMFSTTRNSEQ) & M_NUMFSTTRNSEQ)

#define S_REPLAYLMT    2
#define M_REPLAYLMT    0xfffff

#define V_REPLAYLMT(x) ((x) << S_REPLAYLMT)

#define A_PCIE_PEX_CTRL1 0x9c

#define S_T3A_ACKLAT    0
#define M_T3A_ACKLAT    0x7ff

#define V_T3A_ACKLAT(x) ((x) << S_T3A_ACKLAT)

#define S_ACKLAT    0
#define M_ACKLAT    0x1fff

#define V_ACKLAT(x) ((x) << S_ACKLAT)

#define A_PCIE_PEX_ERR 0xa4

#define A_T3DBG_GPIO_EN 0xd0

#define S_GPIO11_OEN    27
#define V_GPIO11_OEN(x) ((x) << S_GPIO11_OEN)
#define F_GPIO11_OEN    V_GPIO11_OEN(1U)

#define S_GPIO10_OEN    26
#define V_GPIO10_OEN(x) ((x) << S_GPIO10_OEN)
#define F_GPIO10_OEN    V_GPIO10_OEN(1U)

#define S_GPIO7_OEN    23
#define V_GPIO7_OEN(x) ((x) << S_GPIO7_OEN)
#define F_GPIO7_OEN    V_GPIO7_OEN(1U)

#define S_GPIO6_OEN    22
#define V_GPIO6_OEN(x) ((x) << S_GPIO6_OEN)
#define F_GPIO6_OEN    V_GPIO6_OEN(1U)

#define S_GPIO5_OEN    21
#define V_GPIO5_OEN(x) ((x) << S_GPIO5_OEN)
#define F_GPIO5_OEN    V_GPIO5_OEN(1U)

#define S_GPIO4_OEN    20
#define V_GPIO4_OEN(x) ((x) << S_GPIO4_OEN)
#define F_GPIO4_OEN    V_GPIO4_OEN(1U)

#define S_GPIO2_OEN    18
#define V_GPIO2_OEN(x) ((x) << S_GPIO2_OEN)
#define F_GPIO2_OEN    V_GPIO2_OEN(1U)

#define S_GPIO1_OEN    17
#define V_GPIO1_OEN(x) ((x) << S_GPIO1_OEN)
#define F_GPIO1_OEN    V_GPIO1_OEN(1U)

#define S_GPIO0_OEN    16
#define V_GPIO0_OEN(x) ((x) << S_GPIO0_OEN)
#define F_GPIO0_OEN    V_GPIO0_OEN(1U)

#define S_GPIO10_OUT_VAL    10
#define V_GPIO10_OUT_VAL(x) ((x) << S_GPIO10_OUT_VAL)
#define F_GPIO10_OUT_VAL    V_GPIO10_OUT_VAL(1U)

#define S_GPIO7_OUT_VAL    7
#define V_GPIO7_OUT_VAL(x) ((x) << S_GPIO7_OUT_VAL)
#define F_GPIO7_OUT_VAL    V_GPIO7_OUT_VAL(1U)

#define S_GPIO6_OUT_VAL    6
#define V_GPIO6_OUT_VAL(x) ((x) << S_GPIO6_OUT_VAL)
#define F_GPIO6_OUT_VAL    V_GPIO6_OUT_VAL(1U)

#define S_GPIO5_OUT_VAL    5
#define V_GPIO5_OUT_VAL(x) ((x) << S_GPIO5_OUT_VAL)
#define F_GPIO5_OUT_VAL    V_GPIO5_OUT_VAL(1U)

#define S_GPIO4_OUT_VAL    4
#define V_GPIO4_OUT_VAL(x) ((x) << S_GPIO4_OUT_VAL)
#define F_GPIO4_OUT_VAL    V_GPIO4_OUT_VAL(1U)

#define S_GPIO2_OUT_VAL    2
#define V_GPIO2_OUT_VAL(x) ((x) << S_GPIO2_OUT_VAL)
#define F_GPIO2_OUT_VAL    V_GPIO2_OUT_VAL(1U)

#define S_GPIO1_OUT_VAL    1
#define V_GPIO1_OUT_VAL(x) ((x) << S_GPIO1_OUT_VAL)
#define F_GPIO1_OUT_VAL    V_GPIO1_OUT_VAL(1U)

#define S_GPIO0_OUT_VAL    0
#define V_GPIO0_OUT_VAL(x) ((x) << S_GPIO0_OUT_VAL)
#define F_GPIO0_OUT_VAL    V_GPIO0_OUT_VAL(1U)

#define A_T3DBG_INT_ENABLE 0xd8

#define S_GPIO11    11
#define V_GPIO11(x) ((x) << S_GPIO11)
#define F_GPIO11    V_GPIO11(1U)

#define S_GPIO10    10
#define V_GPIO10(x) ((x) << S_GPIO10)
#define F_GPIO10    V_GPIO10(1U)

#define S_GPIO9    9
#define V_GPIO9(x) ((x) << S_GPIO9)
#define F_GPIO9    V_GPIO9(1U)

#define S_GPIO7    7
#define V_GPIO7(x) ((x) << S_GPIO7)
#define F_GPIO7    V_GPIO7(1U)

#define S_GPIO6    6
#define V_GPIO6(x) ((x) << S_GPIO6)
#define F_GPIO6    V_GPIO6(1U)

#define S_GPIO5    5
#define V_GPIO5(x) ((x) << S_GPIO5)
#define F_GPIO5    V_GPIO5(1U)

#define S_GPIO4    4
#define V_GPIO4(x) ((x) << S_GPIO4)
#define F_GPIO4    V_GPIO4(1U)

#define S_GPIO3    3
#define V_GPIO3(x) ((x) << S_GPIO3)
#define F_GPIO3    V_GPIO3(1U)

#define S_GPIO2    2
#define V_GPIO2(x) ((x) << S_GPIO2)
#define F_GPIO2    V_GPIO2(1U)

#define S_GPIO1    1
#define V_GPIO1(x) ((x) << S_GPIO1)
#define F_GPIO1    V_GPIO1(1U)

#define S_GPIO0    0
#define V_GPIO0(x) ((x) << S_GPIO0)
#define F_GPIO0    V_GPIO0(1U)

#define A_T3DBG_INT_CAUSE 0xdc

#define A_T3DBG_GPIO_ACT_LOW 0xf0

#define MC7_PMRX_BASE_ADDR 0x100

#define A_MC7_CFG 0x100

#define S_IFEN    13
#define V_IFEN(x) ((x) << S_IFEN)
#define F_IFEN    V_IFEN(1U)

#define S_TERM150    11
#define V_TERM150(x) ((x) << S_TERM150)
#define F_TERM150    V_TERM150(1U)

#define S_SLOW    10
#define V_SLOW(x) ((x) << S_SLOW)
#define F_SLOW    V_SLOW(1U)

#define S_WIDTH    8
#define M_WIDTH    0x3
#define V_WIDTH(x) ((x) << S_WIDTH)
#define G_WIDTH(x) (((x) >> S_WIDTH) & M_WIDTH)

#define S_BKS    6
#define V_BKS(x) ((x) << S_BKS)
#define F_BKS    V_BKS(1U)

#define S_ORG    5
#define V_ORG(x) ((x) << S_ORG)
#define F_ORG    V_ORG(1U)

#define S_DEN    2
#define M_DEN    0x7
#define V_DEN(x) ((x) << S_DEN)
#define G_DEN(x) (((x) >> S_DEN) & M_DEN)

#define S_RDY    1
#define V_RDY(x) ((x) << S_RDY)
#define F_RDY    V_RDY(1U)

#define S_CLKEN    0
#define V_CLKEN(x) ((x) << S_CLKEN)
#define F_CLKEN    V_CLKEN(1U)

#define A_MC7_MODE 0x104

#define S_BUSY    31
#define V_BUSY(x) ((x) << S_BUSY)
#define F_BUSY    V_BUSY(1U)

#define S_BUSY    31
#define V_BUSY(x) ((x) << S_BUSY)
#define F_BUSY    V_BUSY(1U)

#define A_MC7_EXT_MODE1 0x108

#define A_MC7_EXT_MODE2 0x10c

#define A_MC7_EXT_MODE3 0x110

#define A_MC7_PRE 0x114

#define A_MC7_REF 0x118

#define S_PREREFDIV    1
#define M_PREREFDIV    0x3fff
#define V_PREREFDIV(x) ((x) << S_PREREFDIV)

#define S_PERREFEN    0
#define V_PERREFEN(x) ((x) << S_PERREFEN)
#define F_PERREFEN    V_PERREFEN(1U)

#define A_MC7_DLL 0x11c

#define S_DLLENB    1
#define V_DLLENB(x) ((x) << S_DLLENB)
#define F_DLLENB    V_DLLENB(1U)

#define S_DLLRST    0
#define V_DLLRST(x) ((x) << S_DLLRST)
#define F_DLLRST    V_DLLRST(1U)

#define A_MC7_PARM 0x120

#define S_ACTTOPREDLY    26
#define M_ACTTOPREDLY    0xf
#define V_ACTTOPREDLY(x) ((x) << S_ACTTOPREDLY)

#define S_ACTTORDWRDLY    23
#define M_ACTTORDWRDLY    0x7
#define V_ACTTORDWRDLY(x) ((x) << S_ACTTORDWRDLY)

#define S_PRECYC    20
#define M_PRECYC    0x7
#define V_PRECYC(x) ((x) << S_PRECYC)

#define S_REFCYC    13
#define M_REFCYC    0x7f
#define V_REFCYC(x) ((x) << S_REFCYC)

#define S_BKCYC    8
#define M_BKCYC    0x1f
#define V_BKCYC(x) ((x) << S_BKCYC)

#define S_WRTORDDLY    4
#define M_WRTORDDLY    0xf
#define V_WRTORDDLY(x) ((x) << S_WRTORDDLY)

#define S_RDTOWRDLY    0
#define M_RDTOWRDLY    0xf
#define V_RDTOWRDLY(x) ((x) << S_RDTOWRDLY)

#define A_MC7_CAL 0x128

#define S_BUSY    31
#define V_BUSY(x) ((x) << S_BUSY)
#define F_BUSY    V_BUSY(1U)

#define S_BUSY    31
#define V_BUSY(x) ((x) << S_BUSY)
#define F_BUSY    V_BUSY(1U)

#define S_CAL_FAULT    30
#define V_CAL_FAULT(x) ((x) << S_CAL_FAULT)
#define F_CAL_FAULT    V_CAL_FAULT(1U)

#define S_SGL_CAL_EN    20
#define V_SGL_CAL_EN(x) ((x) << S_SGL_CAL_EN)
#define F_SGL_CAL_EN    V_SGL_CAL_EN(1U)

#define A_MC7_ERR_ADDR 0x12c

#define A_MC7_ECC 0x130

#define S_ECCCHKEN    1
#define V_ECCCHKEN(x) ((x) << S_ECCCHKEN)
#define F_ECCCHKEN    V_ECCCHKEN(1U)

#define S_ECCGENEN    0
#define V_ECCGENEN(x) ((x) << S_ECCGENEN)
#define F_ECCGENEN    V_ECCGENEN(1U)

#define A_MC7_CE_ADDR 0x134

#define A_MC7_CE_DATA0 0x138

#define A_MC7_CE_DATA1 0x13c

#define A_MC7_CE_DATA2 0x140

#define S_DATA    0
#define M_DATA    0xff

#define G_DATA(x) (((x) >> S_DATA) & M_DATA)

#define A_MC7_UE_ADDR 0x144

#define A_MC7_UE_DATA0 0x148

#define A_MC7_UE_DATA1 0x14c

#define A_MC7_UE_DATA2 0x150

#define A_MC7_BD_ADDR 0x154

#define S_ADDR    3

#define M_ADDR    0x1fffffff

#define A_MC7_BD_DATA0 0x158

#define A_MC7_BD_DATA1 0x15c

#define A_MC7_BD_OP 0x164

#define S_OP    0

#define V_OP(x) ((x) << S_OP)
#define F_OP    V_OP(1U)

#define F_OP    V_OP(1U)
#define A_SF_OP 0x6dc

#define A_MC7_BIST_ADDR_BEG 0x168

#define A_MC7_BIST_ADDR_END 0x16c

#define A_MC7_BIST_DATA 0x170

#define A_MC7_BIST_OP 0x174

#define S_CONT    3
#define V_CONT(x) ((x) << S_CONT)
#define F_CONT    V_CONT(1U)

#define F_CONT    V_CONT(1U)

#define A_MC7_INT_ENABLE 0x178

#define S_AE    17
#define V_AE(x) ((x) << S_AE)
#define F_AE    V_AE(1U)

#define S_PE    2
#define M_PE    0x7fff

#define V_PE(x) ((x) << S_PE)

#define G_PE(x) (((x) >> S_PE) & M_PE)

#define S_UE    1
#define V_UE(x) ((x) << S_UE)
#define F_UE    V_UE(1U)

#define S_CE    0
#define V_CE(x) ((x) << S_CE)
#define F_CE    V_CE(1U)

#define A_MC7_INT_CAUSE 0x17c

#define MC7_PMTX_BASE_ADDR 0x180

#define MC7_CM_BASE_ADDR 0x200

#define A_CIM_BOOT_CFG 0x280

#define S_BOOTADDR    2
#define M_BOOTADDR    0x3fffffff
#define V_BOOTADDR(x) ((x) << S_BOOTADDR)

#define A_CIM_SDRAM_BASE_ADDR 0x28c

#define A_CIM_SDRAM_ADDR_SIZE 0x290

#define A_CIM_HOST_INT_ENABLE 0x298

#define S_DTAGPARERR    28
#define V_DTAGPARERR(x) ((x) << S_DTAGPARERR)
#define F_DTAGPARERR    V_DTAGPARERR(1U)

#define S_ITAGPARERR    27
#define V_ITAGPARERR(x) ((x) << S_ITAGPARERR)
#define F_ITAGPARERR    V_ITAGPARERR(1U)

#define S_IBQTPPARERR    26
#define V_IBQTPPARERR(x) ((x) << S_IBQTPPARERR)
#define F_IBQTPPARERR    V_IBQTPPARERR(1U)

#define S_IBQULPPARERR    25
#define V_IBQULPPARERR(x) ((x) << S_IBQULPPARERR)
#define F_IBQULPPARERR    V_IBQULPPARERR(1U)

#define S_IBQSGEHIPARERR    24
#define V_IBQSGEHIPARERR(x) ((x) << S_IBQSGEHIPARERR)
#define F_IBQSGEHIPARERR    V_IBQSGEHIPARERR(1U)

#define S_IBQSGELOPARERR    23
#define V_IBQSGELOPARERR(x) ((x) << S_IBQSGELOPARERR)
#define F_IBQSGELOPARERR    V_IBQSGELOPARERR(1U)

#define S_OBQULPLOPARERR    22
#define V_OBQULPLOPARERR(x) ((x) << S_OBQULPLOPARERR)
#define F_OBQULPLOPARERR    V_OBQULPLOPARERR(1U)

#define S_OBQULPHIPARERR    21
#define V_OBQULPHIPARERR(x) ((x) << S_OBQULPHIPARERR)
#define F_OBQULPHIPARERR    V_OBQULPHIPARERR(1U)

#define S_OBQSGEPARERR    20
#define V_OBQSGEPARERR(x) ((x) << S_OBQSGEPARERR)
#define F_OBQSGEPARERR    V_OBQSGEPARERR(1U)

#define S_DCACHEPARERR    19
#define V_DCACHEPARERR(x) ((x) << S_DCACHEPARERR)
#define F_DCACHEPARERR    V_DCACHEPARERR(1U)

#define S_ICACHEPARERR    18
#define V_ICACHEPARERR(x) ((x) << S_ICACHEPARERR)
#define F_ICACHEPARERR    V_ICACHEPARERR(1U)

#define S_DRAMPARERR    17
#define V_DRAMPARERR(x) ((x) << S_DRAMPARERR)
#define F_DRAMPARERR    V_DRAMPARERR(1U)

#define A_CIM_HOST_INT_CAUSE 0x29c

#define S_BLKWRPLINT    12
#define V_BLKWRPLINT(x) ((x) << S_BLKWRPLINT)
#define F_BLKWRPLINT    V_BLKWRPLINT(1U)

#define S_BLKRDPLINT    11
#define V_BLKRDPLINT(x) ((x) << S_BLKRDPLINT)
#define F_BLKRDPLINT    V_BLKRDPLINT(1U)

#define S_BLKWRCTLINT    10
#define V_BLKWRCTLINT(x) ((x) << S_BLKWRCTLINT)
#define F_BLKWRCTLINT    V_BLKWRCTLINT(1U)

#define S_BLKRDCTLINT    9
#define V_BLKRDCTLINT(x) ((x) << S_BLKRDCTLINT)
#define F_BLKRDCTLINT    V_BLKRDCTLINT(1U)

#define S_BLKWRFLASHINT    8
#define V_BLKWRFLASHINT(x) ((x) << S_BLKWRFLASHINT)
#define F_BLKWRFLASHINT    V_BLKWRFLASHINT(1U)

#define S_BLKRDFLASHINT    7
#define V_BLKRDFLASHINT(x) ((x) << S_BLKRDFLASHINT)
#define F_BLKRDFLASHINT    V_BLKRDFLASHINT(1U)

#define S_SGLWRFLASHINT    6
#define V_SGLWRFLASHINT(x) ((x) << S_SGLWRFLASHINT)
#define F_SGLWRFLASHINT    V_SGLWRFLASHINT(1U)

#define S_WRBLKFLASHINT    5
#define V_WRBLKFLASHINT(x) ((x) << S_WRBLKFLASHINT)
#define F_WRBLKFLASHINT    V_WRBLKFLASHINT(1U)

#define S_BLKWRBOOTINT    4
#define V_BLKWRBOOTINT(x) ((x) << S_BLKWRBOOTINT)
#define F_BLKWRBOOTINT    V_BLKWRBOOTINT(1U)

#define S_FLASHRANGEINT    2
#define V_FLASHRANGEINT(x) ((x) << S_FLASHRANGEINT)
#define F_FLASHRANGEINT    V_FLASHRANGEINT(1U)

#define S_SDRAMRANGEINT    1
#define V_SDRAMRANGEINT(x) ((x) << S_SDRAMRANGEINT)
#define F_SDRAMRANGEINT    V_SDRAMRANGEINT(1U)

#define S_RSVDSPACEINT    0
#define V_RSVDSPACEINT(x) ((x) << S_RSVDSPACEINT)
#define F_RSVDSPACEINT    V_RSVDSPACEINT(1U)

#define A_CIM_HOST_ACC_CTRL 0x2b0

#define S_HOSTBUSY    17
#define V_HOSTBUSY(x) ((x) << S_HOSTBUSY)
#define F_HOSTBUSY    V_HOSTBUSY(1U)

#define A_CIM_HOST_ACC_DATA 0x2b4

#define A_CIM_IBQ_DBG_CFG 0x2c0

#define S_IBQDBGADDR    16
#define M_IBQDBGADDR    0x1ff
#define V_IBQDBGADDR(x) ((x) << S_IBQDBGADDR)
#define G_IBQDBGADDR(x) (((x) >> S_IBQDBGADDR) & M_IBQDBGADDR)

#define S_IBQDBGQID    3
#define M_IBQDBGQID    0x3
#define V_IBQDBGQID(x) ((x) << S_IBQDBGQID)
#define G_IBQDBGQID(x) (((x) >> S_IBQDBGQID) & M_IBQDBGQID)

#define S_IBQDBGWR    2
#define V_IBQDBGWR(x) ((x) << S_IBQDBGWR)
#define F_IBQDBGWR    V_IBQDBGWR(1U)

#define S_IBQDBGBUSY    1
#define V_IBQDBGBUSY(x) ((x) << S_IBQDBGBUSY)
#define F_IBQDBGBUSY    V_IBQDBGBUSY(1U)

#define S_IBQDBGEN    0
#define V_IBQDBGEN(x) ((x) << S_IBQDBGEN)
#define F_IBQDBGEN    V_IBQDBGEN(1U)

#define A_CIM_IBQ_DBG_DATA 0x2c8

#define A_TP_IN_CONFIG 0x300

#define S_RXFBARBPRIO    25
#define V_RXFBARBPRIO(x) ((x) << S_RXFBARBPRIO)
#define F_RXFBARBPRIO    V_RXFBARBPRIO(1U)

#define S_TXFBARBPRIO    24
#define V_TXFBARBPRIO(x) ((x) << S_TXFBARBPRIO)
#define F_TXFBARBPRIO    V_TXFBARBPRIO(1U)

#define S_NICMODE    14
#define V_NICMODE(x) ((x) << S_NICMODE)
#define F_NICMODE    V_NICMODE(1U)

#define F_NICMODE    V_NICMODE(1U)

#define S_IPV6ENABLE    15
#define V_IPV6ENABLE(x) ((x) << S_IPV6ENABLE)
#define F_IPV6ENABLE    V_IPV6ENABLE(1U)

#define A_TP_OUT_CONFIG 0x304

#define S_VLANEXTRACTIONENABLE    12

#define A_TP_GLOBAL_CONFIG 0x308

#define S_TXPACINGENABLE    24
#define V_TXPACINGENABLE(x) ((x) << S_TXPACINGENABLE)
#define F_TXPACINGENABLE    V_TXPACINGENABLE(1U)

#define S_PATHMTU    15
#define V_PATHMTU(x) ((x) << S_PATHMTU)
#define F_PATHMTU    V_PATHMTU(1U)

#define S_IPCHECKSUMOFFLOAD    13
#define V_IPCHECKSUMOFFLOAD(x) ((x) << S_IPCHECKSUMOFFLOAD)
#define F_IPCHECKSUMOFFLOAD    V_IPCHECKSUMOFFLOAD(1U)

#define S_UDPCHECKSUMOFFLOAD    12
#define V_UDPCHECKSUMOFFLOAD(x) ((x) << S_UDPCHECKSUMOFFLOAD)
#define F_UDPCHECKSUMOFFLOAD    V_UDPCHECKSUMOFFLOAD(1U)

#define S_TCPCHECKSUMOFFLOAD    11
#define V_TCPCHECKSUMOFFLOAD(x) ((x) << S_TCPCHECKSUMOFFLOAD)
#define F_TCPCHECKSUMOFFLOAD    V_TCPCHECKSUMOFFLOAD(1U)

#define S_IPTTL    0
#define M_IPTTL    0xff
#define V_IPTTL(x) ((x) << S_IPTTL)

#define A_TP_CMM_MM_BASE 0x314

#define A_TP_CMM_TIMER_BASE 0x318

#define S_CMTIMERMAXNUM    28
#define M_CMTIMERMAXNUM    0x3
#define V_CMTIMERMAXNUM(x) ((x) << S_CMTIMERMAXNUM)

#define A_TP_PMM_SIZE 0x31c

#define A_TP_PMM_TX_BASE 0x320

#define A_TP_PMM_RX_BASE 0x328

#define A_TP_PMM_RX_PAGE_SIZE 0x32c

#define A_TP_PMM_RX_MAX_PAGE 0x330

#define A_TP_PMM_TX_PAGE_SIZE 0x334

#define A_TP_PMM_TX_MAX_PAGE 0x338

#define A_TP_TCP_OPTIONS 0x340

#define S_MTUDEFAULT    16
#define M_MTUDEFAULT    0xffff
#define V_MTUDEFAULT(x) ((x) << S_MTUDEFAULT)

#define S_MTUENABLE    10
#define V_MTUENABLE(x) ((x) << S_MTUENABLE)
#define F_MTUENABLE    V_MTUENABLE(1U)

#define S_SACKRX    8
#define V_SACKRX(x) ((x) << S_SACKRX)
#define F_SACKRX    V_SACKRX(1U)

#define S_SACKMODE    4

#define M_SACKMODE    0x3

#define V_SACKMODE(x) ((x) << S_SACKMODE)

#define S_WINDOWSCALEMODE    2
#define M_WINDOWSCALEMODE    0x3
#define V_WINDOWSCALEMODE(x) ((x) << S_WINDOWSCALEMODE)

#define S_TIMESTAMPSMODE    0

#define M_TIMESTAMPSMODE    0x3

#define V_TIMESTAMPSMODE(x) ((x) << S_TIMESTAMPSMODE)

#define A_TP_DACK_CONFIG 0x344

#define S_AUTOSTATE3    30
#define M_AUTOSTATE3    0x3
#define V_AUTOSTATE3(x) ((x) << S_AUTOSTATE3)

#define S_AUTOSTATE2    28
#define M_AUTOSTATE2    0x3
#define V_AUTOSTATE2(x) ((x) << S_AUTOSTATE2)

#define S_AUTOSTATE1    26
#define M_AUTOSTATE1    0x3
#define V_AUTOSTATE1(x) ((x) << S_AUTOSTATE1)

#define S_BYTETHRESHOLD    5
#define M_BYTETHRESHOLD    0xfffff
#define V_BYTETHRESHOLD(x) ((x) << S_BYTETHRESHOLD)

#define S_MSSTHRESHOLD    3
#define M_MSSTHRESHOLD    0x3
#define V_MSSTHRESHOLD(x) ((x) << S_MSSTHRESHOLD)

#define S_AUTOCAREFUL    2
#define V_AUTOCAREFUL(x) ((x) << S_AUTOCAREFUL)
#define F_AUTOCAREFUL    V_AUTOCAREFUL(1U)

#define S_AUTOENABLE    1
#define V_AUTOENABLE(x) ((x) << S_AUTOENABLE)
#define F_AUTOENABLE    V_AUTOENABLE(1U)

#define S_DACK_MODE    0
#define V_DACK_MODE(x) ((x) << S_DACK_MODE)
#define F_DACK_MODE    V_DACK_MODE(1U)

#define A_TP_PC_CONFIG 0x348

#define S_TXTOSQUEUEMAPMODE    26
#define V_TXTOSQUEUEMAPMODE(x) ((x) << S_TXTOSQUEUEMAPMODE)
#define F_TXTOSQUEUEMAPMODE    V_TXTOSQUEUEMAPMODE(1U)

#define S_ENABLEEPCMDAFULL    23
#define V_ENABLEEPCMDAFULL(x) ((x) << S_ENABLEEPCMDAFULL)
#define F_ENABLEEPCMDAFULL    V_ENABLEEPCMDAFULL(1U)

#define S_MODULATEUNIONMODE    22
#define V_MODULATEUNIONMODE(x) ((x) << S_MODULATEUNIONMODE)
#define F_MODULATEUNIONMODE    V_MODULATEUNIONMODE(1U)

#define S_TXDEFERENABLE    20
#define V_TXDEFERENABLE(x) ((x) << S_TXDEFERENABLE)
#define F_TXDEFERENABLE    V_TXDEFERENABLE(1U)

#define S_RXCONGESTIONMODE    19
#define V_RXCONGESTIONMODE(x) ((x) << S_RXCONGESTIONMODE)
#define F_RXCONGESTIONMODE    V_RXCONGESTIONMODE(1U)

#define S_HEARBEATDACK    16
#define V_HEARBEATDACK(x) ((x) << S_HEARBEATDACK)
#define F_HEARBEATDACK    V_HEARBEATDACK(1U)

#define S_TXCONGESTIONMODE    15
#define V_TXCONGESTIONMODE(x) ((x) << S_TXCONGESTIONMODE)
#define F_TXCONGESTIONMODE    V_TXCONGESTIONMODE(1U)

#define S_ENABLEOCSPIFULL    30
#define V_ENABLEOCSPIFULL(x) ((x) << S_ENABLEOCSPIFULL)
#define F_ENABLEOCSPIFULL    V_ENABLEOCSPIFULL(1U)

#define S_LOCKTID    28
#define V_LOCKTID(x) ((x) << S_LOCKTID)
#define F_LOCKTID    V_LOCKTID(1U)

#define S_TABLELATENCYDELTA    0
#define M_TABLELATENCYDELTA    0xf
#define V_TABLELATENCYDELTA(x) ((x) << S_TABLELATENCYDELTA)
#define G_TABLELATENCYDELTA(x) \
	(((x) >> S_TABLELATENCYDELTA) & M_TABLELATENCYDELTA)

#define A_TP_PC_CONFIG2 0x34c

#define S_DISBLEDAPARBIT0    15
#define V_DISBLEDAPARBIT0(x) ((x) << S_DISBLEDAPARBIT0)
#define F_DISBLEDAPARBIT0    V_DISBLEDAPARBIT0(1U)

#define S_ENABLEARPMISS    13
#define V_ENABLEARPMISS(x) ((x) << S_ENABLEARPMISS)
#define F_ENABLEARPMISS    V_ENABLEARPMISS(1U)

#define S_ENABLENONOFDTNLSYN    12
#define V_ENABLENONOFDTNLSYN(x) ((x) << S_ENABLENONOFDTNLSYN)
#define F_ENABLENONOFDTNLSYN    V_ENABLENONOFDTNLSYN(1U)

#define S_ENABLEIPV6RSS    11
#define V_ENABLEIPV6RSS(x) ((x) << S_ENABLEIPV6RSS)
#define F_ENABLEIPV6RSS    V_ENABLEIPV6RSS(1U)

#define S_CHDRAFULL    4
#define V_CHDRAFULL(x) ((x) << S_CHDRAFULL)
#define F_CHDRAFULL    V_CHDRAFULL(1U)

#define A_TP_TCP_BACKOFF_REG0 0x350

#define A_TP_TCP_BACKOFF_REG1 0x354

#define A_TP_TCP_BACKOFF_REG2 0x358

#define A_TP_TCP_BACKOFF_REG3 0x35c

#define A_TP_PARA_REG2 0x368

#define S_MAXRXDATA    16
#define M_MAXRXDATA    0xffff
#define V_MAXRXDATA(x) ((x) << S_MAXRXDATA)

#define S_RXCOALESCESIZE    0
#define M_RXCOALESCESIZE    0xffff
#define V_RXCOALESCESIZE(x) ((x) << S_RXCOALESCESIZE)

#define A_TP_PARA_REG3 0x36c

#define S_TXDATAACKIDX    16
#define M_TXDATAACKIDX    0xf

#define V_TXDATAACKIDX(x) ((x) << S_TXDATAACKIDX)

#define S_TXPACEAUTOSTRICT    10
#define V_TXPACEAUTOSTRICT(x) ((x) << S_TXPACEAUTOSTRICT)
#define F_TXPACEAUTOSTRICT    V_TXPACEAUTOSTRICT(1U)

#define S_TXPACEFIXED    9
#define V_TXPACEFIXED(x) ((x) << S_TXPACEFIXED)
#define F_TXPACEFIXED    V_TXPACEFIXED(1U)

#define S_TXPACEAUTO    8
#define V_TXPACEAUTO(x) ((x) << S_TXPACEAUTO)
#define F_TXPACEAUTO    V_TXPACEAUTO(1U)

#define S_RXCOALESCEENABLE    1
#define V_RXCOALESCEENABLE(x) ((x) << S_RXCOALESCEENABLE)
#define F_RXCOALESCEENABLE    V_RXCOALESCEENABLE(1U)

#define S_RXCOALESCEPSHEN    0
#define V_RXCOALESCEPSHEN(x) ((x) << S_RXCOALESCEPSHEN)
#define F_RXCOALESCEPSHEN    V_RXCOALESCEPSHEN(1U)

#define A_TP_PARA_REG4 0x370

#define A_TP_PARA_REG5 0x374

#define S_RXDDPOFFINIT    3
#define V_RXDDPOFFINIT(x) ((x) << S_RXDDPOFFINIT)
#define F_RXDDPOFFINIT    V_RXDDPOFFINIT(1U)

#define A_TP_PARA_REG6 0x378

#define S_T3A_ENABLEESND    13
#define V_T3A_ENABLEESND(x) ((x) << S_T3A_ENABLEESND)
#define F_T3A_ENABLEESND    V_T3A_ENABLEESND(1U)

#define S_ENABLEESND    11
#define V_ENABLEESND(x) ((x) << S_ENABLEESND)
#define F_ENABLEESND    V_ENABLEESND(1U)

#define A_TP_PARA_REG7 0x37c

#define S_PMMAXXFERLEN1    16
#define M_PMMAXXFERLEN1    0xffff
#define V_PMMAXXFERLEN1(x) ((x) << S_PMMAXXFERLEN1)

#define S_PMMAXXFERLEN0    0
#define M_PMMAXXFERLEN0    0xffff
#define V_PMMAXXFERLEN0(x) ((x) << S_PMMAXXFERLEN0)

#define A_TP_TIMER_RESOLUTION 0x390

#define S_TIMERRESOLUTION    16
#define M_TIMERRESOLUTION    0xff
#define V_TIMERRESOLUTION(x) ((x) << S_TIMERRESOLUTION)

#define S_TIMESTAMPRESOLUTION    8
#define M_TIMESTAMPRESOLUTION    0xff
#define V_TIMESTAMPRESOLUTION(x) ((x) << S_TIMESTAMPRESOLUTION)

#define S_DELAYEDACKRESOLUTION    0
#define M_DELAYEDACKRESOLUTION    0xff
#define V_DELAYEDACKRESOLUTION(x) ((x) << S_DELAYEDACKRESOLUTION)

#define A_TP_MSL 0x394

#define A_TP_RXT_MIN 0x398

#define A_TP_RXT_MAX 0x39c

#define A_TP_PERS_MIN 0x3a0

#define A_TP_PERS_MAX 0x3a4

#define A_TP_KEEP_IDLE 0x3a8

#define A_TP_KEEP_INTVL 0x3ac

#define A_TP_INIT_SRTT 0x3b0

#define A_TP_DACK_TIMER 0x3b4

#define A_TP_FINWAIT2_TIMER 0x3b8

#define A_TP_SHIFT_CNT 0x3c0

#define S_SYNSHIFTMAX    24

#define M_SYNSHIFTMAX    0xff

#define V_SYNSHIFTMAX(x) ((x) << S_SYNSHIFTMAX)

#define S_RXTSHIFTMAXR1    20

#define M_RXTSHIFTMAXR1    0xf

#define V_RXTSHIFTMAXR1(x) ((x) << S_RXTSHIFTMAXR1)

#define S_RXTSHIFTMAXR2    16

#define M_RXTSHIFTMAXR2    0xf

#define V_RXTSHIFTMAXR2(x) ((x) << S_RXTSHIFTMAXR2)

#define S_PERSHIFTBACKOFFMAX    12
#define M_PERSHIFTBACKOFFMAX    0xf
#define V_PERSHIFTBACKOFFMAX(x) ((x) << S_PERSHIFTBACKOFFMAX)

#define S_PERSHIFTMAX    8
#define M_PERSHIFTMAX    0xf
#define V_PERSHIFTMAX(x) ((x) << S_PERSHIFTMAX)

#define S_KEEPALIVEMAX    0

#define M_KEEPALIVEMAX    0xff

#define V_KEEPALIVEMAX(x) ((x) << S_KEEPALIVEMAX)

#define A_TP_MTU_PORT_TABLE 0x3d0

#define A_TP_CCTRL_TABLE 0x3dc

#define A_TP_MTU_TABLE 0x3e4

#define A_TP_RSS_MAP_TABLE 0x3e8

#define A_TP_RSS_LKP_TABLE 0x3ec

#define A_TP_RSS_CONFIG 0x3f0

#define S_TNL4TUPEN    29
#define V_TNL4TUPEN(x) ((x) << S_TNL4TUPEN)
#define F_TNL4TUPEN    V_TNL4TUPEN(1U)

#define S_TNL2TUPEN    28
#define V_TNL2TUPEN(x) ((x) << S_TNL2TUPEN)
#define F_TNL2TUPEN    V_TNL2TUPEN(1U)

#define S_TNLPRTEN    26
#define V_TNLPRTEN(x) ((x) << S_TNLPRTEN)
#define F_TNLPRTEN    V_TNLPRTEN(1U)

#define S_TNLMAPEN    25
#define V_TNLMAPEN(x) ((x) << S_TNLMAPEN)
#define F_TNLMAPEN    V_TNLMAPEN(1U)

#define S_TNLLKPEN    24
#define V_TNLLKPEN(x) ((x) << S_TNLLKPEN)
#define F_TNLLKPEN    V_TNLLKPEN(1U)

#define S_RRCPLMAPEN    7
#define V_RRCPLMAPEN(x) ((x) << S_RRCPLMAPEN)
#define F_RRCPLMAPEN    V_RRCPLMAPEN(1U)

#define S_RRCPLCPUSIZE    4
#define M_RRCPLCPUSIZE    0x7
#define V_RRCPLCPUSIZE(x) ((x) << S_RRCPLCPUSIZE)

#define S_RQFEEDBACKENABLE    3
#define V_RQFEEDBACKENABLE(x) ((x) << S_RQFEEDBACKENABLE)
#define F_RQFEEDBACKENABLE    V_RQFEEDBACKENABLE(1U)

#define S_HASHTOEPLITZ    2
#define V_HASHTOEPLITZ(x) ((x) << S_HASHTOEPLITZ)
#define F_HASHTOEPLITZ    V_HASHTOEPLITZ(1U)

#define S_DISABLE    0

#define A_TP_TM_PIO_ADDR 0x418

#define A_TP_TM_PIO_DATA 0x41c

#define A_TP_TX_MOD_QUE_TABLE 0x420

#define A_TP_TX_RESOURCE_LIMIT 0x424

#define A_TP_TX_MOD_QUEUE_REQ_MAP 0x428

#define S_TX_MOD_QUEUE_REQ_MAP    0
#define M_TX_MOD_QUEUE_REQ_MAP    0xff
#define V_TX_MOD_QUEUE_REQ_MAP(x) ((x) << S_TX_MOD_QUEUE_REQ_MAP)

#define A_TP_TX_MOD_QUEUE_WEIGHT1 0x42c

#define A_TP_TX_MOD_QUEUE_WEIGHT0 0x430

#define A_TP_MOD_CHANNEL_WEIGHT 0x434

#define A_TP_MOD_RATE_LIMIT 0x438

#define A_TP_PIO_ADDR 0x440

#define A_TP_PIO_DATA 0x444

#define A_TP_RESET 0x44c

#define S_FLSTINITENABLE    1
#define V_FLSTINITENABLE(x) ((x) << S_FLSTINITENABLE)
#define F_FLSTINITENABLE    V_FLSTINITENABLE(1U)

#define S_TPRESET    0
#define V_TPRESET(x) ((x) << S_TPRESET)
#define F_TPRESET    V_TPRESET(1U)

#define A_TP_CMM_MM_RX_FLST_BASE 0x460

#define A_TP_CMM_MM_TX_FLST_BASE 0x464

#define A_TP_CMM_MM_PS_FLST_BASE 0x468

#define A_TP_MIB_INDEX 0x450

#define A_TP_MIB_RDATA 0x454

#define A_TP_CMM_MM_MAX_PSTRUCT 0x46c

#define A_TP_INT_ENABLE 0x470

#define S_FLMTXFLSTEMPTY    30
#define V_FLMTXFLSTEMPTY(x) ((x) << S_FLMTXFLSTEMPTY)
#define F_FLMTXFLSTEMPTY    V_FLMTXFLSTEMPTY(1U)

#define S_FLMRXFLSTEMPTY    29
#define V_FLMRXFLSTEMPTY(x) ((x) << S_FLMRXFLSTEMPTY)
#define F_FLMRXFLSTEMPTY    V_FLMRXFLSTEMPTY(1U)

#define S_ARPLUTPERR    26
#define V_ARPLUTPERR(x) ((x) << S_ARPLUTPERR)
#define F_ARPLUTPERR    V_ARPLUTPERR(1U)

#define S_CMCACHEPERR    24
#define V_CMCACHEPERR(x) ((x) << S_CMCACHEPERR)
#define F_CMCACHEPERR    V_CMCACHEPERR(1U)

#define A_TP_INT_CAUSE 0x474

#define A_TP_TX_MOD_Q1_Q0_RATE_LIMIT 0x8

#define A_TP_TX_DROP_CFG_CH0 0x12b

#define A_TP_TX_DROP_MODE 0x12f

#define A_TP_EGRESS_CONFIG 0x145

#define S_REWRITEFORCETOSIZE    0
#define V_REWRITEFORCETOSIZE(x) ((x) << S_REWRITEFORCETOSIZE)
#define F_REWRITEFORCETOSIZE    V_REWRITEFORCETOSIZE(1U)

#define A_TP_TX_TRC_KEY0 0x20

#define A_TP_RX_TRC_KEY0 0x120

#define A_TP_TX_DROP_CNT_CH0 0x12d

#define S_TXDROPCNTCH0RCVD    0
#define M_TXDROPCNTCH0RCVD    0xffff
#define V_TXDROPCNTCH0RCVD(x) ((x) << S_TXDROPCNTCH0RCVD)
#define G_TXDROPCNTCH0RCVD(x) (((x) >> S_TXDROPCNTCH0RCVD) & \
			       M_TXDROPCNTCH0RCVD)

#define A_TP_PROXY_FLOW_CNTL 0x4b0

#define A_TP_EMBED_OP_FIELD0 0x4e8
#define A_TP_EMBED_OP_FIELD1 0x4ec
#define A_TP_EMBED_OP_FIELD2 0x4f0
#define A_TP_EMBED_OP_FIELD3 0x4f4
#define A_TP_EMBED_OP_FIELD4 0x4f8
#define A_TP_EMBED_OP_FIELD5 0x4fc

#define A_ULPRX_CTL 0x500

#define S_ROUND_ROBIN    4
#define V_ROUND_ROBIN(x) ((x) << S_ROUND_ROBIN)
#define F_ROUND_ROBIN    V_ROUND_ROBIN(1U)

#define A_ULPRX_INT_ENABLE 0x504

#define S_DATASELFRAMEERR0    7
#define V_DATASELFRAMEERR0(x) ((x) << S_DATASELFRAMEERR0)
#define F_DATASELFRAMEERR0    V_DATASELFRAMEERR0(1U)

#define S_DATASELFRAMEERR1    6
#define V_DATASELFRAMEERR1(x) ((x) << S_DATASELFRAMEERR1)
#define F_DATASELFRAMEERR1    V_DATASELFRAMEERR1(1U)

#define S_PCMDMUXPERR    5
#define V_PCMDMUXPERR(x) ((x) << S_PCMDMUXPERR)
#define F_PCMDMUXPERR    V_PCMDMUXPERR(1U)

#define S_ARBFPERR    4
#define V_ARBFPERR(x) ((x) << S_ARBFPERR)
#define F_ARBFPERR    V_ARBFPERR(1U)

#define S_ARBPF0PERR    3
#define V_ARBPF0PERR(x) ((x) << S_ARBPF0PERR)
#define F_ARBPF0PERR    V_ARBPF0PERR(1U)

#define S_ARBPF1PERR    2
#define V_ARBPF1PERR(x) ((x) << S_ARBPF1PERR)
#define F_ARBPF1PERR    V_ARBPF1PERR(1U)

#define S_PARERRPCMD    1
#define V_PARERRPCMD(x) ((x) << S_PARERRPCMD)
#define F_PARERRPCMD    V_PARERRPCMD(1U)

#define S_PARERRDATA    0
#define V_PARERRDATA(x) ((x) << S_PARERRDATA)
#define F_PARERRDATA    V_PARERRDATA(1U)

#define A_ULPRX_INT_CAUSE 0x508

#define A_ULPRX_ISCSI_LLIMIT 0x50c

#define A_ULPRX_ISCSI_ULIMIT 0x510

#define A_ULPRX_ISCSI_TAGMASK 0x514

#define A_ULPRX_ISCSI_PSZ 0x518

#define A_ULPRX_TDDP_LLIMIT 0x51c

#define A_ULPRX_TDDP_ULIMIT 0x520
#define A_ULPRX_TDDP_PSZ 0x528

#define S_HPZ0    0
#define M_HPZ0    0xf
#define V_HPZ0(x) ((x) << S_HPZ0)
#define G_HPZ0(x) (((x) >> S_HPZ0) & M_HPZ0)

#define A_ULPRX_STAG_LLIMIT 0x52c

#define A_ULPRX_STAG_ULIMIT 0x530

#define A_ULPRX_RQ_LLIMIT 0x534
#define A_ULPRX_RQ_LLIMIT 0x534

#define A_ULPRX_RQ_ULIMIT 0x538
#define A_ULPRX_RQ_ULIMIT 0x538

#define A_ULPRX_PBL_LLIMIT 0x53c

#define A_ULPRX_PBL_ULIMIT 0x540
#define A_ULPRX_PBL_ULIMIT 0x540

#define A_ULPRX_TDDP_TAGMASK 0x524

#define A_ULPRX_RQ_LLIMIT 0x534
#define A_ULPRX_RQ_LLIMIT 0x534

#define A_ULPRX_RQ_ULIMIT 0x538
#define A_ULPRX_RQ_ULIMIT 0x538

#define A_ULPRX_PBL_ULIMIT 0x540
#define A_ULPRX_PBL_ULIMIT 0x540

#define A_ULPTX_CONFIG 0x580

#define S_CFG_CQE_SOP_MASK    1
#define V_CFG_CQE_SOP_MASK(x) ((x) << S_CFG_CQE_SOP_MASK)
#define F_CFG_CQE_SOP_MASK    V_CFG_CQE_SOP_MASK(1U)

#define S_CFG_RR_ARB    0
#define V_CFG_RR_ARB(x) ((x) << S_CFG_RR_ARB)
#define F_CFG_RR_ARB    V_CFG_RR_ARB(1U)

#define A_ULPTX_INT_ENABLE 0x584

#define S_PBL_BOUND_ERR_CH1    1
#define V_PBL_BOUND_ERR_CH1(x) ((x) << S_PBL_BOUND_ERR_CH1)
#define F_PBL_BOUND_ERR_CH1    V_PBL_BOUND_ERR_CH1(1U)

#define S_PBL_BOUND_ERR_CH0    0
#define V_PBL_BOUND_ERR_CH0(x) ((x) << S_PBL_BOUND_ERR_CH0)
#define F_PBL_BOUND_ERR_CH0    V_PBL_BOUND_ERR_CH0(1U)

#define A_ULPTX_INT_CAUSE 0x588

#define A_ULPTX_TPT_LLIMIT 0x58c

#define A_ULPTX_TPT_ULIMIT 0x590

#define A_ULPTX_PBL_LLIMIT 0x594

#define A_ULPTX_PBL_ULIMIT 0x598

#define A_ULPTX_DMA_WEIGHT 0x5ac

#define S_D1_WEIGHT    16
#define M_D1_WEIGHT    0xffff
#define V_D1_WEIGHT(x) ((x) << S_D1_WEIGHT)

#define S_D0_WEIGHT    0
#define M_D0_WEIGHT    0xffff
#define V_D0_WEIGHT(x) ((x) << S_D0_WEIGHT)

#define A_PM1_RX_CFG 0x5c0
#define A_PM1_RX_MODE 0x5c4

#define A_PM1_RX_INT_ENABLE 0x5d8

#define S_ZERO_E_CMD_ERROR    18
#define V_ZERO_E_CMD_ERROR(x) ((x) << S_ZERO_E_CMD_ERROR)
#define F_ZERO_E_CMD_ERROR    V_ZERO_E_CMD_ERROR(1U)

#define S_IESPI0_FIFO2X_RX_FRAMING_ERROR    17
#define V_IESPI0_FIFO2X_RX_FRAMING_ERROR(x) ((x) << S_IESPI0_FIFO2X_RX_FRAMING_ERROR)
#define F_IESPI0_FIFO2X_RX_FRAMING_ERROR    V_IESPI0_FIFO2X_RX_FRAMING_ERROR(1U)

#define S_IESPI1_FIFO2X_RX_FRAMING_ERROR    16
#define V_IESPI1_FIFO2X_RX_FRAMING_ERROR(x) ((x) << S_IESPI1_FIFO2X_RX_FRAMING_ERROR)
#define F_IESPI1_FIFO2X_RX_FRAMING_ERROR    V_IESPI1_FIFO2X_RX_FRAMING_ERROR(1U)

#define S_IESPI0_RX_FRAMING_ERROR    15
#define V_IESPI0_RX_FRAMING_ERROR(x) ((x) << S_IESPI0_RX_FRAMING_ERROR)
#define F_IESPI0_RX_FRAMING_ERROR    V_IESPI0_RX_FRAMING_ERROR(1U)

#define S_IESPI1_RX_FRAMING_ERROR    14
#define V_IESPI1_RX_FRAMING_ERROR(x) ((x) << S_IESPI1_RX_FRAMING_ERROR)
#define F_IESPI1_RX_FRAMING_ERROR    V_IESPI1_RX_FRAMING_ERROR(1U)

#define S_IESPI0_TX_FRAMING_ERROR    13
#define V_IESPI0_TX_FRAMING_ERROR(x) ((x) << S_IESPI0_TX_FRAMING_ERROR)
#define F_IESPI0_TX_FRAMING_ERROR    V_IESPI0_TX_FRAMING_ERROR(1U)

#define S_IESPI1_TX_FRAMING_ERROR    12
#define V_IESPI1_TX_FRAMING_ERROR(x) ((x) << S_IESPI1_TX_FRAMING_ERROR)
#define F_IESPI1_TX_FRAMING_ERROR    V_IESPI1_TX_FRAMING_ERROR(1U)

#define S_OCSPI0_RX_FRAMING_ERROR    11
#define V_OCSPI0_RX_FRAMING_ERROR(x) ((x) << S_OCSPI0_RX_FRAMING_ERROR)
#define F_OCSPI0_RX_FRAMING_ERROR    V_OCSPI0_RX_FRAMING_ERROR(1U)

#define S_OCSPI1_RX_FRAMING_ERROR    10
#define V_OCSPI1_RX_FRAMING_ERROR(x) ((x) << S_OCSPI1_RX_FRAMING_ERROR)
#define F_OCSPI1_RX_FRAMING_ERROR    V_OCSPI1_RX_FRAMING_ERROR(1U)

#define S_OCSPI0_TX_FRAMING_ERROR    9
#define V_OCSPI0_TX_FRAMING_ERROR(x) ((x) << S_OCSPI0_TX_FRAMING_ERROR)
#define F_OCSPI0_TX_FRAMING_ERROR    V_OCSPI0_TX_FRAMING_ERROR(1U)

#define S_OCSPI1_TX_FRAMING_ERROR    8
#define V_OCSPI1_TX_FRAMING_ERROR(x) ((x) << S_OCSPI1_TX_FRAMING_ERROR)
#define F_OCSPI1_TX_FRAMING_ERROR    V_OCSPI1_TX_FRAMING_ERROR(1U)

#define S_OCSPI0_OFIFO2X_TX_FRAMING_ERROR    7
#define V_OCSPI0_OFIFO2X_TX_FRAMING_ERROR(x) ((x) << S_OCSPI0_OFIFO2X_TX_FRAMING_ERROR)
#define F_OCSPI0_OFIFO2X_TX_FRAMING_ERROR    V_OCSPI0_OFIFO2X_TX_FRAMING_ERROR(1U)

#define S_OCSPI1_OFIFO2X_TX_FRAMING_ERROR    6
#define V_OCSPI1_OFIFO2X_TX_FRAMING_ERROR(x) ((x) << S_OCSPI1_OFIFO2X_TX_FRAMING_ERROR)
#define F_OCSPI1_OFIFO2X_TX_FRAMING_ERROR    V_OCSPI1_OFIFO2X_TX_FRAMING_ERROR(1U)

#define S_IESPI_PAR_ERROR    3
#define M_IESPI_PAR_ERROR    0x7

#define V_IESPI_PAR_ERROR(x) ((x) << S_IESPI_PAR_ERROR)

#define S_OCSPI_PAR_ERROR    0
#define M_OCSPI_PAR_ERROR    0x7

#define V_OCSPI_PAR_ERROR(x) ((x) << S_OCSPI_PAR_ERROR)

#define A_PM1_RX_INT_CAUSE 0x5dc

#define A_PM1_TX_CFG 0x5e0
#define A_PM1_TX_MODE 0x5e4

#define A_PM1_TX_INT_ENABLE 0x5f8

#define S_ZERO_C_CMD_ERROR    18
#define V_ZERO_C_CMD_ERROR(x) ((x) << S_ZERO_C_CMD_ERROR)
#define F_ZERO_C_CMD_ERROR    V_ZERO_C_CMD_ERROR(1U)

#define S_ICSPI0_FIFO2X_RX_FRAMING_ERROR    17
#define V_ICSPI0_FIFO2X_RX_FRAMING_ERROR(x) ((x) << S_ICSPI0_FIFO2X_RX_FRAMING_ERROR)
#define F_ICSPI0_FIFO2X_RX_FRAMING_ERROR    V_ICSPI0_FIFO2X_RX_FRAMING_ERROR(1U)

#define S_ICSPI1_FIFO2X_RX_FRAMING_ERROR    16
#define V_ICSPI1_FIFO2X_RX_FRAMING_ERROR(x) ((x) << S_ICSPI1_FIFO2X_RX_FRAMING_ERROR)
#define F_ICSPI1_FIFO2X_RX_FRAMING_ERROR    V_ICSPI1_FIFO2X_RX_FRAMING_ERROR(1U)

#define S_ICSPI0_RX_FRAMING_ERROR    15
#define V_ICSPI0_RX_FRAMING_ERROR(x) ((x) << S_ICSPI0_RX_FRAMING_ERROR)
#define F_ICSPI0_RX_FRAMING_ERROR    V_ICSPI0_RX_FRAMING_ERROR(1U)

#define S_ICSPI1_RX_FRAMING_ERROR    14
#define V_ICSPI1_RX_FRAMING_ERROR(x) ((x) << S_ICSPI1_RX_FRAMING_ERROR)
#define F_ICSPI1_RX_FRAMING_ERROR    V_ICSPI1_RX_FRAMING_ERROR(1U)

#define S_ICSPI0_TX_FRAMING_ERROR    13
#define V_ICSPI0_TX_FRAMING_ERROR(x) ((x) << S_ICSPI0_TX_FRAMING_ERROR)
#define F_ICSPI0_TX_FRAMING_ERROR    V_ICSPI0_TX_FRAMING_ERROR(1U)

#define S_ICSPI1_TX_FRAMING_ERROR    12
#define V_ICSPI1_TX_FRAMING_ERROR(x) ((x) << S_ICSPI1_TX_FRAMING_ERROR)
#define F_ICSPI1_TX_FRAMING_ERROR    V_ICSPI1_TX_FRAMING_ERROR(1U)

#define S_OESPI0_RX_FRAMING_ERROR    11
#define V_OESPI0_RX_FRAMING_ERROR(x) ((x) << S_OESPI0_RX_FRAMING_ERROR)
#define F_OESPI0_RX_FRAMING_ERROR    V_OESPI0_RX_FRAMING_ERROR(1U)

#define S_OESPI1_RX_FRAMING_ERROR    10
#define V_OESPI1_RX_FRAMING_ERROR(x) ((x) << S_OESPI1_RX_FRAMING_ERROR)
#define F_OESPI1_RX_FRAMING_ERROR    V_OESPI1_RX_FRAMING_ERROR(1U)

#define S_OESPI0_TX_FRAMING_ERROR    9
#define V_OESPI0_TX_FRAMING_ERROR(x) ((x) << S_OESPI0_TX_FRAMING_ERROR)
#define F_OESPI0_TX_FRAMING_ERROR    V_OESPI0_TX_FRAMING_ERROR(1U)

#define S_OESPI1_TX_FRAMING_ERROR    8
#define V_OESPI1_TX_FRAMING_ERROR(x) ((x) << S_OESPI1_TX_FRAMING_ERROR)
#define F_OESPI1_TX_FRAMING_ERROR    V_OESPI1_TX_FRAMING_ERROR(1U)

#define S_OESPI0_OFIFO2X_TX_FRAMING_ERROR    7
#define V_OESPI0_OFIFO2X_TX_FRAMING_ERROR(x) ((x) << S_OESPI0_OFIFO2X_TX_FRAMING_ERROR)
#define F_OESPI0_OFIFO2X_TX_FRAMING_ERROR    V_OESPI0_OFIFO2X_TX_FRAMING_ERROR(1U)

#define S_OESPI1_OFIFO2X_TX_FRAMING_ERROR    6
#define V_OESPI1_OFIFO2X_TX_FRAMING_ERROR(x) ((x) << S_OESPI1_OFIFO2X_TX_FRAMING_ERROR)
#define F_OESPI1_OFIFO2X_TX_FRAMING_ERROR    V_OESPI1_OFIFO2X_TX_FRAMING_ERROR(1U)

#define S_ICSPI_PAR_ERROR    3
#define M_ICSPI_PAR_ERROR    0x7

#define V_ICSPI_PAR_ERROR(x) ((x) << S_ICSPI_PAR_ERROR)

#define S_OESPI_PAR_ERROR    0
#define M_OESPI_PAR_ERROR    0x7

#define V_OESPI_PAR_ERROR(x) ((x) << S_OESPI_PAR_ERROR)

#define A_PM1_TX_INT_CAUSE 0x5fc

#define A_MPS_CFG 0x600

#define S_TPRXPORTEN    4
#define V_TPRXPORTEN(x) ((x) << S_TPRXPORTEN)
#define F_TPRXPORTEN    V_TPRXPORTEN(1U)

#define S_TPTXPORT1EN    3
#define V_TPTXPORT1EN(x) ((x) << S_TPTXPORT1EN)
#define F_TPTXPORT1EN    V_TPTXPORT1EN(1U)

#define S_TPTXPORT0EN    2
#define V_TPTXPORT0EN(x) ((x) << S_TPTXPORT0EN)
#define F_TPTXPORT0EN    V_TPTXPORT0EN(1U)

#define S_PORT1ACTIVE    1
#define V_PORT1ACTIVE(x) ((x) << S_PORT1ACTIVE)
#define F_PORT1ACTIVE    V_PORT1ACTIVE(1U)

#define S_PORT0ACTIVE    0
#define V_PORT0ACTIVE(x) ((x) << S_PORT0ACTIVE)
#define F_PORT0ACTIVE    V_PORT0ACTIVE(1U)

#define S_ENFORCEPKT    11
#define V_ENFORCEPKT(x) ((x) << S_ENFORCEPKT)
#define F_ENFORCEPKT    V_ENFORCEPKT(1U)

#define A_MPS_INT_ENABLE 0x61c

#define S_MCAPARERRENB    6
#define M_MCAPARERRENB    0x7

#define V_MCAPARERRENB(x) ((x) << S_MCAPARERRENB)

#define S_RXTPPARERRENB    4
#define M_RXTPPARERRENB    0x3

#define V_RXTPPARERRENB(x) ((x) << S_RXTPPARERRENB)

#define S_TX1TPPARERRENB    2
#define M_TX1TPPARERRENB    0x3

#define V_TX1TPPARERRENB(x) ((x) << S_TX1TPPARERRENB)

#define S_TX0TPPARERRENB    0
#define M_TX0TPPARERRENB    0x3

#define V_TX0TPPARERRENB(x) ((x) << S_TX0TPPARERRENB)

#define A_MPS_INT_CAUSE 0x620

#define S_MCAPARERR    6
#define M_MCAPARERR    0x7

#define V_MCAPARERR(x) ((x) << S_MCAPARERR)

#define S_RXTPPARERR    4
#define M_RXTPPARERR    0x3

#define V_RXTPPARERR(x) ((x) << S_RXTPPARERR)

#define S_TX1TPPARERR    2
#define M_TX1TPPARERR    0x3

#define V_TX1TPPARERR(x) ((x) << S_TX1TPPARERR)

#define S_TX0TPPARERR    0
#define M_TX0TPPARERR    0x3

#define V_TX0TPPARERR(x) ((x) << S_TX0TPPARERR)

#define A_CPL_SWITCH_CNTRL 0x640

#define A_CPL_INTR_ENABLE 0x650

#define S_CIM_OP_MAP_PERR    5
#define V_CIM_OP_MAP_PERR(x) ((x) << S_CIM_OP_MAP_PERR)
#define F_CIM_OP_MAP_PERR    V_CIM_OP_MAP_PERR(1U)

#define S_CIM_OVFL_ERROR    4
#define V_CIM_OVFL_ERROR(x) ((x) << S_CIM_OVFL_ERROR)
#define F_CIM_OVFL_ERROR    V_CIM_OVFL_ERROR(1U)

#define S_TP_FRAMING_ERROR    3
#define V_TP_FRAMING_ERROR(x) ((x) << S_TP_FRAMING_ERROR)
#define F_TP_FRAMING_ERROR    V_TP_FRAMING_ERROR(1U)

#define S_SGE_FRAMING_ERROR    2
#define V_SGE_FRAMING_ERROR(x) ((x) << S_SGE_FRAMING_ERROR)
#define F_SGE_FRAMING_ERROR    V_SGE_FRAMING_ERROR(1U)

#define S_CIM_FRAMING_ERROR    1
#define V_CIM_FRAMING_ERROR(x) ((x) << S_CIM_FRAMING_ERROR)
#define F_CIM_FRAMING_ERROR    V_CIM_FRAMING_ERROR(1U)

#define S_ZERO_SWITCH_ERROR    0
#define V_ZERO_SWITCH_ERROR(x) ((x) << S_ZERO_SWITCH_ERROR)
#define F_ZERO_SWITCH_ERROR    V_ZERO_SWITCH_ERROR(1U)

#define A_CPL_INTR_CAUSE 0x654

#define A_CPL_MAP_TBL_DATA 0x65c

#define A_SMB_GLOBAL_TIME_CFG 0x660

#define A_I2C_CFG 0x6a0

#define S_I2C_CLKDIV    0
#define M_I2C_CLKDIV    0xfff
#define V_I2C_CLKDIV(x) ((x) << S_I2C_CLKDIV)

#define A_MI1_CFG 0x6b0

#define S_CLKDIV    5
#define M_CLKDIV    0xff
#define V_CLKDIV(x) ((x) << S_CLKDIV)

#define S_ST    3

#define M_ST    0x3

#define V_ST(x) ((x) << S_ST)

#define G_ST(x) (((x) >> S_ST) & M_ST)

#define S_PREEN    2
#define V_PREEN(x) ((x) << S_PREEN)
#define F_PREEN    V_PREEN(1U)

#define S_MDIINV    1
#define V_MDIINV(x) ((x) << S_MDIINV)
#define F_MDIINV    V_MDIINV(1U)

#define S_MDIEN    0
#define V_MDIEN(x) ((x) << S_MDIEN)
#define F_MDIEN    V_MDIEN(1U)

#define A_MI1_ADDR 0x6b4

#define S_PHYADDR    5
#define M_PHYADDR    0x1f
#define V_PHYADDR(x) ((x) << S_PHYADDR)

#define S_REGADDR    0
#define M_REGADDR    0x1f
#define V_REGADDR(x) ((x) << S_REGADDR)

#define A_MI1_DATA 0x6b8

#define A_MI1_OP 0x6bc

#define S_MDI_OP    0
#define M_MDI_OP    0x3
#define V_MDI_OP(x) ((x) << S_MDI_OP)

#define A_SF_DATA 0x6d8

#define A_SF_OP 0x6dc

#define S_BYTECNT    1
#define M_BYTECNT    0x3
#define V_BYTECNT(x) ((x) << S_BYTECNT)

#define A_PL_INT_ENABLE0 0x6e0

#define S_T3DBG    23
#define V_T3DBG(x) ((x) << S_T3DBG)
#define F_T3DBG    V_T3DBG(1U)

#define S_XGMAC0_1    20
#define V_XGMAC0_1(x) ((x) << S_XGMAC0_1)
#define F_XGMAC0_1    V_XGMAC0_1(1U)

#define S_XGMAC0_0    19
#define V_XGMAC0_0(x) ((x) << S_XGMAC0_0)
#define F_XGMAC0_0    V_XGMAC0_0(1U)

#define S_MC5A    18
#define V_MC5A(x) ((x) << S_MC5A)
#define F_MC5A    V_MC5A(1U)

#define S_CPL_SWITCH    12
#define V_CPL_SWITCH(x) ((x) << S_CPL_SWITCH)
#define F_CPL_SWITCH    V_CPL_SWITCH(1U)

#define S_MPS0    11
#define V_MPS0(x) ((x) << S_MPS0)
#define F_MPS0    V_MPS0(1U)

#define S_PM1_TX    10
#define V_PM1_TX(x) ((x) << S_PM1_TX)
#define F_PM1_TX    V_PM1_TX(1U)

#define S_PM1_RX    9
#define V_PM1_RX(x) ((x) << S_PM1_RX)
#define F_PM1_RX    V_PM1_RX(1U)

#define S_ULP2_TX    8
#define V_ULP2_TX(x) ((x) << S_ULP2_TX)
#define F_ULP2_TX    V_ULP2_TX(1U)

#define S_ULP2_RX    7
#define V_ULP2_RX(x) ((x) << S_ULP2_RX)
#define F_ULP2_RX    V_ULP2_RX(1U)

#define S_TP1    6
#define V_TP1(x) ((x) << S_TP1)
#define F_TP1    V_TP1(1U)

#define S_CIM    5
#define V_CIM(x) ((x) << S_CIM)
#define F_CIM    V_CIM(1U)

#define S_MC7_CM    4
#define V_MC7_CM(x) ((x) << S_MC7_CM)
#define F_MC7_CM    V_MC7_CM(1U)

#define S_MC7_PMTX    3
#define V_MC7_PMTX(x) ((x) << S_MC7_PMTX)
#define F_MC7_PMTX    V_MC7_PMTX(1U)

#define S_MC7_PMRX    2
#define V_MC7_PMRX(x) ((x) << S_MC7_PMRX)
#define F_MC7_PMRX    V_MC7_PMRX(1U)

#define S_PCIM0    1
#define V_PCIM0(x) ((x) << S_PCIM0)
#define F_PCIM0    V_PCIM0(1U)

#define S_SGE3    0
#define V_SGE3(x) ((x) << S_SGE3)
#define F_SGE3    V_SGE3(1U)

#define A_PL_INT_CAUSE0 0x6e4

#define A_PL_RST 0x6f0

#define S_CRSTWRM    1
#define V_CRSTWRM(x) ((x) << S_CRSTWRM)
#define F_CRSTWRM    V_CRSTWRM(1U)

#define A_PL_REV 0x6f4

#define A_PL_CLI 0x6f8

#define A_MC5_DB_CONFIG 0x704

#define S_TMTYPEHI    30
#define V_TMTYPEHI(x) ((x) << S_TMTYPEHI)
#define F_TMTYPEHI    V_TMTYPEHI(1U)

#define S_TMPARTSIZE    28
#define M_TMPARTSIZE    0x3
#define V_TMPARTSIZE(x) ((x) << S_TMPARTSIZE)
#define G_TMPARTSIZE(x) (((x) >> S_TMPARTSIZE) & M_TMPARTSIZE)

#define S_TMTYPE    26
#define M_TMTYPE    0x3
#define V_TMTYPE(x) ((x) << S_TMTYPE)
#define G_TMTYPE(x) (((x) >> S_TMTYPE) & M_TMTYPE)

#define S_COMPEN    17
#define V_COMPEN(x) ((x) << S_COMPEN)
#define F_COMPEN    V_COMPEN(1U)

#define S_PRTYEN    6
#define V_PRTYEN(x) ((x) << S_PRTYEN)
#define F_PRTYEN    V_PRTYEN(1U)

#define S_MBUSEN    5
#define V_MBUSEN(x) ((x) << S_MBUSEN)
#define F_MBUSEN    V_MBUSEN(1U)

#define S_DBGIEN    4
#define V_DBGIEN(x) ((x) << S_DBGIEN)
#define F_DBGIEN    V_DBGIEN(1U)

#define S_TMRDY    2
#define V_TMRDY(x) ((x) << S_TMRDY)
#define F_TMRDY    V_TMRDY(1U)

#define S_TMRST    1
#define V_TMRST(x) ((x) << S_TMRST)
#define F_TMRST    V_TMRST(1U)

#define S_TMMODE    0
#define V_TMMODE(x) ((x) << S_TMMODE)
#define F_TMMODE    V_TMMODE(1U)

#define F_TMMODE    V_TMMODE(1U)

#define A_MC5_DB_ROUTING_TABLE_INDEX 0x70c

#define A_MC5_DB_FILTER_TABLE 0x710

#define A_MC5_DB_SERVER_INDEX 0x714

#define A_MC5_DB_RSP_LATENCY 0x720

#define S_RDLAT    16
#define M_RDLAT    0x1f
#define V_RDLAT(x) ((x) << S_RDLAT)

#define S_LRNLAT    8
#define M_LRNLAT    0x1f
#define V_LRNLAT(x) ((x) << S_LRNLAT)

#define S_SRCHLAT    0
#define M_SRCHLAT    0x1f
#define V_SRCHLAT(x) ((x) << S_SRCHLAT)

#define A_MC5_DB_PART_ID_INDEX 0x72c

#define A_MC5_DB_INT_ENABLE 0x740

#define S_DELACTEMPTY    18
#define V_DELACTEMPTY(x) ((x) << S_DELACTEMPTY)
#define F_DELACTEMPTY    V_DELACTEMPTY(1U)

#define S_DISPQPARERR    17
#define V_DISPQPARERR(x) ((x) << S_DISPQPARERR)
#define F_DISPQPARERR    V_DISPQPARERR(1U)

#define S_REQQPARERR    16
#define V_REQQPARERR(x) ((x) << S_REQQPARERR)
#define F_REQQPARERR    V_REQQPARERR(1U)

#define S_UNKNOWNCMD    15
#define V_UNKNOWNCMD(x) ((x) << S_UNKNOWNCMD)
#define F_UNKNOWNCMD    V_UNKNOWNCMD(1U)

#define S_NFASRCHFAIL    8
#define V_NFASRCHFAIL(x) ((x) << S_NFASRCHFAIL)
#define F_NFASRCHFAIL    V_NFASRCHFAIL(1U)

#define S_ACTRGNFULL    7
#define V_ACTRGNFULL(x) ((x) << S_ACTRGNFULL)
#define F_ACTRGNFULL    V_ACTRGNFULL(1U)

#define S_PARITYERR    6
#define V_PARITYERR(x) ((x) << S_PARITYERR)
#define F_PARITYERR    V_PARITYERR(1U)

#define A_MC5_DB_INT_CAUSE 0x744

#define A_MC5_DB_DBGI_CONFIG 0x774

#define A_MC5_DB_DBGI_REQ_CMD 0x778

#define A_MC5_DB_DBGI_REQ_ADDR0 0x77c

#define A_MC5_DB_DBGI_REQ_ADDR1 0x780

#define A_MC5_DB_DBGI_REQ_ADDR2 0x784

#define A_MC5_DB_DBGI_REQ_DATA0 0x788

#define A_MC5_DB_DBGI_REQ_DATA1 0x78c

#define A_MC5_DB_DBGI_REQ_DATA2 0x790

#define A_MC5_DB_DBGI_RSP_STATUS 0x7b0

#define S_DBGIRSPVALID    0
#define V_DBGIRSPVALID(x) ((x) << S_DBGIRSPVALID)
#define F_DBGIRSPVALID    V_DBGIRSPVALID(1U)

#define A_MC5_DB_DBGI_RSP_DATA0 0x7b4

#define A_MC5_DB_DBGI_RSP_DATA1 0x7b8

#define A_MC5_DB_DBGI_RSP_DATA2 0x7bc

#define A_MC5_DB_POPEN_DATA_WR_CMD 0x7cc

#define A_MC5_DB_POPEN_MASK_WR_CMD 0x7d0

#define A_MC5_DB_AOPEN_SRCH_CMD 0x7d4

#define A_MC5_DB_AOPEN_LRN_CMD 0x7d8

#define A_MC5_DB_SYN_SRCH_CMD 0x7dc

#define A_MC5_DB_SYN_LRN_CMD 0x7e0

#define A_MC5_DB_ACK_SRCH_CMD 0x7e4

#define A_MC5_DB_ACK_LRN_CMD 0x7e8

#define A_MC5_DB_ILOOKUP_CMD 0x7ec

#define A_MC5_DB_ELOOKUP_CMD 0x7f0

#define A_MC5_DB_DATA_WRITE_CMD 0x7f4

#define A_MC5_DB_DATA_READ_CMD 0x7f8

#define XGMAC0_0_BASE_ADDR 0x800

#define A_XGM_TX_CTRL 0x800

#define S_TXEN    0
#define V_TXEN(x) ((x) << S_TXEN)
#define F_TXEN    V_TXEN(1U)

#define A_XGM_TX_CFG 0x804

#define S_TXPAUSEEN    0
#define V_TXPAUSEEN(x) ((x) << S_TXPAUSEEN)
#define F_TXPAUSEEN    V_TXPAUSEEN(1U)

#define A_XGM_TX_PAUSE_QUANTA 0x808

#define A_XGM_RX_CTRL 0x80c

#define S_RXEN    0
#define V_RXEN(x) ((x) << S_RXEN)
#define F_RXEN    V_RXEN(1U)

#define A_XGM_RX_CFG 0x810

#define S_DISPAUSEFRAMES    9
#define V_DISPAUSEFRAMES(x) ((x) << S_DISPAUSEFRAMES)
#define F_DISPAUSEFRAMES    V_DISPAUSEFRAMES(1U)

#define S_EN1536BFRAMES    8
#define V_EN1536BFRAMES(x) ((x) << S_EN1536BFRAMES)
#define F_EN1536BFRAMES    V_EN1536BFRAMES(1U)

#define S_ENJUMBO    7
#define V_ENJUMBO(x) ((x) << S_ENJUMBO)
#define F_ENJUMBO    V_ENJUMBO(1U)

#define S_RMFCS    6
#define V_RMFCS(x) ((x) << S_RMFCS)
#define F_RMFCS    V_RMFCS(1U)

#define S_ENHASHMCAST    2
#define V_ENHASHMCAST(x) ((x) << S_ENHASHMCAST)
#define F_ENHASHMCAST    V_ENHASHMCAST(1U)

#define S_COPYALLFRAMES    0
#define V_COPYALLFRAMES(x) ((x) << S_COPYALLFRAMES)
#define F_COPYALLFRAMES    V_COPYALLFRAMES(1U)

#define S_DISBCAST    1
#define V_DISBCAST(x) ((x) << S_DISBCAST)
#define F_DISBCAST    V_DISBCAST(1U)

#define A_XGM_RX_HASH_LOW 0x814

#define A_XGM_RX_HASH_HIGH 0x818

#define A_XGM_RX_EXACT_MATCH_LOW_1 0x81c

#define A_XGM_RX_EXACT_MATCH_HIGH_1 0x820

#define A_XGM_RX_EXACT_MATCH_LOW_2 0x824

#define A_XGM_RX_EXACT_MATCH_LOW_3 0x82c

#define A_XGM_RX_EXACT_MATCH_LOW_4 0x834

#define A_XGM_RX_EXACT_MATCH_LOW_5 0x83c

#define A_XGM_RX_EXACT_MATCH_LOW_6 0x844

#define A_XGM_RX_EXACT_MATCH_LOW_7 0x84c

#define A_XGM_RX_EXACT_MATCH_LOW_8 0x854

#define A_XGM_INT_STATUS 0x86c

#define S_LINKFAULTCHANGE    9
#define V_LINKFAULTCHANGE(x) ((x) << S_LINKFAULTCHANGE)
#define F_LINKFAULTCHANGE    V_LINKFAULTCHANGE(1U)

#define A_XGM_XGM_INT_ENABLE 0x874
#define A_XGM_XGM_INT_DISABLE 0x878

#define A_XGM_STAT_CTRL 0x880

#define S_CLRSTATS    2
#define V_CLRSTATS(x) ((x) << S_CLRSTATS)
#define F_CLRSTATS    V_CLRSTATS(1U)

#define A_XGM_RXFIFO_CFG 0x884

#define S_RXFIFO_EMPTY    31
#define V_RXFIFO_EMPTY(x) ((x) << S_RXFIFO_EMPTY)
#define F_RXFIFO_EMPTY    V_RXFIFO_EMPTY(1U)

#define S_RXFIFOPAUSEHWM    17
#define M_RXFIFOPAUSEHWM    0xfff

#define V_RXFIFOPAUSEHWM(x) ((x) << S_RXFIFOPAUSEHWM)

#define G_RXFIFOPAUSEHWM(x) (((x) >> S_RXFIFOPAUSEHWM) & M_RXFIFOPAUSEHWM)

#define S_RXFIFOPAUSELWM    5
#define M_RXFIFOPAUSELWM    0xfff

#define V_RXFIFOPAUSELWM(x) ((x) << S_RXFIFOPAUSELWM)

#define G_RXFIFOPAUSELWM(x) (((x) >> S_RXFIFOPAUSELWM) & M_RXFIFOPAUSELWM)

#define S_RXSTRFRWRD    1
#define V_RXSTRFRWRD(x) ((x) << S_RXSTRFRWRD)
#define F_RXSTRFRWRD    V_RXSTRFRWRD(1U)

#define S_DISERRFRAMES    0
#define V_DISERRFRAMES(x) ((x) << S_DISERRFRAMES)
#define F_DISERRFRAMES    V_DISERRFRAMES(1U)

#define A_XGM_TXFIFO_CFG 0x888

#define S_UNDERUNFIX    22
#define V_UNDERUNFIX(x) ((x) << S_UNDERUNFIX)
#define F_UNDERUNFIX    V_UNDERUNFIX(1U)

#define S_TXIPG    13
#define M_TXIPG    0xff
#define V_TXIPG(x) ((x) << S_TXIPG)
#define G_TXIPG(x) (((x) >> S_TXIPG) & M_TXIPG)

#define S_TXFIFOTHRESH    4
#define M_TXFIFOTHRESH    0x1ff

#define V_TXFIFOTHRESH(x) ((x) << S_TXFIFOTHRESH)

#define S_ENDROPPKT    21
#define V_ENDROPPKT(x) ((x) << S_ENDROPPKT)
#define F_ENDROPPKT    V_ENDROPPKT(1U)

#define A_XGM_SERDES_CTRL 0x890
#define A_XGM_SERDES_CTRL0 0x8e0

#define S_SERDESRESET_    24
#define V_SERDESRESET_(x) ((x) << S_SERDESRESET_)
#define F_SERDESRESET_    V_SERDESRESET_(1U)

#define S_RXENABLE    4
#define V_RXENABLE(x) ((x) << S_RXENABLE)
#define F_RXENABLE    V_RXENABLE(1U)

#define S_TXENABLE    3
#define V_TXENABLE(x) ((x) << S_TXENABLE)
#define F_TXENABLE    V_TXENABLE(1U)

#define A_XGM_PAUSE_TIMER 0x890

#define A_XGM_RGMII_IMP 0x89c

#define S_XGM_IMPSETUPDATE    6
#define V_XGM_IMPSETUPDATE(x) ((x) << S_XGM_IMPSETUPDATE)
#define F_XGM_IMPSETUPDATE    V_XGM_IMPSETUPDATE(1U)

#define S_RGMIIIMPPD    3
#define M_RGMIIIMPPD    0x7
#define V_RGMIIIMPPD(x) ((x) << S_RGMIIIMPPD)

#define S_RGMIIIMPPU    0
#define M_RGMIIIMPPU    0x7
#define V_RGMIIIMPPU(x) ((x) << S_RGMIIIMPPU)

#define S_CALRESET    8
#define V_CALRESET(x) ((x) << S_CALRESET)
#define F_CALRESET    V_CALRESET(1U)

#define S_CALUPDATE    7
#define V_CALUPDATE(x) ((x) << S_CALUPDATE)
#define F_CALUPDATE    V_CALUPDATE(1U)

#define A_XGM_XAUI_IMP 0x8a0

#define S_CALBUSY    31
#define V_CALBUSY(x) ((x) << S_CALBUSY)
#define F_CALBUSY    V_CALBUSY(1U)

#define S_XGM_CALFAULT    29
#define V_XGM_CALFAULT(x) ((x) << S_XGM_CALFAULT)
#define F_XGM_CALFAULT    V_XGM_CALFAULT(1U)

#define S_CALIMP    24
#define M_CALIMP    0x1f
#define V_CALIMP(x) ((x) << S_CALIMP)
#define G_CALIMP(x) (((x) >> S_CALIMP) & M_CALIMP)

#define S_XAUIIMP    0
#define M_XAUIIMP    0x7
#define V_XAUIIMP(x) ((x) << S_XAUIIMP)

#define A_XGM_RX_MAX_PKT_SIZE 0x8a8

#define S_RXMAXFRAMERSIZE    17
#define M_RXMAXFRAMERSIZE    0x3fff
#define V_RXMAXFRAMERSIZE(x) ((x) << S_RXMAXFRAMERSIZE)
#define G_RXMAXFRAMERSIZE(x) (((x) >> S_RXMAXFRAMERSIZE) & M_RXMAXFRAMERSIZE)

#define S_RXENFRAMER    14
#define V_RXENFRAMER(x) ((x) << S_RXENFRAMER)
#define F_RXENFRAMER    V_RXENFRAMER(1U)

#define S_RXMAXPKTSIZE    0
#define M_RXMAXPKTSIZE    0x3fff
#define V_RXMAXPKTSIZE(x) ((x) << S_RXMAXPKTSIZE)
#define G_RXMAXPKTSIZE(x) (((x) >> S_RXMAXPKTSIZE) & M_RXMAXPKTSIZE)

#define A_XGM_RESET_CTRL 0x8ac

#define S_XGMAC_STOP_EN    4
#define V_XGMAC_STOP_EN(x) ((x) << S_XGMAC_STOP_EN)
#define F_XGMAC_STOP_EN    V_XGMAC_STOP_EN(1U)

#define S_XG2G_RESET_    3
#define V_XG2G_RESET_(x) ((x) << S_XG2G_RESET_)
#define F_XG2G_RESET_    V_XG2G_RESET_(1U)

#define S_RGMII_RESET_    2
#define V_RGMII_RESET_(x) ((x) << S_RGMII_RESET_)
#define F_RGMII_RESET_    V_RGMII_RESET_(1U)

#define S_PCS_RESET_    1
#define V_PCS_RESET_(x) ((x) << S_PCS_RESET_)
#define F_PCS_RESET_    V_PCS_RESET_(1U)

#define S_MAC_RESET_    0
#define V_MAC_RESET_(x) ((x) << S_MAC_RESET_)
#define F_MAC_RESET_    V_MAC_RESET_(1U)

#define A_XGM_PORT_CFG 0x8b8

#define S_CLKDIVRESET_    3
#define V_CLKDIVRESET_(x) ((x) << S_CLKDIVRESET_)
#define F_CLKDIVRESET_    V_CLKDIVRESET_(1U)

#define S_PORTSPEED    1
#define M_PORTSPEED    0x3

#define V_PORTSPEED(x) ((x) << S_PORTSPEED)

#define S_ENRGMII    0
#define V_ENRGMII(x) ((x) << S_ENRGMII)
#define F_ENRGMII    V_ENRGMII(1U)

#define A_XGM_INT_ENABLE 0x8d4

#define S_TXFIFO_PRTY_ERR    17
#define M_TXFIFO_PRTY_ERR    0x7

#define V_TXFIFO_PRTY_ERR(x) ((x) << S_TXFIFO_PRTY_ERR)

#define S_RXFIFO_PRTY_ERR    14
#define M_RXFIFO_PRTY_ERR    0x7

#define V_RXFIFO_PRTY_ERR(x) ((x) << S_RXFIFO_PRTY_ERR)

#define S_TXFIFO_UNDERRUN    13
#define V_TXFIFO_UNDERRUN(x) ((x) << S_TXFIFO_UNDERRUN)
#define F_TXFIFO_UNDERRUN    V_TXFIFO_UNDERRUN(1U)

#define S_RXFIFO_OVERFLOW    12
#define V_RXFIFO_OVERFLOW(x) ((x) << S_RXFIFO_OVERFLOW)
#define F_RXFIFO_OVERFLOW    V_RXFIFO_OVERFLOW(1U)

#define S_SERDES_LOS    4
#define M_SERDES_LOS    0xf

#define V_SERDES_LOS(x) ((x) << S_SERDES_LOS)

#define S_XAUIPCSCTCERR    3
#define V_XAUIPCSCTCERR(x) ((x) << S_XAUIPCSCTCERR)
#define F_XAUIPCSCTCERR    V_XAUIPCSCTCERR(1U)

#define S_XAUIPCSALIGNCHANGE    2
#define V_XAUIPCSALIGNCHANGE(x) ((x) << S_XAUIPCSALIGNCHANGE)
#define F_XAUIPCSALIGNCHANGE    V_XAUIPCSALIGNCHANGE(1U)

#define S_XGM_INT    0
#define V_XGM_INT(x) ((x) << S_XGM_INT)
#define F_XGM_INT    V_XGM_INT(1U)

#define A_XGM_INT_CAUSE 0x8d8

#define A_XGM_XAUI_ACT_CTRL 0x8dc

#define S_TXACTENABLE    1
#define V_TXACTENABLE(x) ((x) << S_TXACTENABLE)
#define F_TXACTENABLE    V_TXACTENABLE(1U)

#define A_XGM_SERDES_CTRL0 0x8e0

#define S_RESET3    23
#define V_RESET3(x) ((x) << S_RESET3)
#define F_RESET3    V_RESET3(1U)

#define S_RESET2    22
#define V_RESET2(x) ((x) << S_RESET2)
#define F_RESET2    V_RESET2(1U)

#define S_RESET1    21
#define V_RESET1(x) ((x) << S_RESET1)
#define F_RESET1    V_RESET1(1U)

#define S_RESET0    20
#define V_RESET0(x) ((x) << S_RESET0)
#define F_RESET0    V_RESET0(1U)

#define S_PWRDN3    19
#define V_PWRDN3(x) ((x) << S_PWRDN3)
#define F_PWRDN3    V_PWRDN3(1U)

#define S_PWRDN2    18
#define V_PWRDN2(x) ((x) << S_PWRDN2)
#define F_PWRDN2    V_PWRDN2(1U)

#define S_PWRDN1    17
#define V_PWRDN1(x) ((x) << S_PWRDN1)
#define F_PWRDN1    V_PWRDN1(1U)

#define S_PWRDN0    16
#define V_PWRDN0(x) ((x) << S_PWRDN0)
#define F_PWRDN0    V_PWRDN0(1U)

#define S_RESETPLL23    15
#define V_RESETPLL23(x) ((x) << S_RESETPLL23)
#define F_RESETPLL23    V_RESETPLL23(1U)

#define S_RESETPLL01    14
#define V_RESETPLL01(x) ((x) << S_RESETPLL01)
#define F_RESETPLL01    V_RESETPLL01(1U)

#define A_XGM_SERDES_STAT0 0x8f0
#define A_XGM_SERDES_STAT1 0x8f4
#define A_XGM_SERDES_STAT2 0x8f8

#define S_LOWSIG0    0
#define V_LOWSIG0(x) ((x) << S_LOWSIG0)
#define F_LOWSIG0    V_LOWSIG0(1U)

#define A_XGM_SERDES_STAT3 0x8fc

#define A_XGM_STAT_TX_BYTE_LOW 0x900

#define A_XGM_STAT_TX_BYTE_HIGH 0x904

#define A_XGM_STAT_TX_FRAME_LOW 0x908

#define A_XGM_STAT_TX_FRAME_HIGH 0x90c

#define A_XGM_STAT_TX_BCAST 0x910

#define A_XGM_STAT_TX_MCAST 0x914

#define A_XGM_STAT_TX_PAUSE 0x918

#define A_XGM_STAT_TX_64B_FRAMES 0x91c

#define A_XGM_STAT_TX_65_127B_FRAMES 0x920

#define A_XGM_STAT_TX_128_255B_FRAMES 0x924

#define A_XGM_STAT_TX_256_511B_FRAMES 0x928

#define A_XGM_STAT_TX_512_1023B_FRAMES 0x92c

#define A_XGM_STAT_TX_1024_1518B_FRAMES 0x930

#define A_XGM_STAT_TX_1519_MAXB_FRAMES 0x934

#define A_XGM_STAT_TX_ERR_FRAMES 0x938

#define A_XGM_STAT_RX_BYTES_LOW 0x93c

#define A_XGM_STAT_RX_BYTES_HIGH 0x940

#define A_XGM_STAT_RX_FRAMES_LOW 0x944

#define A_XGM_STAT_RX_FRAMES_HIGH 0x948

#define A_XGM_STAT_RX_BCAST_FRAMES 0x94c

#define A_XGM_STAT_RX_MCAST_FRAMES 0x950

#define A_XGM_STAT_RX_PAUSE_FRAMES 0x954

#define A_XGM_STAT_RX_64B_FRAMES 0x958

#define A_XGM_STAT_RX_65_127B_FRAMES 0x95c

#define A_XGM_STAT_RX_128_255B_FRAMES 0x960

#define A_XGM_STAT_RX_256_511B_FRAMES 0x964

#define A_XGM_STAT_RX_512_1023B_FRAMES 0x968

#define A_XGM_STAT_RX_1024_1518B_FRAMES 0x96c

#define A_XGM_STAT_RX_1519_MAXB_FRAMES 0x970

#define A_XGM_STAT_RX_SHORT_FRAMES 0x974

#define A_XGM_STAT_RX_OVERSIZE_FRAMES 0x978

#define A_XGM_STAT_RX_JABBER_FRAMES 0x97c

#define A_XGM_STAT_RX_CRC_ERR_FRAMES 0x980

#define A_XGM_STAT_RX_LENGTH_ERR_FRAMES 0x984

#define A_XGM_STAT_RX_SYM_CODE_ERR_FRAMES 0x988

#define A_XGM_SERDES_STATUS0 0x98c

#define A_XGM_SERDES_STATUS1 0x990

#define S_CMULOCK    31
#define V_CMULOCK(x) ((x) << S_CMULOCK)
#define F_CMULOCK    V_CMULOCK(1U)

#define A_XGM_RX_MAX_PKT_SIZE_ERR_CNT 0x9a4

#define A_XGM_TX_SPI4_SOP_EOP_CNT 0x9a8

#define S_TXSPI4SOPCNT    16
#define M_TXSPI4SOPCNT    0xffff
#define V_TXSPI4SOPCNT(x) ((x) << S_TXSPI4SOPCNT)
#define G_TXSPI4SOPCNT(x) (((x) >> S_TXSPI4SOPCNT) & M_TXSPI4SOPCNT)

#define A_XGM_RX_SPI4_SOP_EOP_CNT 0x9ac

#define XGMAC0_1_BASE_ADDR 0xa00
