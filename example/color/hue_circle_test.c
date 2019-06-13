#include <zeo/zeo_color.h>

#define DIV 100

int main(int argc, char *argv[])
{
  zHSV hsv;
  zRGB rgb;
  register int i;

  hsv.sat = 1.0;
  hsv.val = 1.0;
  for( i=0; i<DIV; i++ ){
    hsv.hue = 360*(double)i/DIV;
    zHSV2RGB( &hsv, &rgb );
    zRGBPrint( &rgb );
  }
  return 0;
}
