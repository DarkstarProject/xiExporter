#include <stdio.h>
#include <iostream>
#include <string>
#include "MapExporter.h"
#include <vector>

MapExporter mapExporter;

int main( int argc, const char* argv[] )
{

  // behemoth dominion
  int zoneId = 127;
  const char* path = "C:\\Program Files (x86)\\PlayOnline\\SquareEnix\\FINAL FANTASY XI\\";

  if(argc >= 2){
    zoneId = atoi(argv[1]);
  } else if(argc == 3){
    path = _strdup(argv[2]);
  }

  mapExporter.SetPath(path);

  printf("Set path to (%s)\n", path);

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
