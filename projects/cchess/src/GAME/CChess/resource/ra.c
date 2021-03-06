
/*******************************************************************************
 * image
 * filename: unsaved
 * name: ra
 *
 * preset name: Color R5G6B5
 * data block size: 16 bit(s), uint16_t
 * RLE compression enabled: no
 * conversion type: Color, not_used not_used
 * bits per pixel: 16
 *
 * preprocess:
 *  main scan direction: top_to_bottom
 *  line scan direction: forward
 *  inverse: no
 *******************************************************************************/

/*
 typedef struct {
     const uint16_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
     } tImage;
*/
#include "lcd.h"

static const uint16_t image_data_ra[576] = {
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xf7be, 0xd658, 0xcd52, 0xd50f, 0xed0e,
    0xd4ce, 0xb4af, 0xbd53, 0xdefb, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xce16, 0xed2d, 0xfd09, 0xfd09, 0xfcc9, 0xfc88, 0xf448, 0xe3c6, 0xcb86,
    0xbb66, 0xac0c, 0xd679, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xef3c, 0xf5b0, 0xfd4b, 0xfd8d, 0xfdcf,
    0xf5b1, 0xed70, 0xe52f, 0xed4f, 0xe52e, 0xe48b, 0xcb66, 0xaac4, 0x9a84,
    0xacf2, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xef3c, 0xed8f, 0xfdce, 0xfe73, 0xf5d1, 0xe570, 0xe591, 0xedb2, 0xf590,
    0xed4f, 0xe50e, 0xd4ad, 0xdcac, 0xdc8b, 0xa2c4, 0x8a23, 0x9c4f, 0xf7be,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xf5b0, 0xfdee, 0xfe74,
    0xe54f, 0xe5b1, 0xf5f2, 0xf570, 0xed6f, 0xed4e, 0xed2f, 0xed2e, 0xe4ed,
    0xdcac, 0xcc4b, 0xe4cc, 0x9a84, 0x71c2, 0xad34, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xd5d5, 0xfd8c, 0xfe33, 0xed70, 0xedf3, 0xf5b1, 0xed4f,
    0xf570, 0xed6f, 0xed4e, 0xed2d, 0xe4ed, 0xed0d, 0xe4ed, 0xe4cc, 0xcc2a,
    0xdcab, 0x79e2, 0x6a04, 0xef5d, 0xffff, 0xffff, 0xffff, 0xef5d, 0xf4ec,
    0xfdcf, 0xed70, 0xe550, 0xed2f, 0xd3ea, 0xb9a3, 0xed0e, 0xe50d, 0xed0d,
    0xdc2b, 0xba44, 0xe4ed, 0xe4cc, 0xe48b, 0xdc6b, 0xc3e9, 0xbb87, 0x5941,
    0x93ee, 0xffff, 0xffff, 0xffff, 0xd616, 0xf48a, 0xf5b0, 0xdccd, 0xcb68,
    0xb121, 0xa8e0, 0xa921, 0xed70, 0xe4cd, 0xe4cd, 0xd3c9, 0xb182, 0xe50e,
    0xdc8b, 0xdc6b, 0xdc8b, 0xc3c8, 0xd42a, 0x71c2, 0x5983, 0xe73c, 0xffff,
    0xffff, 0xcd10, 0xfcca, 0xed2f, 0xdcee, 0xb161, 0xa8e0, 0xa900, 0xe50f,
    0xe50d, 0xdc6b, 0xd388, 0xb9c3, 0xa900, 0xc2c6, 0xcb47, 0xc2c6, 0xba65,
    0xcb47, 0xbbc8, 0x92c5, 0x5121, 0xb595, 0xffff, 0xffff, 0xdced, 0xec6a,
    0xdd0e, 0xe50e, 0xdc8c, 0xba45, 0xb9e3, 0xe48d, 0xb141, 0xa900, 0xa8e0,
    0xa8e0, 0xa8e0, 0xa8e0, 0xa8e0, 0xa8e0, 0xb141, 0xe50e, 0xbbc8, 0xbb87,
    0x5941, 0x9c71, 0xffdf, 0xffff, 0xd44a, 0xec6a, 0xe50e, 0xed4f, 0xed4f,
    0xe4cd, 0xba04, 0xed4f, 0xb1a2, 0xc2e7, 0xdc4c, 0xc308, 0xb141, 0xe4ee,
    0xcb69, 0xc2c6, 0xdc6c, 0xe4cc, 0xc3a8, 0xc3e8, 0x5940, 0x940e, 0xf79e,
    0xffff, 0xc40a, 0xdc08, 0xdd0e, 0xe50e, 0xed4f, 0xe4cd, 0xb1a2, 0xed70,
    0xed0e, 0xed4f, 0xe4ed, 0xc306, 0xba44, 0xe50e, 0xdcac, 0xdcac, 0xdcac,
    0xdc8b, 0xbbc8, 0xc3e9, 0x6141, 0x940e, 0xf7be, 0xffff, 0xc46c, 0xc366,
    0xdced, 0xdccd, 0xe4cd, 0xdc8c, 0xb9c3, 0xed4f, 0xe4ac, 0xdcac, 0xdc6b,
    0xb9e3, 0xb1e3, 0xdc6b, 0xdc6b, 0xdc6b, 0xd42a, 0xdc4a, 0xbba8, 0xbb87,
    0x6982, 0xacb1, 0xf7be, 0xffff, 0xb4b0, 0xb2c5, 0xdccd, 0xcc8c, 0xe4cd,
    0xd40a, 0xba25, 0xe4ee, 0xc2a5, 0xba03, 0xa900, 0xa8e0, 0xa8e0, 0xa8e0,
    0xb121, 0xb9e3, 0xd42a, 0xd409, 0xc3e9, 0xa2e5, 0x69c2, 0xbdb6, 0xffdf,
    0xffff, 0xc5f7, 0x9aa4, 0xc3a8, 0xd48b, 0xdcac, 0xd3c9, 0xba25, 0xe4cc,
    0xa900, 0xb121, 0xb9e3, 0xba45, 0xba25, 0xb9c3, 0xa921, 0xa8e0, 0xd40a,
    0xc3c8, 0xdc8b, 0x8223, 0x7a45, 0xdedb, 0xffff, 0xffff, 0xf79e, 0x9328,
    0x9263, 0xdc8c, 0xc409, 0xd44a, 0xc286, 0xdccd, 0xdcac, 0xe550, 0xdced,
    0xdcac, 0xd46b, 0xdcac, 0xe4ed, 0xe50e, 0xdcac, 0xc42a, 0xbb67, 0x71c2,
    0x9c2e, 0xf79e, 0xffff, 0xffff, 0xffff, 0xbd95, 0x7a23, 0xa2e6, 0xdcac,
    0xc40a, 0xdccd, 0xdc8c, 0xdc6b, 0xdc6b, 0xdc6b, 0xdc6a, 0xd42a, 0xd42a,
    0xd42a, 0xd409, 0xbbe9, 0xcc09, 0x71e2, 0x7a65, 0xd69a, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffdf, 0x93cd, 0x69c3, 0xa306, 0xdcac, 0xbbea, 0xd429,
    0xdc8b, 0xdc8b, 0xdc6b, 0xd44a, 0xd44a, 0xd42a, 0xc3c8, 0xbc2a, 0xcc09,
    0x71c2, 0x71e3, 0xb554, 0xf7be, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xe73c, 0x836b, 0x5961, 0x7a03, 0xd46a, 0xd46b, 0xbc0a, 0xc409, 0xc3c9,
    0xc3e9, 0xc40a, 0xc42b, 0xdc8b, 0xb367, 0x6961, 0x69c3, 0xa4b1, 0xef7d,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xe71c, 0x940e,
    0x5983, 0x5121, 0x7a24, 0xbba8, 0xcc2a, 0xcc6b, 0xcc4b, 0xc3e9, 0xa306,
    0x71c2, 0x5961, 0x7225, 0xb533, 0xef7d, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xf79e, 0xc5d7, 0x832a, 0x59a3,
    0x5121, 0x5121, 0x5121, 0x5101, 0x5121, 0x5981, 0x6a04, 0x93ed, 0xd699,
    0xf7be, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xef5d, 0xd679, 0xad13, 0x93cd, 0x834b,
    0x836b, 0x9c2f, 0xb574, 0xdeba, 0xf79e, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffdf, 0xf7be, 0xf79e, 0xf79e, 0xf7be, 0xffdf,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff};
const tImage LCD_RES_RA = {image_data_ra, 24, 24, 16};
