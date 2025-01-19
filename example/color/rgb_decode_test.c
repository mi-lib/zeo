#include <zeo/zeo_render_color.h>

int main(int argc, char *argv[])
{
  zRGB rgb;

  if( argc < 2 ) return 1;
  printf( "entered string -> %s\n", argv[1] );
  zRGBPrint( zRGBDecodeStr( &rgb, argv[1] ) );
  return 0;
}
