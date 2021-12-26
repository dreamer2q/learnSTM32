
/*******************************************************************************
 * image
 * filename: unsaved
 * name: bb
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

static const uint16_t image_data_bb[576] = {
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xf7be, 0xd658, 0xcd91, 0xd54c, 0xed69,
    0xdd09, 0xbccd, 0xbd73, 0xdefb, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xce36, 0xee0c, 0xfe29, 0xfe09, 0xfde8, 0xfda8, 0xfd66, 0xf4e4, 0xdc83,
    0xcc45, 0xb48b, 0xd699, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xef3c, 0xf670, 0xfe2b, 0xfe6c, 0xfe8f,
    0xf650, 0xf62e, 0xf60d, 0xee0e, 0xf5cc, 0xf568, 0xd464, 0xbba2, 0x9b43,
    0xb532, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xef3c, 0xee2f, 0xfe6e, 0xfed2, 0xf650, 0xee0f, 0xee30, 0xf671, 0xf64f,
    0xf62e, 0xedcc, 0xe56b, 0xed8b, 0xed49, 0xbba3, 0x92e2, 0xa48f, 0xf7be,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xf64f, 0xfe6e, 0xfef4,
    0xedee, 0xf630, 0xfe92, 0xfe50, 0xf62e, 0xfe0d, 0xfe0c, 0xfdec, 0xf5cb,
    0xed8a, 0xd509, 0xedaa, 0xab62, 0x7a41, 0xad34, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xd614, 0xfe4d, 0xfed3, 0xf5ee, 0xf672, 0xfe50, 0xf60d,
    0xeded, 0xc4aa, 0x72c7, 0x5225, 0xac49, 0xb449, 0xb469, 0xf5aa, 0xdce9,
    0xed69, 0x8a81, 0x7244, 0xef5d, 0xffff, 0xffff, 0xffff, 0xef5d, 0xf5cc,
    0xfe6f, 0xf60f, 0xac4b, 0x8349, 0x4a06, 0x39a4, 0x6287, 0xa44c, 0xcd6e,
    0xb4ac, 0x49e6, 0x5247, 0xac8c, 0xed8b, 0xed29, 0xd4a8, 0xc446, 0x69e1,
    0x940e, 0xffff, 0xffff, 0xffff, 0xce37, 0xfda9, 0xf650, 0xe5ac, 0xb48b,
    0x18e3, 0x39a5, 0x18e3, 0x62a8, 0x5225, 0x2923, 0x18e3, 0x3164, 0x5a87,
    0x6a85, 0x72c5, 0x8347, 0xd4a6, 0xe508, 0x7a61, 0x61e3, 0xe73c, 0xffff,
    0xffff, 0xd56f, 0xfda9, 0xf60f, 0xedcd, 0xf5ec, 0xdd8e, 0xf60f, 0x3984,
    0x6ae9, 0xb4cd, 0x9c0b, 0x2103, 0xacad, 0xbcab, 0x9c0a, 0x39a5, 0xcd4e,
    0xed28, 0xd487, 0xa343, 0x59a1, 0xbd95, 0xffff, 0xffff, 0xedac, 0xfd88,
    0xeded, 0xf5cc, 0xfe0d, 0xf5ec, 0x8327, 0x18e3, 0x41c5, 0x4a26, 0x2123,
    0x49e6, 0x62c8, 0x5a67, 0x4a26, 0xddaf, 0xed49, 0xed48, 0xd466, 0xcc46,
    0x61c1, 0xa4b1, 0xffdf, 0xffff, 0xe508, 0xf568, 0xedcd, 0xf62e, 0xf60d,
    0xdd8c, 0x93eb, 0xb4ce, 0x93eb, 0x4a26, 0x2103, 0xac6b, 0xf5ab, 0xe56b,
    0x8328, 0xdd4a, 0xed69, 0xed69, 0xcc86, 0xd4a6, 0x69e0, 0x9c4d, 0xf79e,
    0xffff, 0xd4c8, 0xed06, 0xe5ad, 0xf5cc, 0xf5ed, 0x93c9, 0x5226, 0x6b09,
    0xb4ad, 0xac8c, 0x5a87, 0x2944, 0x7ae7, 0x6ac7, 0x7b6a, 0xc50d, 0xed69,
    0xed69, 0xcc66, 0xd4c7, 0x69e0, 0x9c4e, 0xf7be, 0xffff, 0xccec, 0xd444,
    0xedab, 0xed8b, 0xccca, 0xa44c, 0xa46c, 0x5a46, 0x62c8, 0xc50d, 0x62a7,
    0x2103, 0x39a5, 0x7b27, 0xe529, 0xe528, 0xe528, 0xed28, 0xcc66, 0xc445,
    0x7201, 0xacf1, 0xf7be, 0xffff, 0xbd0f, 0xbbc3, 0xed8b, 0xdd2a, 0xb449,
    0x3184, 0x62c8, 0xacad, 0x6b08, 0x5a87, 0xcd4e, 0x6ac7, 0xc50d, 0x5225,
    0x49e4, 0xabe7, 0xdcc8, 0xe4e7, 0xcca8, 0xaba4, 0x7241, 0xc5d6, 0xffdf,
    0xffff, 0xce17, 0xab63, 0xd4a6, 0xdd29, 0xe52a, 0xbccc, 0x5a87, 0x3164,
    0xb4cd, 0xedcd, 0xc4a9, 0x3184, 0xe5ce, 0xd4e8, 0x3143, 0x18e3, 0x2943,
    0x6a65, 0xdd2a, 0x92c1, 0x82c4, 0xdedb, 0xffff, 0xffff, 0xf79e, 0x9b88,
    0xa322, 0xed6a, 0xd4c8, 0x9be8, 0x9c2c, 0xe5cf, 0xcca9, 0x6265, 0x2103,
    0x838a, 0xe56a, 0xe529, 0xb447, 0x2964, 0x3164, 0xc52e, 0xcc67, 0x7a61,
    0xa46e, 0xf79e, 0xffff, 0xffff, 0xffff, 0xbdb5, 0x82a2, 0xb3c4, 0xe56a,
    0xd4a8, 0xed49, 0xed49, 0xe549, 0xdd4b, 0xdd6c, 0xedab, 0xe528, 0xe528,
    0xe528, 0xe508, 0xd52b, 0xdce7, 0x8261, 0x82c5, 0xd69a, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffdf, 0x93ed, 0x7241, 0xb3c4, 0xed8a, 0xd4a8, 0xe4e7,
    0xed49, 0xed69, 0xe529, 0xe528, 0xe528, 0xe508, 0xcc87, 0xd4c9, 0xdcc7,
    0x7a61, 0x7222, 0xb554, 0xf7be, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xe73c, 0x8b8b, 0x61c1, 0x8282, 0xe549, 0xdd29, 0xd4a8, 0xd4a8, 0xd4a7,
    0xd4a7, 0xd4c8, 0xccc9, 0xed69, 0xc425, 0x7201, 0x7242, 0xacd0, 0xef7d,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xe71c, 0x9c2e,
    0x59c3, 0x5181, 0x8aa3, 0xc446, 0xdd08, 0xdd29, 0xdd29, 0xd4a7, 0xb3a5,
    0x7a41, 0x69e1, 0x7aa5, 0xb553, 0xef7d, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xf79e, 0xc5f7, 0x7b4a, 0x59e3,
    0x5161, 0x5161, 0x5181, 0x59a0, 0x59a1, 0x61e1, 0x7264, 0x9c2d, 0xd699,
    0xf7be, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xef5d, 0xd699, 0xad33, 0x93ed, 0x8b8b,
    0x8bab, 0x9c6f, 0xbd94, 0xdeba, 0xf79e, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffdf, 0xf7be, 0xf79e, 0xf79e, 0xf7be, 0xffdf,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff};

const tImage LCD_RES_BB = {image_data_bb, 24, 24, 16};
