#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

//compile to test g++ -o test main.cpp
using namespace std;
//functions
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

bool checkOper(const vector<string>& input, int pos){
    char operations[4] = {'-', '+', '*', '/'};
    //char oper = input.at(i).substr(0, 1);
    if (input.at(pos).find(operations[0]) > 0 && input.at(pos).find(operations[0]) < 10) {
        return true;
    } else if (input.at(pos).find(operations[1]) > 0 && input.at(pos).find(operations[1]) < 10) {
        return true;
    } else if (input.at(pos).find(operations[2]) > 0 && input.at(pos).find(operations[2]) < 10) {
        return true;
    } else if (input.at(pos).find(operations[3]) > 0 && input.at(pos).find(operations[3]) < 10) {
        return true;
    } else {
        return false;
    }
}


bool isPipe(const vector<string>& input, int pos){
    return (input.at(pos).find('p') < 3 and input.at(pos).find('p') > 0);
}

int whatVal(vector<string>& input, vector<int>& valList, int pos){
    vector<string> inputNames;
    for(int i = 0; i < input.size(); i++){
        if(!isPipe(input, i)){
            if(checkOper(input, i)){
                inputNames.push_back(input.at(i).substr(1));
            }else{
                inputNames.push_back(input.at(i));
            }
        }
    }
    sort(inputNames.begin(), inputNames.end());
    for(int i = 0; i < valList.size(); i++){
        if(checkOper(input, pos)){
            if(input.at(pos).substr(1) == inputNames.at(i)){
                return valList.at(i);
            }
        }else{
            if(input.at(pos) == inputNames.at(i)){
                return valList.at(i);
            }
        }
    }
    return -1;          //Error, val not found
}

int whichPipe(const vector<string>& pipe, int pos){
    return stoi(pipe.at(pos).substr(pipe.at(pos).find('p')+1));
}

int doOper(int a, int b, vector<string>& input, int pos){
    if(input.at(pos).substr(0,1) == "+"){
        return a + b;
    }else if(input.at(pos).substr(0,1) == "-"){
        return a - b;
    }else if(input.at(pos).substr(0,1) == "*"){
        return a * b;
    }else if(input.at(pos).substr(0,1) == "/"){
        return a / b;
    }
    return -1;          //Error, no Operation
}

//File Reading
//-------------------------------------------------------------------------------------------------------------------------------------------------


int main(int argc, char** argv) {
    vector<int> inputValList;   //input val in dynamic data struct do allow versatile amount of input
    vector<string> input;
    vector<string> updating;
    int pipes[4][2];            //p0, p1, p2, p3 w/ read or write

    if(argc != 3){  //error if input not provided
        cout<< "No input argument is found.\n";
        return 1;
    }

    ifstream file(argv[1]);
    //ifstream file("input1-1.txt");
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
    //ifstream fileOper("s1-1.txt");
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

//Pipe actions
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //pipe creation
    for(int i = 0; i < 4; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Pipe");
            return 1;
        }
    }

    for(int i =0; i < 4; i++){
        pid_t pID = fork();
        if(pID == -1){
            perror("Fork");
        }
        if(pID == 0) {                       //child process
            close(pipes[i][1]);
            if (i > 0) {
                close(pipes[i - 1][0]);
            }

            int result;
            read(pipes[i][0], &result, sizeof(result));
            cout<< "p" << i << " = "<< result<< endl;

            close(pipes[i][0]);         // Close read end
            return 0;
        }
        close(pipes[i][0]);             //parent process
    }

    //input values in each pipe
    int result;
    for(int i = 0; i < input.size(); i++){
        int outPipe = whichPipe(updating, i);
        if(checkOper(input, i) && i > 0){
            if(isPipe(input, i)) {
                int inPipe = whichPipe(input, i);

                int currVal;
                int currPipe = pipes[inPipe][0];
                read(currPipe, &currVal, sizeof(currVal));
                close(currPipe);

                int prevVal;
                int prevPipeLoc = whichPipe(updating, i-1);
                int prevPipe = pipes[prevPipeLoc][0];
                read(prevPipe, &prevVal, sizeof(prevVal));
                close(prevPipe);

                result = doOper(prevVal, currVal, input, i);
                cout<< "a is "<< prevVal<< " and b is"<< currVal<< "result is "<< result<< endl;
                write(pipes[outPipe][1], &result, sizeof(result));

            }else{      //input
                int prevVal;
                int prevPipeLoc = whichPipe(updating, i-1);
                int prevPipe = pipes[prevPipeLoc][0];
                read(prevPipe, &prevVal, sizeof(prevPipeLoc));
                close(prevPipe);

                result = doOper(prevVal, whatVal(input, inputValList, i), input, i);
                cout<< "a is "<< prevVal<< " and b is"<< whatVal(input, inputValList, i)<< "result is "<< result<< endl;
                write(pipes[outPipe][1], &result, sizeof(result));
            }
        }else if(isPipe(input, i)){
            int inPipe = whichPipe(input, i);

            int prevResult;
            int prevPipe = pipes[inPipe][0];
            read(prevPipe, &prevResult, sizeof(prevResult));
            close(prevPipe);

            write(pipes[outPipe][1], &prevResult,sizeof(prevResult));
        }else{          //if inputting input val directly into pipe
            int val = whatVal(input, inputValList, i);
            cout<< "val into pipe is "<< val<< endl;
            write(pipes[outPipe][1], &val, sizeof(val));
        }
    }

    //waiting for all child processes
    for(int i =0; i< 4; i++){
        wait(nullptr);
    }
    return 0;
}
