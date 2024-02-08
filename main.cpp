#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

//compile to rest g++ -0 test main.cpp

using namespace std;
int main(int argc, char** argv) {
    vector<int> inputValList;   //input val in dynamic data struct do allow versatile amount of input
    int p0, p1, p2, p3;         //internal var
    char operations[4] = {'-', '+', '*', '/'};

    if(argc != 2){  //error if input not provided
        cout<< "No input argument is found.\n";
        return 1;
    }

    ifstream file(argv[1]);
    if(!file.is_open()){    //If it can't find file
        cout<< "No file can be found.\n";
        return 1;
    }

    string inputLine;
    int numVal;
    while(getline(file, inputLine)){   //to read lines of file
        replace(inputLine.begin(), inputLine.end(), ',', ' ');  //to fix if chance of having ','

        stringstream str(inputLine);
        while(str >> numVal){
            inputValList.push_back(numVal);
        }
    }
    file.close();

    ifstream fileOper(argv[2]);     //fileOper -> File Operations
    if(!fileOper.is_open()){    //If it can't find file
        cout<< "No file can be found.\n";
        return 1;
    }
    while(getline(fileOper, inputLine)){
        if(inputLine.find("->")){
            //read left and right for arthritic and piping ordering
        }
    }

    /*cout<<"Output:\n";
    for(int num : inputValList){
        cout<< num<<endl;
    }*/




    return 0;
}
