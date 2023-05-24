#include <iostream>
#include "../include/sort.h"

using namespace std;

char command;

void sortInput() {
    cout << "Display file content every phase? [y/n]\n";
    cin >> command;

    bool displayEveryRun = command == 'y' || command == 'Y';

    cout << "[1] Generate records automatically\n";
    cout << "[2] Generate records manually\n";
    cout << "[3] Load input file\n";
    cin >> command;

    switch (command) {
        case '1':
        case '2':
        {
            int recordsNum;
            cout << "Type records number:\n";
            cin >> recordsNum;
            if (recordsNum) {
                sortWithGeneratedData(recordsNum, command == '1', displayEveryRun);
            }
            break;
        }

        case '3':
        {
            string fileName;
            cout << "Type file name:\n";
            cin >> fileName;
            sortWithInputFile(displayEveryRun, fileName);
            break;
        }

        default:
        {
            break;
        }
    }
}


int main()
{
    while (true) {
        cout << "\n[1] Sort\n";
        cout << "[2] Exit\n";

        cin >> command;

        switch (command) {
        case '1':
            sortInput();
            break;

        case '2':
            return 0;

        default:
            break;
        }
        
    }

    return 0;
}
