#include "Callback.h"
using namespace std;


Callback::Callback(void)
{
  count = 0;
}


Callback::~Callback(void)
{
}

bool Callback::HandleNewObject(char* ID, UINT NumIndexes, UINT NumVerteses, CONST void* pIndexData, CONST void* pVertexData)
{

  count++;
  // printf("Object %d (%s)\n", count, ID);
  if(count == 243){
    printf("Handling new object (%s) Indexes %d, Verteses %d\n", ID, NumVerteses, NumVerteses);
    // std::string buf("objs\\");
    // buf.append(ID);
    std::stringstream buf;
    buf << "obj" << count << ".obj";
    const std::string tmp = buf.str();

    ofstream objFile(tmp.c_str());

    if(objFile.is_open()){
      // output this object

      float* data = (float*)pVertexData;
      for(unsigned int i=0; i<NumVerteses*3; i+=3){
        int offset = 0;
        printf("Vertex: x: %f y: %f z: %f\n", data[i + offset], data[i+1 + offset], data[i+2 + offset]);
        objFile << "v " << std::fixed << data[i] << " " << std::fixed << data[i+1] << " " << std::fixed << data[i+2] << "\n";
      }

      objFile << "\n";

      short* indexData = (short*)pIndexData;
      for(unsigned int i=0; i<NumIndexes; i+=4){
        printf("Face: %u %u %u %u\n", indexData[i], indexData[i+1], indexData[i+2], indexData[i+3]);
        objFile << "f " << (indexData[i] + 1) << " " << (indexData[i+1] + 1) << " " << (indexData[i+2] + 1) << " " << (indexData[i+3] + 1) << "\n";
      }

      printf("Wrote (%s) to file\n", ID);
      objFile.close();
    } else {
      printf("Failed to open file (%s)\n", tmp.c_str());
    }
  }

  return true;
}

bool Callback::HandleNewTexture(char* ID, char* pdata)
{

  return true;
}

void Callback::HandleProgress(const char* pStage, UINT Progress, UINT Total)
{
  printf("Progress of %s %d / %d \n", pStage, Progress, Total);
}
