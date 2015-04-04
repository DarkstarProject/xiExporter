#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include "maplib.h"

class Callback :
	public FFxiMapLib::ICallbackClass {
public:
	Callback(void);
	~Callback(void);

        bool HandleNewObject(char* ID, UINT NumIndexes, UINT NumVerteses, CONST void* pIndexData, CONST void* pVertexData);
        bool HandleNewTexture(char* ID, char* pdata);
        void HandleProgress(const char* pStage, UINT Progress, UINT Total);
private:
  int count;
};

