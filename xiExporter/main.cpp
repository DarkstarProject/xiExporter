#include <stdio.h>
#include <iostream>
#include <string>
#include "MapExporter.h"

MapExporter mapExporter("C:\\Program Files (x86)\\PlayOnline\\SquareEnix\\FINAL FANTASY XI\\");

int main( int argc, const char* argv[] )
{

  // behemoth dominion
  // int zoneId = 100;
  // Rabao
  int zoneId = 127;

  printf( "Starting FFXI exporter. Exporting (%d)\n\n", zoneId );

  if(!mapExporter.LoadMap(zoneId)){
    printf( "Failed loading map (%d)\n", zoneId );
  }

  printf("Loaded FFXI map\n");

  if(!mapExporter.ExportMap()){
    printf("Error exporting map (%d)\n", zoneId);
  } else {
    printf("Exported map!\n");
  }

  // keep window open
  getchar();
}
