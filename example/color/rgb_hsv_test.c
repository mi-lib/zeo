#include <zeo/zeo_color.h>

int main(int argc, char *argv[])
{
  zRGB rgb;
  zHSV hsv;

  if( argc < 2 ) return 1;
  printf( "entered string -> %s\n", argv[1] );
  printf( "<original RGB>\n" );
  zRGBPrint( zRGBDec( &rgb, argv[1] ) );

  printf( "<converted HSV>\n" );
  zRGB2HSV( &rgb, &hsv );
  zHSVPrint( &hsv );

  printf( "<reconverted RGB>\n" );
  zHSV2RGB( &hsv, &rgb );
  zRGBPrint( &rgb );

  return 0;
}
