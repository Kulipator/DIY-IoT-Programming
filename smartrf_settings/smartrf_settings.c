//*********************************************************************************
// Generated by SmartRF Studio version 2.6.0 (build #81)
// Tested for SimpleLink SDK version: CC13x0 SDK 1.0.xx
// Device: CC1350 Rev. 2.1
//
//*********************************************************************************


//*********************************************************************************


#include <ti/drivers/rf/RF.h>
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/rf_mailbox.h)
#include DeviceFamily_constructPath(driverlib/rf_common_cmd.h)
#include DeviceFamily_constructPath(driverlib/rf_prop_cmd.h)
#include <ti/devices/cc13x0/inc/hw_rfc_dbell.h>
#include DeviceFamily_constructPath(rf_patches/rf_patch_cpe_genfsk.h)
#include DeviceFamily_constructPath(rf_patches/rf_patch_rfe_genfsk.h)
#include "smartrf_settings.h"


#define BIAS_MODE_USE_EXTERNAL_COIL  1
#define BIAS_MODE_USE_INTERNAL_COIL  0

// TI-RTOS RF Mode Object
RF_Mode RF_prop =
{
    .rfMode = RF_MODE_PROPRIETARY_SUB_1,
    .cpePatchFxn = &rf_patch_cpe_genfsk,
    .mcePatchFxn = 0,
    .rfePatchFxn = &rf_patch_rfe_genfsk,
};

// Overrides for CMD_PROP_RADIO_DIV_SETUP
uint32_t pOverrides[] =
{
     // override_use_patch_prop_genfsk.xml
     // PHY: Use MCE ROM bank 4, RFE RAM patch
     MCE_RFE_OVERRIDE(0,4,0,1,0,0),
     // override_synth_prop_430_510_div10.xml
     // Synth: Set recommended RTRIM to 7
     HW_REG_OVERRIDE(0x4038,0x0037),
     // Synth: Set Fref to 4 MHz
     (uint32_t)0x000684A3,
     // Synth: Configure fine calibration setting
     HW_REG_OVERRIDE(0x4020,0x7F00),
     // Synth: Configure fine calibration setting
     HW_REG_OVERRIDE(0x4064,0x0040),
     // Synth: Configure fine calibration setting
     (uint32_t)0xB1070503,
     // Synth: Configure fine calibration setting
     (uint32_t)0x05330523,
     // Synth: Set loop bandwidth after lock to 20 kHz
     (uint32_t)0x0A480583,
     // Synth: Set loop bandwidth after lock to 20 kHz
     (uint32_t)0x7AB80603,
     // Synth: Configure VCO LDO (in ADI1, set VCOLDOCFG=0x9F to use voltage input reference)
     ADI_REG_OVERRIDE(1,4,0x9F),
     // Synth: Configure synth LDO (in ADI1, set SLDOCTL0.COMP_CAP=1)
     ADI_HALFREG_OVERRIDE(1,7,0x4,0x4),
     // Synth: Use 24 MHz XOSC as synth clock, enable extra PLL filtering
     (uint32_t)0x02010403,
     // Synth: Configure extra PLL filtering
     (uint32_t)0x00108463,
     // Synth: Increase synth programming timeout (0x04B0 RAT ticks = 300 us)
     (uint32_t)0x04B00243,
     // override_synth_disable_bias_div10.xml
     // Synth: Set divider bias to disabled
     HW32_ARRAY_OVERRIDE(0x405C,1),
     // Synth: Set divider bias to disabled (specific for loDivider=10)
     (uint32_t)0x18000280,
     // override_phy_rx_aaf_bw_0xd.xml
     // Rx: Set anti-aliasing filter bandwidth to 0xD (in ADI0, set IFAMPCTL3[7:4]=0xD)
     ADI_HALFREG_OVERRIDE(0,61,0xF,0xD),
     // override_phy_gfsk_rx.xml
     // Rx: Set LNA bias current trim offset to 3
     (uint32_t)0x00038883,
     // Rx: Freeze RSSI on sync found event
     HW_REG_OVERRIDE(0x6084,0x35F1),
     // override_phy_gfsk_pa_ramp_agc_reflevel_0x1a.xml
     // Tx: Configure PA ramping setting (0x41). Rx: Set AGC reference level to 0x1A.
     HW_REG_OVERRIDE(0x6088,0x411A),
     // Tx: Configure PA ramping setting
     HW_REG_OVERRIDE(0x608C,0x8213),
     // override_phy_rx_rssi_offset_neg2db.xml
     // Rx: Set RSSI offset to adjust reported RSSI by -2 dB
     (uint32_t)0x000288A3,
     // TX power override
     // Tx: Set PA trim to max (in ADI0, set PACTL0=0xF8)
     ADI_REG_OVERRIDE(0,12,0xF8),
     // Enabling syncword detection
     (uint32_t)0x008F88B3,
#ifdef RF_PACKET_TIME_COMPENSATE_RAT_TICKS
     // Compensate adjustRatTicks (SW_REG_OVERRIDE(CMD_RX, timestamp, RF_PACKET_TIME_COMPENSATE_RAT_TICKS))
     (uint32_t)((RF_PACKET_TIME_COMPENSATE_RAT_TICKS << 16) | 0x02A3), // 6 bytes
#endif
     //Configure new CRC16 polynom
     HW32_ARRAY_OVERRIDE(0x2004, 1),
     // The CRC16 polynome: CRC-16-CCITT normal form, 0x1021 is x^16 + x^15 + x^5 + 1
     0x10210000,
     // Configure new CRC init value
     0xC0040051,
     // The new init value
     0x00000000,
     // Set the SYSGPOCTL register
     HW_REG_OVERRIDE(0x1110, RFC_DBELL_SYSGPOCTL_GPOCTL2_RATGPO1 | RFC_DBELL_SYSGPOCTL_GPOCTL3_MCEGPO0),
#ifdef GPRAM_AS_RAM
     // Use GPRAM as RAM
     (uint32_t)0x00018063,
#endif
     (uint32_t)0xFFFFFFFF,
};

uint32_t pOverrides868[] =
{
 // override_use_patch_prop_genfsk.xml
     // PHY: Use MCE ROM bank 4, RFE RAM patch
     MCE_RFE_OVERRIDE(0,4,0,1,0,0),
     // override_synth_prop_863_930_div5.xml
     // Synth: Set recommended RTRIM to 7
     HW_REG_OVERRIDE(0x4038,0x0037),
     // Synth: Set Fref to 4 MHz
     (uint32_t)0x000684A3,
     // Synth: Configure fine calibration setting
     HW_REG_OVERRIDE(0x4020,0x7F00),
     // Synth: Configure fine calibration setting
     HW_REG_OVERRIDE(0x4064,0x0040),
     // Synth: Configure fine calibration setting
     (uint32_t)0xB1070503,
     // Synth: Configure fine calibration setting
     (uint32_t)0x05330523,
     // Synth: Set loop bandwidth after lock to 20 kHz
     (uint32_t)0x0A480583,
     // Synth: Set loop bandwidth after lock to 20 kHz
     (uint32_t)0x7AB80603,
     // Synth: Configure VCO LDO (in ADI1, set VCOLDOCFG=0x9F to use voltage input reference)
     ADI_REG_OVERRIDE(1,4,0x9F),
     // Synth: Configure synth LDO (in ADI1, set SLDOCTL0.COMP_CAP=1)
     ADI_HALFREG_OVERRIDE(1,7,0x4,0x4),
     // Synth: Use 24 MHz XOSC as synth clock, enable extra PLL filtering
     (uint32_t)0x02010403,
     // Synth: Configure extra PLL filtering
     (uint32_t)0x00108463,
     // Synth: Increase synth programming timeout (0x04B0 RAT ticks = 300 us)
     (uint32_t)0x04B00243,
     // override_phy_rx_aaf_bw_0xd.xml
     // Rx: Set anti-aliasing filter bandwidth to 0xD (in ADI0, set IFAMPCTL3[7:4]=0xD)
     //ADI_HALFREG_OVERRIDE(0,61,0xF,0xD),
     // override_phy_gfsk_rx.xml
     // Rx: Set LNA bias current trim offset to 3
     (uint32_t)0x00FF038883,
     // Rx: Freeze RSSI on sync found event
     HW_REG_OVERRIDE(0x6084,0x35F1),
     // override_phy_gfsk_pa_ramp_agc_reflevel_0x1a.xml
     // Tx: Configure PA ramping setting (0x41). Rx: Set AGC reference level to 0x1A.
     HW_REG_OVERRIDE(0x6088,0x411A),
     // Tx: Configure PA ramping setting
     HW_REG_OVERRIDE(0x608C,0x8213),
#if ((defined(Board_PIN_LNA_EN) && Board_PIN_LNA_EN != IOID_UNUSED) && \
     (defined(Board_PIN_PA_EN) && Board_PIN_PA_EN != IOID_UNUSED) && \
     (defined(Board_PIN_HGM) && Board_PIN_HGM != IOID_UNUSED))
     // Support Power Amplifier
     // override_phy_rx_rssi_offset_cc1310_cc1190_869.xml
     // Rx: Set RSSI offset to adjust reported RSSI by +26 dB
     (uint32_t)0x000188A3,
#else
     // override_phy_rx_rssi_offset_5db.xml
     // Rx: Set RSSI offset to adjust reported RSSI by +5 dB
     (uint32_t)0x00FB88A3,
     // TX power override
     // Tx: Set PA trim to max (in ADI0, set PACTL0=0xF8)
     ADI_REG_OVERRIDE(0,12,0xF8),
#endif
     //Enabling syncword detection
     (uint32_t)0x008F88B3,
#ifdef RF_PACKET_TIME_COMPENSATE_RAT_TICKS
     // Compensate adjustRatTicks
     (uint32_t)((RF_PACKET_TIME_COMPENSATE_RAT_TICKS << 16) | 0x02A3),
#endif
      //Configure new CRC16 polynom
     HW32_ARRAY_OVERRIDE(0x2004, 1),
     // The CRC16 polynome: CRC-16-CCITT normal form, 0x1021 is x^16 + x^15 + x^5 + 1
     0x10210000,
     // Configure new CRC init value
     0xC0040051,
     // The new init value
     0x00000000,
     // Set the SYSGPOCTL register
     HW_REG_OVERRIDE(0x1110, RFC_DBELL_SYSGPOCTL_GPOCTL2_RATGPO1 | RFC_DBELL_SYSGPOCTL_GPOCTL3_MCEGPO0),
     //HW_REG_OVERRIDE(0x1110, RFC_DBELL_SYSGPOCTL_GPOCTL2_RATGPO1 | RFC_DBELL_SYSGPOCTL_GPOCTL3_MCEGPO0 | RFC_DBELL_SYSGPOCTL_GPOCTL0_RFEGPO0 | RFC_DBELL_SYSGPOCTL_GPOCTL1_RFEGPO1),
#ifdef GPRAM_AS_RAM
     // Use GPRAM as RAM
     (uint32_t)0x00018063,
#endif
     (uint32_t)0xFFFFFFFF,
};

// CMD_PROP_RADIO_DIV_SETUP
rfc_CMD_PROP_RADIO_DIV_SETUP_t RF_cmdPropRadioDivSetup =
{
     .commandNo = 0x3807,
     .status = 0x0000,
     .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)
     .startTime = 0x00000000,
     .startTrigger.triggerType = 0x0,
     .startTrigger.bEnaCmd = 0x0,
     .startTrigger.triggerNo = 0x0,
     .startTrigger.pastTrig = 0x0,
     .condition.rule = 0x1,
     .condition.nSkip = 0x0,
     .modulation.modType = 0x1,
     .modulation.deviation = 0x50,
     .symbolRate.preScale = 0xF,
     .symbolRate.rateWord = 0x1893,
     .rxBw = 0x24,
     .preamConf.nPreamBytes = RF_PREAMBLE_BYTES,
     .preamConf.preamMode = 0x1,
     .formatConf.nSwBits = 0x10,
     .formatConf.bBitReversal = 0x0,
     .formatConf.bMsbFirst = 0x1,
     .formatConf.fecMode = 0x0,
     .formatConf.whitenMode = 0x0,
#ifdef FRONT_END_MODE
    .config.frontEndMode = 0x1,
#else
    .config.frontEndMode = 0x0,
#endif
#ifdef NOCOIL
    .config.biasMode = BIAS_MODE_USE_INTERNAL_COIL,
#else
    .config.biasMode = BIAS_MODE_USE_EXTERNAL_COIL,
#endif
     .config.analogCfgMode = 0x0,
     .config.bNoFsPowerUp = 0x0,
     .txPower = 0x46CB,
     .pRegOverride = pOverrides,
     .centerFreq = 0x01B1,
     .intFreq = 0x8000,
     .loDivider = 0x0A,
};

// CMD_FS
rfc_CMD_FS_t RF_cmdFs =
{
     .commandNo = 0x0803,
     .status = 0x0000,
     .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)
     .startTime = 0x00000000,
     .startTrigger.triggerType = 0x0,
     .startTrigger.bEnaCmd = 0x0,
     .startTrigger.triggerNo = 0x0,
     .startTrigger.pastTrig = 0x0,
     .condition.rule = 0x0,
     .condition.nSkip = 0x0,
     .frequency = 0x01B1,
     .fractFreq = 0xEB4F,
     .synthConf.bTxMode = 0x0,
     .synthConf.refFreq = 0 ,
     .__dummy0 = 0x00,
     .__dummy1 = 0x00,
     .__dummy2 = 0x00,
     .__dummy3 = 0x0000,
};

rfc_CMD_PROP_RX_ADV_t cmdRx =
{
    .commandNo = 0x3804,
    .status = 0x0000,
    .pNextOp = (RF_Op*) &cmdRx,
    .startTime = 0x00000000,
    .startTrigger.triggerType = TRIG_NOW,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = COND_NEVER,
    .condition.nSkip = 0x0,
    .pktConf.bFsOff = 0x0,
    .pktConf.bRepeatOk = 0x0,
    .pktConf.bRepeatNok = 0x0,
    .pktConf.bUseCrc = 0x1,
    .pktConf.bCrcIncSw = 0x0,
    .pktConf.bCrcIncHdr = 0x0,
    .pktConf.endType = 0x0,
    .pktConf.filterOp = 0x0,
    .rxConf.bAutoFlushIgnored = 0x0,
    .rxConf.bAutoFlushCrcErr = 0x1,
    .rxConf.bIncludeHdr = 0x1,
    .rxConf.bIncludeCrc = 0x1,
    .rxConf.bAppendRssi = 0x1,
    .rxConf.bAppendTimestamp = 0x1,
    .rxConf.bAppendStatus = 0x0,
    .syncWord0 = 0x0000,
    .syncWord1 = 0x0000,
    .maxPktLen = 127,
    .hdrConf.numHdrBits = 8,
    .hdrConf.lenPos = 0x0,
    .hdrConf.numLenBits = 8,
    .addrConf.addrType = 0x0,
    .addrConf.addrSize = 0,
    .addrConf.addrPos = 0,
    .addrConf.numAddr = 0,
    .lenOffset = 0,
    .endTrigger.triggerType = TRIG_ABSTIME,
    .endTrigger.bEnaCmd = 0x0,
    .endTrigger.triggerNo = 0x0,
    .endTrigger.pastTrig = 0x0,
    .endTime = 0x00000000,
};

rfc_CMD_PROP_TX_ADV_t cmdTx =
{
     .commandNo = 0x3803,
     .status = 0x0000,
     .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)
     .startTime = 0x00000000,
     .startTrigger.triggerType = TRIG_NOW,
     .startTrigger.bEnaCmd = 0x0,
     .startTrigger.triggerNo = 0x0,
     .startTrigger.pastTrig = 0x1,
     .condition.rule = TRIG_NEVER,
     .condition.nSkip = 0x0,
     .pktConf.bFsOff = 0x0,
     .pktConf.bUseCrc = 0x1,
     .pktConf.bCrcIncSw = 0x0,
     .pktConf.bCrcIncHdr = 0x0,
     .numHdrBits = 0x08,
     .pktLen = 0,
     .startConf.bExtTxTrig = 0x0,
     .startConf.inputMode = 0x0,
     .startConf.source = 0x0,
     .preTrigger.triggerType = TRIG_NOW,
     .preTrigger.bEnaCmd = 0x0,
     .preTrigger.triggerNo = 0x0,
     .preTrigger.pastTrig = 0x0,
     .preTime = 0,
     .syncWord = 0x00000000,
};

// CMD_NOP
rfc_CMD_NOP_t cmdNop =
{
    .commandNo                = CMD_NOP,
    .status                   = 0x0000,
    .pNextOp                  = 0, // Set this to (uint8_t*)&RF_cmdPropCs in the application
    .startTime                = 0x00000000,
    .startTrigger.triggerType = TRIG_ABSTIME, // Trigs at an absolute time
    .startTrigger.bEnaCmd     = 0x0,
    .startTrigger.triggerNo   = 0x0,
    .startTrigger.pastTrig    = 0x0,
    .condition.rule           = COND_ALWAYS, // Always run next command (except in case of Abort)
    .condition.nSkip          = 0x0,
};

// CMD_COUNT_BRANCH
rfc_CMD_COUNT_BRANCH_t cmdCountBranch =
{
    .commandNo                = CMD_COUNT_BRANCH,
    .status                   = 0x0000,
    .pNextOp                  = 0, // Set this to (uint8_t*)&RF_cmdPropTx in the application
    .startTime                = 0x00000000,
    .startTrigger.triggerType = TRIG_NOW, // Triggers immediately
    .startTrigger.bEnaCmd     = 0x0,
    .startTrigger.triggerNo   = 0x0,
    .startTrigger.pastTrig    = 0x0,
    .condition.rule           = COND_STOP_ON_FALSE, // Run next command if this command returned TRUE, stop if it returned FALSE
                                                    // End causes for the CMD_COUNT_BRANCH command:
                                                    // Finished operation with counter = 0 when being started: DONE_OK         TRUE
                                                    // Finished operation with counter > 0 after decrementing: DONE_OK         TRUE
                                                    // Finished operation with counter = 0 after decrementing: DONE_COUNTDOWN  FALSE
    .condition.nSkip          = 0x0,
    .counter                  = 0, // On start, the radio CPU decrements the value, and the end status of the operation differs if the result is zero
                                   // This number is set in the application (CS_RETRIES_WHEN_BUSY) and determines how many times the CMD_PROP_CS should run
                                   // in the case where the channel i Busy
    .pNextOpIfOk              = 0, // Set this to (uint8_t*)&RF_cmdPropCs in the application
};

rfc_CMD_PROP_CS_t cmdPropCs={
    .commandNo                = CMD_PROP_CS,
    .status                   = 0x0000,
    .pNextOp                  = 0, // Set this to (uint8_t*)&RF_cmdCountBranch in the application
    .startTime                = 0x00000000,
    .startTrigger.triggerType = TRIG_NOW,
    .startTrigger.bEnaCmd     = 0x0,
    .startTrigger.triggerNo   = 0x0,
    .startTrigger.pastTrig    = 0x0,
    .condition.rule           = COND_SKIP_ON_FALSE, // Run next command if this command returned TRUE,
                                                  // skip a number of commands (.condition.nSkip - 1) if it returned FALSE
                                                  // End causes for the CMD_PROP_CS command:
                                                  // Observed channel state Busy with csConf.busyOp = 1:                            PROP_DONE_BUSY        TRUE
                                                  // 0bserved channel state Idle with csConf.idleOp = 1:                            PROP_DONE_IDLE        FALSE
                                                  // Timeout trigger observed with channel state Busy:                              PROP_DONE_BUSY        TRUE
                                                  // Timeout trigger observed with channel state Idle:                              PROP_DONE_IDLE        FALSE
                                                  // Timeout trigger observed with channel state Invalid and csConf.timeoutRes = 0: PROP_DONE_BUSYTIMEOUT TRUE
                                                  // Timeout trigger observed with channel state Invalid and csConf.timeoutRes = 1: PROP_DONE_IDLETIMEOUT FALSE
                                                  // Received CMD_STOP after command started:                                       PROP_DONE_STOPPED     FALSE
    .condition.nSkip          = 0x2, // Number of skips + 1 if the rule involves skipping. 0: Same, 1: Next, 2: Skip next
    .csFsConf.bFsOffIdle      = 0x0, // Keep synthesizer running if command ends with channel Idle
    .csFsConf.bFsOffBusy      = 0x0, // Keep synthesizer running if command ends with Busy
    .__dummy0                 = 0x00,
    .csConf.bEnaRssi          = 0x1, // Enable RSSI as a criterion
    .csConf.bEnaCorr          = 0x0, // Disable correlation (PQT) as a criterion
    .csConf.operation         = 0x0, // Busy if either RSSI or correlation indicates Busy
    .csConf.busyOp            = 0x1, // End carrier sense on channel Busy
    .csConf.idleOp            = 0x0, // Continue on channel Idle
    .csConf.timeoutRes        = 0x0, // Timeout with channel state Invalid treated as Busy
    .rssiThr                  = 0x0, // Set the RSSI threshold in the application
    .numRssiIdle              = 0x0, // Number of consecutive RSSI measurements - 1 below the threshold
                                   // needed before the channel is declared Idle
    .numRssiBusy              = 0x0, // Number of consecutive RSSI measurements -1 above the threshold
                                   // needed before the channel is declared Busy
    .corrPeriod               = 0x0000, // N/A since .csConf.bEnaCorr = 0
    .corrConfig.numCorrInv    = 0x0, // N/A since .csConf.bEnaCorr = 0
    .corrConfig.numCorrBusy   = 0x0, // N/A since .csConf.bEnaCorr = 0
    .csEndTrigger.triggerType = TRIG_REL_START, // Trigs at a time relative to the command started
    .csEndTrigger.bEnaCmd     = 0x0,
    .csEndTrigger.triggerNo   = 0x0,
    .csEndTrigger.pastTrig    = 0x0,
    .csEndTime                = 0x00000000, // Set the CS end time in the application
};

volatile rfc_CMD_RX_TEST_t RF_cmdRxTest = {
    .commandNo = CMD_RX_TEST,
    .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .status = 0x0000,
    .startTime = 0x00000000,
    .startTrigger.triggerType = TRIG_NOW,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x1,
    .condition.rule = COND_NEVER,
    .condition.nSkip = 0x0,
    .config.bEnaFifo = 0x0,
    .config.bFsOff = 0x0,
    .config.bNoSync = 0x0,
    .endTrigger.triggerType = TRIG_REL_START,
    .endTrigger.bEnaCmd = 0x0,
    .endTrigger.triggerNo = 0x0,
    .endTrigger.pastTrig = 0x0,
    .syncWord = 0xAAAA,
    .endTime = 15000,
};

rfc_CMD_TX_TEST_t RF_cmdTxTest={
    .commandNo = CMD_TX_TEST,
    .status = 0x0000,
    .startTime = 0x00000000,
    .startTrigger.triggerType = TRIG_NOW,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .config.bUseCw = 0x0,
    .config.bFsOff = 0x0,
    .config.whitenMode = 0x0,
    .__dummy0 = 0x00,
    .txWord = 0xAAAA,
    .__dummy1 = 0x00,
    .endTrigger.triggerType = TRIG_NEVER,
    .endTrigger.bEnaCmd = 0x0,
    .endTrigger.triggerNo = 0x0,
    .endTrigger.pastTrig = 0x1,
    .syncWord = 0x0,
    .endTime = 1000,
};

// CMD_PROP_RX_ADV_SNIFF
// Proprietary Mode Advanced Receive Command with Sniff Mode
rfc_CMD_PROP_RX_ADV_SNIFF_t RF_cmdPropRxAdvSniff =
{
    .commandNo = CMD_PROP_RX_ADV_SNIFF,
    .status = 0x0000,
    .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .startTime = 0x00000000,
    .startTrigger.triggerType = TRIG_NOW,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x1,
    .condition.rule = COND_NEVER,
    .condition.nSkip = 0x0,
    .pktConf.bFsOff = 0x0,
    .pktConf.bRepeatOk = 0x0,
    .pktConf.bRepeatNok = 0x0,
    .pktConf.bUseCrc = 0x1,
    .pktConf.bCrcIncSw = 0x0,
    .pktConf.bCrcIncHdr = 0x0,
    .pktConf.endType = 0x0,
    .pktConf.filterOp = 0x0,
    .rxConf.bAutoFlushIgnored = 0x0,
    .rxConf.bAutoFlushCrcErr = 0x1,
    .rxConf.bIncludeHdr = 0x1,
    .rxConf.bIncludeCrc = 0x1,
    .rxConf.bAppendRssi = 0x1,
    .rxConf.bAppendTimestamp = 0x1,
    .rxConf.bAppendStatus = 0x0,
    .syncWord0 = 0x00000000,
    .syncWord1 = 0x00000000,
    .maxPktLen = 0x0000,
    .hdrConf.numHdrBits = 0x8,
    .hdrConf.lenPos = 0x0,
    .hdrConf.numLenBits = 0x8,
    .addrConf.addrType = 0x0,
    .addrConf.addrSize = 0x0,
    .addrConf.addrPos = 0x0,
    .addrConf.numAddr = 0x0,
    .lenOffset = 0x00,
    .endTrigger.triggerType = TRIG_REL_START,
    .endTrigger.bEnaCmd = 0x0,
    .endTrigger.triggerNo = 0x0,
    .endTrigger.pastTrig = 0x0,
    .endTime = 0x193A5,//0x29700,//0x00000000,
    .pAddr = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .pQueue = 0, // INSERT APPLICABLE POINTER: (dataQueue_t*)&xxx
    .pOutput = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .csConf.bEnaRssi = 0x1,
    .csConf.bEnaCorr = 0x1,
    .csConf.operation = 0x1,
    .csConf.busyOp = 0x0,
    .csConf.idleOp = 0x1,
    .csConf.timeoutRes = 0x1,
    .rssiThr = 0x00,
    .numRssiIdle = 0x01,
    .numRssiBusy = 0x01,
    .corrPeriod = 0x3400,//0x0000,
    .corrConfig.numCorrInv = 0x1,
    .corrConfig.numCorrBusy = 0x1,
    .csEndTrigger.triggerType = TRIG_REL_START,
    .csEndTrigger.bEnaCmd = 0x0,
    .csEndTrigger.triggerNo = 0x0,
    .csEndTrigger.pastTrig = 0x1,
    .csEndTime = 0x3318//0x00000000
};
