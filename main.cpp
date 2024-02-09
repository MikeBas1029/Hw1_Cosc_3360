#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

//compile to rest g++ -o test main.cpp

bool varToPipe(){   //to determine if input var will be inside pipe, if false then it's another pipe going into another
    return false;   //write next
}

bool arthVarToPipe(){   //to determine if arithmetic is being used with the input var
    return false;       //write next
}

bool arthPipeToPipe(){  //to determine if arithmetic is being used with the pipes
    return false;       //write next
}

using namespace std;
int main(int argc, char** argv) {
    vector<int> inputValList;   //input val in dynamic data struct do allow versatile amount of input
    vector<string> input;
    vector<string> updating;
    //pid_t pId;       //internal var
    //int pipeFD[2];
    char operations[4] = {'-', '+', '*', '/'};

    if(argc != 3){  //error if input not provided
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

    /*cout<<"Output:\n";
    for(int num : inputValList){
        cout<< num<<endl;
    }*/

    file.close();

    ifstream fileOper(argv[2]);     //fileOper -> File Operations
    if(!fileOper.is_open()){    //If it can't find file
        cout<< "The second file can't be found.\n";
        return 1;
    }
    while(getline(fileOper, inputLine)){
        inputLine.erase(remove(inputLine.begin(), inputLine.end(), ' '), inputLine.end()); //removing all spacing to just call the ordering of inputs
        if(inputLine.find("->") > 0 && inputLine.find("->") < 20){
            input.push_back(inputLine.substr(0, inputLine.find("->")));
            updating.push_back(inputLine.substr(inputLine.find("->")));

            //cout << inputLine.substr(0, inputLine.find("->"))<< endl;
            //cout<< inputLine.substr(inputLine.find("->"))<< endl;
        }
    }
    fileOper.close();   //Closing rather than continuing the process for less nesting

    for(int x = 0; x < input.size(); x++){
        if(varToPipe()){
            if(arthVarToPipe()){

            }else{

            }
        }else{
            if(arthPipeToPipe()){

            }else{

            }
        }
    }

    return 0;
}
