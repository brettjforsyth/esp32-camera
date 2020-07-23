/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * OV2640 register definitions.
 */
#ifndef __REG_REGS_H__
#define __REG_REGS_H__
#define GAIN                    0x00 /* AGC – Gain control gain setting  */
#define BLUE                    0x01 /* AWB – Blue channel gain setting  */
#define RED                     0x02 /* AWB – Red channel gain setting   */
#define GREEN                   0x03 /* AWB – Green channel gain setting */
#define BAVG                    0x05 /* U/B Average Level   */
#define GAVG                    0x06 /* Y/Gb Average Level  */
#define RAVG                    0x07 /* V/R Average Level   */

/* -------- PIDH : (Address: 0x0a) Product ID number MSB -------- */
#define REG_PID                    (0x0a)
#define PIDH_DEFAULT            (0x77 << 0)

/* -------- PIDL : (Address: 0x0b) Product ID number LSB -------- */
#define REG_VER                    (0x0b)
#define PIDL_DEFAULT            (0x40 << 0)

/* -------- REG0C : (Address: 0x0b) -------- */
#define REG0C                   (0x0c)
#define REG0C_MAX_EXPOSURE_Pos    (1)
#define REG0C_MAX_EXPOSURE_Msk    (0x3 << REG0C_MAX_EXPOSURE_Pos) /**< \brief (REG0C) Max exposure = frame length - limit x 2 */
#define REG0C_MAX_EXPOSURE(value) ((REG0C_MAX_EXPOSURE_Msk & ((value) << REG0C_MAX_EXPOSURE_Pos)))
#define REG0C_BYTE_SWAP_Msk       (0x1 << 3) /**< \brief (REG0C) High 8-bit MSB and LSB swap */
#define REG0C_BYTE_SWAP_DISABLE   (0x0 << 3) /**< \brief (REG0C) output Y9,Y8...Y3,Y2,Y1,Y0 */
#define REG0C_BYTE_SWAP_ENABLE    (0x1 << 3) /**< \brief (REG0C) output Y3,Y2...Y8,Y9,Y1,Y0 */
#define REG0C_YUV_SWAP_Msk        (0x1 << 4) /**< \brief (REG0C) YUV output, Y <-> UV swap */
#define REG0C_YUV_SWAP_DISABLE    (0x0 << 4) /**< \brief (REG0C) output YUYVYUYV */
#define REG0C_YUV_SWAP_ENABLE     (0x1 << 4) /**< \brief (REG0C) output UYVYUYVY */
#define REG0C_MIRROR_ENABLE       (0x1 << 6) /**< \brief (REG0C) Mirror enable */
#define REG0C_FLIP_ENABLE         (0x1 << 7) /**< \brief (REG0C) Flip enable */

/* -------- REG0D : (Address: 0x0d) Analog setting -------- */
#define REG0D                   (0x0d)

/* -------- REG0E : (Address: 0x0e) Analog setting -------- */
/* default value: REG0E_BLC_BOTH|REG0E_BLC_OPTICAL */
#define REG0E                   (0x0e)
#define REG0E_OUTPUT_Pos          (0)
#define REG0E_OUTPUT_Msk          (0x3 << REG0E_OUTPUT_Pos) /**< \brief (REG0E) Output driving capability */
#define   REG0E_OUTPUT_1X         (0x0 << REG0E_OUTPUT_Pos) /**< \brief (REG0E) 1x */
#define   REG0E_OUTPUT_2X         (0x1 << REG0E_OUTPUT_Pos) /**< \brief (REG0E) 2x */
#define   REG0E_OUTPUT_3X         (0x2 << REG0E_OUTPUT_Pos) /**< \brief (REG0E) 3x */
#define   REG0E_OUTPUT_4X         (0x3 << REG0E_OUTPUT_Pos) /**< \brief (REG0E) 4x */
#define REG0E_SLEEP_MODE          (0x1 << 3) /**< \brief (REG0E) Sleep mode */
#define REG0E_BLC_Pos             (5)
#define REG0E_BLC_Msk             (0x3 << REG0E_BLC_Pos) /**< \brief (REG0E) BLC line selection */
#define REG0E_BLC_BOTH0           (0x0 << REG0E_BLC_Pos) /**< \brief (REG0E) Select both blue line and red line as BLC line. */
#define REG0E_BLC_RED             (0x1 << REG0E_BLC_Pos) /**< \brief (REG0E) Select red line as BLC line. */
#define REG0E_BLC_BLUE            (0x2 << REG0E_BLC_Pos) /**< \brief (REG0E) Select blue line as BLC line. */
#define REG0E_BLC_BOTH            (0x3 << REG0E_BLC_Pos) /**< \brief (REG0E) Select both blue line and red line as BLC line. */
#define REG0E_BLC_LINE_Msk        (0x1 << 7) /**< \brief (REG0E) BLC line selection */
#define REG0E_BLC_LINE_ELECTRICAL (0x0 << 7) /**< \brief (REG0E) Electrical BLC */
#define REG0E_BLC_LINE_OPTICAL    (0x1 << 7) /**< \brief (REG0E) Optical BLC */

/* -------- HAEC : (Address: 0x0f) Automatic exposure control bit [15:8] -------- */
#define HAEC                    (0x0f)

/* -------- AEC : (Address: 0x10) Automatic exposure control bit [7:0] -------- */
#define AEC                     (0x10)

/* -------- CLK : (Address: 0x11) Clock settings -------- */
#define CLK                     (0x11) /**< \brief (CLK) sysclk=XVCLK1 x PLLDIV / [(CLK[5:0]+1) x2 xPreDiv] */
#define CLK_DIVIDER_Pos           (0)
#define CLK_DIVIDER_Msk           (0x3f << CLK_DIVIDER_Pos) /**< \brief (CLK) Clock divider */
#define CLK_DIVIDER(value)        ((CLK_DIVIDER_Msk & ((value) << CLK_DIVIDER_Pos)))
#define CLK_PLL_Pos               (6)
#define CLK_PLL_Msk               (0x3 << CLK_PLL_Pos) /**< \brief (CLK) PLL setting - Changing this value is not recommanded */
#define CLK_PLL(value)            ((CLK_PLL_Msk & ((value) << CLK_PLL_Pos)))

/* -------- REG12 : (Address: 0x12) -------- */
#define REG12                   (0x12)
#define REG12_RAW_RGB             (0x1 << 0)
#define REG12_SENSOR_RAW          (0x1 << 4)
#define REG12_CC656_MODE          (0x1 << 5)
#define REG12_VSKIP               (0x1 << 6)
#define REG12_RESET               (0x1 << 7)

/* -------- REG13 : (Address: 0x13) -------- */
#define REG13                   (0x13)
#define REG13_EXPOSURE_Msk        (0x01 << 0) /**< \brief (REG13) Exposure auto/manual control selection */
#define REG13_EXPOSURE_MANUAL     (0x0 << 0)
#define REG13_EXPOSURE_AUTO       (0x1 << 0)
#define REG13_WBAL_Msk            (0x1 << 1) /**< \brief (REG13) Auto white balance control selection */
#define REG13_WBAL_MANUAL         (0x0 << 1)
#define REG13_WBAL_AUTO           (0x1 << 1)
#define REG13_AGC_Msk             (0x1 << 2) /**< \brief (REG13) AGC auto/manual control selection */
#define REG13_AGC_MANUAL          (0x0 << 2)
#define REG13_AGC_AUTO            (0x1 << 2)
#define REG13_LAEC_Msk            (0x1 << 3) /**< \brief (REG13) LAEC enable */
#define REG13_LAEC_DISABLE        (0x0 << 3)
#define REG13_LAEC_ENABLE         (0x1 << 3)
#define REG13_BANDING_OPT_Msk     (0x1 << 4) /**< \brief (REG13) Banding option */
#define REG13_BANDING_OPT_LIMITED (0x0 << 4) /**< \brief (REG13) Minimum exposure is limited to 1/120 or 1/100 second when banding filter is enabled */
#define REG13_BANDING_OPT_ENABLE  (0x1 << 4) /**< \brief (REG13) Minimum exposure is allowed to be less than 1/120 or 1/100 second when banding filter is enabled */
#define REG13_BANDING_Mask        (0x1 << 5) /**< \brief (REG13) Banding enable */
#define REG13_BANDING_DISABLE     (0x0 << 5)
#define REG13_BANDING_ENABLE      (0x1 << 5)
#define REG13_FRAME_DROP_Mask     (0x1 << 6) /**< \brief (REG13) Enable frame drop function */
#define REG13_FRAME_DROP_DISABLE  (0x0 << 6)
#define REG13_FRAME_DROP_ENABLE   (0x1 << 6)
#define REG13_AEC_Mask            (0x1 << 7) /**< \brief (REG13) AEC speed selection */
#define REG13_AEC_NORMAL          (0x0 << 7) /**< \brief (REG13) Normal */
#define REG13_AEC_FASTER          (0x1 << 7) /**< \brief (REG13) Faster AEC correction */

/* -------- REG14 : (Address: 0x14) -------- */
#define REG14                   (0x14)

/* -------- REG15 : (Address: 0x15) -------- */
#define REG15                   (0x15)
#define REG15_GAIN_Pos          (0)
#define REG15_GAIN_Msk          (0x3 << REG15_GAIN_Pos) /**< \brief (REG15) AGC MSBs (digital gain) (LSBs in GAIN[7:0]) */
#define REG15_GAIN(value)       ((REG15_GAIN_Msk & ((value) << REG15_GAIN_Pos)))
#define REG15_NIGHT_Mask        (0x3 << 2) /**< \brief (REG15) Night mode triggering point */
#define REG15_NIGHT_2X_GAIN     (0x0 << 2) /**< \brief (REG15) 2x gain */
#define REG15_NIGHT_4X_GAIN     (0x1 << 2) /**< \brief (REG15) 4x gain */
#define REG15_NIGHT_8X_GAIN     (0x2 << 2) /**< \brief (REG15) 8x gain */
#define REG15_NIGHT_16X_GAIN    (0x3 << 2) /**< \brief (REG15) 16x gain */
#define REG15_CEIL_Mask         (0x3 << 4) /**< \brief (REG15) Ceiling of inserting frames */
#define REG15_CEIL_0            (0x0 << 4) /**< \brief (REG15) Up to 0 frames */
#define REG15_CEIL_1            (0x1 << 4) /**< \brief (REG15) Up to 1 frames */
#define REG15_CEIL_2            (0x2 << 4) /**< \brief (REG15) Up to 2 frames */
#define REG15_CEIL_3            (0x3 << 4) /**< \brief (REG15) Up to 3 frames */
#define REG15_CEIL_7            (0x7 << 4) /**< \brief (REG15) Up to 7 frames */
#define REG15_ENABLE_NIGHT      (0x1 << 7)  /**< \brief (REG15) Enable inserting frames in night mode */

/* -------- REG16 : (Address: 0x16) -------- */
#define REG16                   (0x16)

/* -------- AHSTART : (Address: 0x17) Sensor Horizontal output start point 8 MSBs (LSBs in REG16[1:0])-------- */
#define AHSTART                 (0x17)

/* -------- AHSIZE : (Address: 0x18) Sensor Horizontal output size 8 MSBs (LSBs in REG16[4:3])-------- */
#define AHSIZE                  (0x18)

/* -------- AVSTART : (Address: 0x19) Sensor Vertical output start point 8 MSBs (LSBs in REG16[2])-------- */
#define AVSTART                 (0x19)

/* -------- AVSIZE : (Address: 0x1a) Sensor Vertical output size 8 MSBs (LSBs in REG16[5])-------- */
#define AVSIZE                  (0x1a)

/* -------- PIXEL_SHIFT : (Address: 0x1b) Pixel shift -------- */
#define PIXEL_SHIFT             (0x1b)

/* -------- MIDH : (Address: 0x1c) Manufacturer ID Byte - High -------- */
#define REG_MIDH                    (0x1c)
#define MIDH_DEFAULT              (0x7f << 0)

/* -------- MIDL : (Address: 0x1d) Manufacturer ID Byte - Low -------- */
#define REG_MIDL                    (0x1d)
#define MIDL_DEFAULT              (0xa2 << 0)

/* -------- REG1E : (Address: 0x1e) -------- */
#define REG1E                   (0x1e)

/* -------- REG1F : (Address: 0x1f) -------- */
#define REG1F                   (0x1f)

/* -------- REG1E : (Address: 0x1e) -------- */
#define REG1E                   (0x1e)

/* -------- REG20 : (Address: 0x20) -------- */
#define REG20                   (0x20)

/* -------- REG21 : (Address: 0x21) -------- */
#define REG21                   (0x21)

/* -------- REG21 : (Address: 0x24) Luminance signal high range for AEC/AGC operation -------- */
#define WPT                     (0x24)

/* -------- REG21 : (Address: 0x25) Luminance signal low range for AEC/AGC operation -------- */
#define BPT                     (0x25)

/* -------- VPT : (Address: 0x26) effective only in AEG/AGC fast mode -------- */
#define VPT                     (0x26)

/* -------- REG27 : (Address: 0x27) -------- */
#define REG27                   (0x27)
#define REG27_BLACKSUN            (0x1 << 7) /**< \brief (REG27) Black sun cancellation enable */

/* -------- REG28 : (Address: 0x28) -------- */
#define REG28                   (0x28)
#define REG28_VSYNC_Msk           (0x1 << 1) /**< \brief (REG28) VSYNC polarity */
#define REG28_VSYNC_POSITIVE      (0x1 << 0) /**< \brief (REG28) Positive */
#define REG28_VSYNC_NEGATIVE      (0x1 << 1) /**< \brief (REG28) Negative */
#define REG28_VSYNC_OUTPUT_Msk    (0x1 << 3) /**< \brief (REG28) No VSYNC output option */
#define REG28_VSYNC_OUTPUT_STILL  (0x0 << 3) /**< \brief (REG28) Still output VSYNC when frame drop */
#define REG28_VSYNC_OUTPUT_NONE   (0x1 << 3) /**< \brief (REG28) No VSYNC output when frame drop */
#define REG28_HREF_Msk            (0x1 << 4) /**< \brief (REG28) HREF polarity */
#define REG28_HREF_POSITIVE       (0x0 << 4) /**< \brief (REG28) Output positive HREF */
#define REG28_HREF_NEGATIVE       (0x1 << 4) /**< \brief (REG28) Output negative HREF for data valid */
#define REG28_HSYNC_Msk           (0x1 << 5) /**< \brief (REG28) HSYNC polarity */
#define REG28_HSYNC_POSITIVE      (0x0 << 5) /**< \brief (REG28) Positive */
#define REG28_HSYNC_NEGATIVE      (0x1 << 5) /**< \brief (REG28) Negative */
#define REG28_HREF_OUTPUT_Msk     (0x1 << 6) /**< \brief (REG28) HREF pin output swap */
#define REG28_HREF_OUTPUT_HREF    (0x0 << 6) /**< \brief (REG28) HREF */
#define REG28_HREF_OUTPUT_HSYNC   (0x1 << 6) /**< \brief (REG28) HSYNC */
#define REG28_OUTPUT_REVERSE      (0x1 << 7) /**< \brief (REG28) Output data bit reverse option */

/* -------- REG65 : (Address: 0x65) -------- */
#define REG65                  (0x65)
#define REG65_BIT_SWAP_Msk       (0x1 << 3) /**< \brief (REG65) Output data bit swap option */
#define REG65_BIT_SWAP_NORMAL    (0x0 << 3) /**< \brief (REG65) Output DATA[9:0] */
#define REG65_BIT_SWAP_REVERSE   (0x1 << 3) /**< \brief (REG65) Output DATA[0:9] */

/* -------- YUV422CTRL : (Address: 0xd9) -------- */
#define YUV422CTRL             (0xd9)

#define YUV422CTRL_CNV_OPT_Msk   (0x1 << 0) /**< \brief (YUV422CTRL) cnv_opt */
#define YUV422CTRL_CNV_OPT_AVERAGE (0x0 << 0) /**< \brief (YUV422CTRL) Average mode */
#define YUV422CTRL_CNV_OPT_DROP    (0x1 << 0) /**< \brief (YUV422CTRL) Drop mode */

#define YUV422CTRL_V_FIRST_Msk   (0x1 << 1) /**< \brief (YUV422CTRL) v_first */
#define YUV422CTRL_V_FIRST_YUYV  (0x0 << 1) /**< \brief (YUV422CTRL) Output line will be YUYV... */
#define YUV422CTRL_V_FIRST_YVYU  (0x1 << 1) /**< \brief (YUV422CTRL) Output line will be YVYU... (it will affect definition of U/V in SDE. If it is set, all registers in SDE about U/V must be swapped */
#define SET_REG(reg, x)         (##reg_DEFAULT|x)
#endif //__REG_REGS_H__