#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include "readWrite.h"

using namespace std;

struct BlockBuffer {
	char* base;		// base address of block
	int length = 0;	// number of bytes already read from/written to buffer
};

struct FileData {
	string name;
	ofstream os;
	ifstream is;
	BlockBuffer* readBuffer;
	BlockBuffer* writeBuffer;
};

FileData* input = nullptr;	// input file
vector<FileData*> tapes;	// vector of tapes we will read/write to
int reads = 0, writes = 0;


FileData* newFileData(string name) {
	FileData* file = new FileData();
	file->name = name;
	file->readBuffer = new BlockBuffer;
	file->readBuffer->base = new char[BLOCK_SIZE];
	file->writeBuffer = new BlockBuffer;
	file->writeBuffer->base = new char[BLOCK_SIZE];
	return file;
}

void blockRead(FileData* file) {
	if (!file->is.is_open()) {
		file->is.open(file->name, ios_base::binary);
	}
	file->is.read(file->readBuffer->base, BLOCK_SIZE);
	file->readBuffer->length = 0;
	
	if (file->is.gcount()) {
		reads++;
	}
}

Record* recordRead(FileData* file) {
	if (file->is.is_open() && file->is.eof() && file->readBuffer->length >= file->is.gcount()) {
		return nullptr;
	}
	else if (!file->is.is_open() || file->readBuffer->length >= BLOCK_SIZE) {
		blockRead(file);

		if (!file->is.gcount()) {
			return nullptr;
		}
	}

	Record* record = new Record();
	memcpy(record, file->readBuffer->base + file->readBuffer->length, sizeof(Record));
	file->readBuffer->length += sizeof(Record);

	return record;
}

Record* recordFileRead() {
	return recordRead(input);
}

Record* recordTapeRead(int tapeIndex) {
	return recordRead(tapes.at(tapeIndex));
}

void blockWrite(FileData* file) {
	if (!file->os.is_open()) {
		file->os.open(file->name, ios_base::trunc | ios_base::binary);	
	}
	file->os.write(file->writeBuffer->base, file->writeBuffer->length);
	file->os.flush();
	file->writeBuffer->length = 0;

	writes++;
}

void recordWrite(Record* record, FileData* file) {
	memcpy(file->writeBuffer->base + file->writeBuffer->length, record, sizeof(Record));
	file->writeBuffer->length += sizeof(Record);
	if (file->writeBuffer->length >= BLOCK_SIZE) {
		blockWrite(file);
	}
}

void recordFileWrite(Record* record) {
	if (!input) {
		input = newFileData("input");
	}

	recordWrite(record, input);
}

void recordTapeWrite(Record* record, int tapeIndex) {
	if (tapeIndex >= tapes.size()) {
		tapes.push_back(newFileData("tape" + to_string(tapeIndex)));
	}

	return recordWrite(record, tapes.at(tapeIndex));
}

void closeFileStreams(FileData* file) {
	if (file->is.is_open()) {
		file->is.close();
	}
	if (file->os.is_open()) {
		file->os.close();
	}
}

void closeAllStreams() {
	closeFileStreams(input);
	for (int i = 0; i < tapes.size(); i++) {
		closeFileStreams(tapes.at(i));
	}
}

void flushWriteBuffers() {
	if (input->os.is_open() && input->writeBuffer->length) {
		blockWrite(input);
	}
	else {
		for (int i = 0; i < tapes.size(); i++) {
			FileData* file = tapes.at(i);
			if (file->writeBuffer->length) {
				blockWrite(file);	
			}
		}
	}

	closeAllStreams();
}

void clearData() {
	for (int i = 0; i < tapes.size(); i++) {
		remove(tapes.at(i)->name.c_str());
	}
	tapes.clear();
	delete input;
	input = nullptr;
	reads = 0;
	writes = 0;
}

int calculateSum(Record* record) {
	if (!record || !record->q) {
		return 0;
	}
	if (record->q == 1) {
		return record->a1 * SEQUENCE_ELEMS_NUMBER;
	}

	return record->a1 * (1 - pow(record->q, SEQUENCE_ELEMS_NUMBER)) / (1 - record->q);
}

int readInputFile(char* fileName) {
	if (!input) {
		input = newFileData(fileName);
	}

	int recordsRead = 0;
	int savedReads = reads;
	while(true) {
		Record* record = recordFileRead();
		if (!record) {
			break;
		}
		if (record->a1 < 0 || record->q < 0) {
			cout << input->is.gcount();
		}
		cout << recordsRead + 1 << ": " << record->a1 << " " << record->q << " " << calculateSum(record) << "\n";
		recordsRead++;
	}
	
	reads = savedReads;
	input->is.close();

	return recordsRead;
}

int getReadsNumber() {
	return reads;
}

int getWritesNumber() {
	return writes;
}