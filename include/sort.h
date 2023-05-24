#pragma once
#include <iostream>

#define SEQUENCE_ELEMS_NUMBER 3
#define RECORD_MAX_VALUE 100
#define TAPES_NUMBER 4

void sortWithInputFile(bool dispEveryRun, std::string inputFileName);
void sortWithGeneratedData(int recordsNum, bool autoDataGeneration, bool dispEveryRun);