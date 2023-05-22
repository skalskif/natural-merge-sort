#include <cstdlib>
#include <vector>
#include <algorithm>
#include <limits.h>
#include "sort.h"
#include "readWrite.h"

using namespace std;

int tapesUsed;
int recordsNumber;
int phases = 0;
bool sortEnd = false;
bool displayEveryRun;


int calculateSequenceSum(Record* record) {
	if (!record || !record->q) {
		return 0;
	}
	if (record->q == 1) {
		return record->a1 * SEQUENCE_ELEMS_NUMBER;
	}
	
	return record->a1 * (1 - pow(record->q, SEQUENCE_ELEMS_NUMBER)) / (1 - record->q);
}

void generateDataAuto() {
	srand(time(NULL));
	Record* record = new Record();
	for (int i = 0; i < recordsNumber; i++) {
		record->a1 = rand() % RECORD_MAX_VALUE + 1;
		record->q = rand() % RECORD_MAX_VALUE + 1;
		recordFileWrite(record);
	}
	
	flushWriteBuffers();
}

void generateData() {
	Record* record = new Record();
	for (int i = 0; i < recordsNumber; i++) {
		cout << i + 1 << ": ";
		cin >> record->a1 >> record->q;
		recordFileWrite(record);
	}
	
	flushWriteBuffers();
}

void distribute() {
	int prevSequenceSum = INT_MIN;
	int tapeNum = 0;
	tapesUsed = 1;
	for (int i = 0; i < recordsNumber; i++) {
		Record* record = recordFileRead();
		int sequenceSum = calculateSequenceSum(record);
		if (prevSequenceSum > sequenceSum) {
			tapeNum = (tapeNum + 1) % TAPES_NUMBER;
			if (tapesUsed < TAPES_NUMBER) {
				tapesUsed++;
			}
		}
		
		prevSequenceSum = sequenceSum;
		recordTapeWrite(record, tapeNum);
	}
	
	flushWriteBuffers();
	sortEnd = tapesUsed == 1;
}

bool compareRecords(Record* record1, Record* record2) {
	return (calculateSequenceSum(record1) < calculateSequenceSum(record2));
}

void merge() {
	vector<Record*> records;
	vector<Record*> nextRunRecords(tapesUsed, nullptr);	// we store record here when it turns out that current run is over 
	int recordsWritten = 0;
	
	while (recordsWritten != recordsNumber) {	
		for (int i = 0; i < tapesUsed; i++) {
			int prevSequenceSum = nextRunRecords.at(i) ? calculateSequenceSum(nextRunRecords.at(i)) : INT_MIN;

			while (true) {
				Record* record = recordTapeRead(i);
				int sequenceSum = calculateSequenceSum(record);
				if (!record || prevSequenceSum > sequenceSum) {
					nextRunRecords.at(i) = record;
					break;
				}
				prevSequenceSum = sequenceSum;
				records.push_back(record);
			}
		}
		
		std::sort(records.begin(), records.end(), compareRecords);
		for (int i = 0; i < records.size(); i++) {
			recordFileWrite(records.at(i));
			recordsWritten++;
		}
		
		records = nextRunRecords;
		records.erase(remove(records.begin(), records.end(), nullptr), records.end());
	}

	flushWriteBuffers();
}

void sort() {
	int writes = 0;

	while (true) {
		distribute();
		
		if (sortEnd) {
			break;
		}
	
		merge();
		phases++;
		writes = getWritesNumber();
		
		if (displayEveryRun) {
			cout << "\nInput file content after phase " << phases << ":\n";
			readInputFile();
		}
	}

	cout << "\nInput file content after sorting:\n";
	readInputFile();
	cout << "\nNumber of phases: " << phases << "\n";
	cout << "Number of read operations: " << getReadsNumber() << "\n";
	cout << "Number of write operations: " << writes << "\n";

	phases = 0;
	clearData();
}

void sortWithInputFile(bool dispEveryRun, string inputFileName) {
	cout << "\nInput file content:\n";
	recordsNumber = readInputFile((char*)inputFileName.c_str());
	displayEveryRun = dispEveryRun;

	sort();
}

void sortWithGeneratedData(int recordsNum, bool autoDataGeneration, bool dispEveryRun) {
	recordsNumber = recordsNum;
	displayEveryRun = dispEveryRun;

	if (autoDataGeneration) {
		generateDataAuto();
	}
	else {
		generateData();
	}

	cout << "\nInput file content:\n";
	readInputFile();
	
	sort();
}
