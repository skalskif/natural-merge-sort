#pragma once

#define BLOCK_SIZE 40
#define RECORDS_PER_BLOCK BLOCK_SIZE / sizeof(Record)
#define SEQUENCE_ELEMS_NUMBER 3

struct Record {
	int a1;
	int q;
};

Record* recordFileRead();
Record* recordTapeRead(int tapeIndex);
void recordFileWrite(Record* record);
void recordTapeWrite(Record* record, int tapeIndex);
int readInputFile(char* fileName = nullptr);
int getReadsNumber();
int getWritesNumber();
void flushWriteBuffers();
void clearData();