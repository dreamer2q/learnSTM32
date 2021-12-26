
/*******************************************************************************
 * image
 * filename: unsaved
 * name: rc
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

static const uint16_t image_data_rc[576] = {
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xef7d, 0xde99, 0xe615, 0xe5b3,
    0xedf3, 0xcd72, 0xcdf5, 0xe6fb, 0xf7be, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xef5c, 0xe614, 0xfd8e, 0xfd2a, 0xfcea, 0xfcc9, 0xf489, 0xec27, 0xd3c7,
    0xcbc8, 0xbc6d, 0xce37, 0xffdf, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xe699, 0xfdae, 0xfd8c, 0xfdae,
    0xf5d0, 0xed90, 0xed6f, 0xed2f, 0xed2e, 0xe4cc, 0xdc08, 0xb305, 0xa2e5,
    0xac4e, 0xe73c, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xde56, 0xfdce, 0xfe50, 0xf632, 0xe570, 0xe591, 0xedf1, 0xed90,
    0xed6f, 0xe52f, 0xdccd, 0xd48c, 0xe4ed, 0xbb66, 0x9284, 0x9bab, 0xdefb,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xde99, 0xfdce, 0xfe73,
    0xedb1, 0xdd70, 0xf5f2, 0xf5d0, 0xf58f, 0xed6f, 0xed2e, 0xed2e, 0xed0e,
    0xe4ed, 0xcc4b, 0xd48c, 0xc3c8, 0x7a02, 0x8b6b, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xef5c, 0xfdae, 0xfe10, 0xf5f2, 0xe591, 0xf5f2, 0xed90,
    0xf570, 0xf56f, 0xed6f, 0xd348, 0xe46d, 0xed2d, 0xe4ed, 0xe4ed, 0xd42a,
    0xdcac, 0xab26, 0x69a3, 0xad13, 0xffff, 0xffff, 0xffff, 0xffff, 0xddd4,
    0xfd4c, 0xf5f2, 0xdd0e, 0xc226, 0xb944, 0xb903, 0xc1c4, 0xe46c, 0xe4ed,
    0xb923, 0xd3ab, 0xdbea, 0xdbea, 0xdc2a, 0xdc4a, 0xc3e9, 0xd46a, 0x71c2,
    0x6a05, 0xdefb, 0xffff, 0xffff, 0xef7d, 0xeced, 0xfd6d, 0xe52f, 0xe570,
    0xb081, 0xa820, 0xa820, 0xb0a2, 0xe4ee, 0xcaa7, 0xa820, 0xb0a2, 0xc1c5,
    0xc1c5, 0xb0a2, 0xb984, 0xdc6b, 0xbba8, 0xa306, 0x5141, 0xad34, 0xffff,
    0xffff, 0xdeda, 0xec69, 0xf56e, 0xdd0e, 0xed2e, 0xb0a1, 0xd3ab, 0xed70,
    0xed4f, 0xd38a, 0xb040, 0xc288, 0xcb29, 0xc206, 0xd3ca, 0xc245, 0xe46d,
    0xe4ab, 0xbbc8, 0xcbe8, 0x5141, 0x834b, 0xf79e, 0xffff, 0xcdf6, 0xe428,
    0xed4e, 0xdccd, 0xd2e7, 0xa820, 0xb9a5, 0xb943, 0xb0a1, 0xd3eb, 0xb103,
    0xc1e6, 0xd3ab, 0xb0e3, 0xe52f, 0xb0a1, 0xe50e, 0xdc8a, 0xcc09, 0xd42a,
    0x6181, 0x7266, 0xe73c, 0xffff, 0xc532, 0xdc07, 0xf54f, 0xd4ac, 0xb0a2,
    0xa820, 0xb061, 0xb061, 0xb103, 0xed70, 0xb9e4, 0xc206, 0xb902, 0xcac9,
    0xdc2b, 0xc227, 0xe4ec, 0xdc8b, 0xd429, 0xcc09, 0x71e2, 0x6a04, 0xd6ba,
    0xffff, 0xbd32, 0xcba6, 0xe52e, 0xcc4c, 0xd34a, 0xb9a4, 0xe50f, 0xd38a,
    0xcb2a, 0xed2e, 0xb943, 0xc1e6, 0xd38b, 0xe54f, 0xc1c5, 0xdc8e, 0xdc8b,
    0xdc8b, 0xd429, 0xcc4a, 0x71e2, 0x6a04, 0xd69a, 0xffff, 0xc5b5, 0xbb26,
    0xdc6b, 0xd4ac, 0xe4ee, 0xb963, 0xc1c5, 0xb061, 0xe52f, 0xdc8c, 0xb0a2,
    0xdc8d, 0xcaa6, 0xb0e2, 0xc268, 0xe50e, 0xd44a, 0xd44a, 0xcbe9, 0xcc29,
    0x71c2, 0x7a66, 0xdedb, 0xffff, 0xd699, 0xab05, 0xcbc8, 0xd4ad, 0xe4cd,
    0xb923, 0xcb09, 0xe4ef, 0xe4ed, 0xdcac, 0xa840, 0xd3cb, 0xd40a, 0xdc2c,
    0xe4ed, 0xd46a, 0xd40a, 0xcb07, 0xbbe9, 0xcbe9, 0x71c2, 0x8b4a, 0xe73c,
    0xffff, 0xef7d, 0x9b48, 0xaac4, 0xdcac, 0xd46b, 0xd40b, 0xe52e, 0xe4cc,
    0xe4cc, 0xdcab, 0xb0c2, 0xb0a2, 0xa840, 0xb081, 0xb102, 0xc1e4, 0xb081,
    0xc288, 0xcc4a, 0xab46, 0x71e2, 0xacf2, 0xf7be, 0xffff, 0xffff, 0xacf2,
    0x8a64, 0xc3c8, 0xcc4b, 0xdc6b, 0xe4cc, 0xdcab, 0xdc8b, 0xdc8b, 0xdc6b,
    0xe52f, 0xdc8c, 0xc267, 0xb0a2, 0xa820, 0xb0e2, 0xccce, 0xd44a, 0x8223,
    0x7a04, 0xd69a, 0xffff, 0xffff, 0xffff, 0xef7d, 0x82c7, 0x8223, 0xdc6b,
    0xc42b, 0xd44a, 0xdc8b, 0xdc8b, 0xdc8b, 0xdc8b, 0xdc8b, 0xd44a, 0xd46b,
    0xdc8b, 0xdc8b, 0xcc8c, 0xd48b, 0x92a4, 0x69a2, 0xa491, 0xf79e, 0xffff,
    0xffff, 0xffff, 0xffff, 0xc5f7, 0x69e4, 0x79e3, 0xd46b, 0xc44b, 0xc3e9,
    0xd44a, 0xdc8b, 0xdc8b, 0xdc6a, 0xdc6a, 0xd46a, 0xcc09, 0xbbe9, 0xdc6b,
    0x9aa4, 0x6981, 0x8b4a, 0xe71c, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xad13, 0x61a3, 0x5961, 0xbb88, 0xdccb, 0xc42a, 0xc3e9, 0xc3e9,
    0xc3e9, 0xc40a, 0xc42b, 0xcc6b, 0xd449, 0x8223, 0x6161, 0x82c8, 0xd69a,
    0xffdf, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffdf, 0xb574,
    0x6a26, 0x5101, 0x6182, 0xa306, 0xc409, 0xcc6b, 0xcc4b, 0xcc2a, 0xb388,
    0x8243, 0x6161, 0x61a2, 0x93ac, 0xd6ba, 0xffdf, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xd6ba, 0x9c4f, 0x61e5,
    0x5122, 0x5101, 0x5101, 0x5101, 0x5121, 0x5141, 0x61a3, 0x82e8, 0xbd75,
    0xef5d, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xf79e, 0xdefb, 0xbdb5, 0x9c4f, 0x834b,
    0x834b, 0x8bad, 0xacf2, 0xce38, 0xef5d, 0xffdf, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffdf, 0xf7be, 0xf7be, 0xf79e, 0xf7be, 0xffdf,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff};
const tImage LCD_RES_RC = {image_data_rc, 24, 24, 16};