#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <thread>
#include <chrono>

//compile to test g++ -o test main.cpp
using namespace std;
//functions
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

bool checkOper(const vector<string>& input, int pos){
    char operations[4] = {'-', '+', '*', '/'};
    //char oper = input.at(i).substr(0, 1);
    if (input.at(pos).find(operations[0]) != string::npos) {
        return true;
    } else if (input.at(pos).find(operations[1]) != string::npos) {
        return true;
    } else if (input.at(pos).find(operations[2]) != string::npos) {
        return true;
    } else if (input.at(pos).find(operations[3]) != string::npos) {
        return true;
    } else {
        return false;
    }
}


bool isPipe(const vector<string>& input, int pos){
    return (input.at(pos).find('p') != string::npos);
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

/*
struct testPipes{
    int newPipe[3][2];

    testPipes(){
        for(int i = 0; i < 3; ++i){
            if (pipe(newPipe[i]) == -1) {
                perror("Pipe");
                exit(1);
            }
        }
    }

    ~testPipes(){
        for(int i = 0; i < 3; ++i){
            close(newPipe[i][0]);
            close(newPipe[i][1]);
        }
    }
};

int resultPrevPipe(const map<string, int>& results, int range, int pipeNum) {
    string pipe = "p" + std::to_string(pipeNum);
    int result;
    for(const auto& map : results) {
        if(map.first.find(pipe) != string::npos) {
            result = map.second;
        }
    }
    return result;
}

*/
//File Reading
//-------------------------------------------------------------------------------------------------------------------------------------------------


int main(int argc, char** argv) {
    vector<int> inputValList;   //input val in dynamic data struct do allow versatile amount of input
    vector<string> input;
    vector<string> updating;
    //int pipes[4][2];            //p0, p1, p2, p3 w/ read or write

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
            updating.push_back(inputLine.substr(inputLine.find("->")+2));

            //cout << inputLine.substr(0, inputLine.find("->"))<< endl;
            //cout<< inputLine.substr(inputLine.find("->"))<< endl;
        }
    }
    fileOper.close();   //Closing rather than continuing the process for less nesting

    vector<int> pipeTotal;
    for(int i = 0; i < updating.size(); i++){
        pipeTotal.push_back(stoi(updating.at(i).substr(1)));
    }
    sort(pipeTotal.begin(), pipeTotal.end());
    pipeTotal.erase(std::unique(pipeTotal.begin(), pipeTotal.end()), pipeTotal.end());

    int pipeSize = pipeTotal.size();

    int pipes[pipeSize][2];

//Pipe actions
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //pipe creation
    for(int i = 0; i < pipeSize; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Pipe");
            return 1;
        }

    }

    for(int i =0; i < pipeSize; i++){
        pid_t pID = fork();
        if(pID == -1){
            perror("Fork");
        }
        if(pID == 0) {                       //child process
            close(pipes[i][1]);

            int result;
            read(pipes[i][0], &result, sizeof(result));
            cout<< "p" << i << " = "<< result<< endl;

            close(pipes[i][0]);         // Close read end
            exit(0);

        }

        close(pipes [i][0]);
        //for(int x = 0; x < 3; x++){
        //    wait(nullptr);
        //}
    }

    //input values in each pipe
    /*
    map<string, int> result;
    for(int i = 0; i < input.size(); i++){
        if(checkOper(input, i)){
            if(isPipe(input, i)) {
                int prevVal = resultPrevPipe(result, i, whichPipe(updating, i-1));
                int currVal = resultPrevPipe(result, i, whichPipe(input, i));

                int res = doOper(prevVal, currVal, input, i);
                string word = "p" + to_string(whichPipe(input, i)) + " = " + to_string(prevVal) + " (operation) -> " + "p" + to_string(whichPipe(updating, i-1)) + " = " + to_string(currVal) + "\n";
                cout<< word<< " and result is "<< res<< " "<<  i<< endl;
                write(pipes[i][1], &res, sizeof(res));
            }else{
                int val = whatVal(input, inputValList, i);
                int prevVal = resultPrevPipe(result, i, whichPipe(updating, i-1));
                int res = doOper(prevVal, val, input, i);
                string word = input.at(i) + "whatever operation with p" + to_string(whichPipe(updating, i-1))+ " = " + to_string(prevVal) + " -> " + "p" + to_string(whichPipe(updating, i)) + "\n";
                cout<< word<< " and result is "<< res << i<< endl;
                result.insert(pair<string, int>(word, res));
                write(pipes[i][1], &res, sizeof(res));
            }
        }else{
            if(isPipe(input, i)) {      //making to equal to
                string word = input.at(i) + " -> " + "p" + to_string(whichPipe(updating, i));
                int val = resultPrevPipe(result, i, whichPipe(input, i));
                cout<< word<< " "<< input.at(i)<< " = "<< val<< endl;
                result.insert(pair<string, int>(word, val));
                write(pipes[i][1], &val, sizeof(val));
            }else {
                string word = input.at(i) + " -> " + "p" + to_string(whichPipe(updating, i));
                int val = whatVal(input, inputValList, i);
                result.insert(pair<string, int>(word, val));
                write(pipes[i][1], &val, sizeof(val));
            }
        }
        //close(pipes[i][1]);
    }

    */
    int result;
    for(int i = 0; i < input.size(); i++){
        int outPipe = whichPipe(updating, i);

        if(checkOper(input, i)){
            if(isPipe(input, i)) {
                int inPipe = whichPipe(input, i);

                int prevVal;
                int prevPipeLoc = whichPipe(updating, i-1);
                int prevPipe = pipes[prevPipeLoc][0];
                close(pipes[prevPipeLoc][1]);
                read(prevPipe, &prevVal, sizeof(prevVal));
                close(prevPipe);

                //this_thread::sleep_for(chrono::milliseconds(5));

                int currVal;
                int currPipe = pipes[inPipe][0];
                close(pipes[inPipe][1]);
                read(currPipe, &currVal, sizeof(currVal));
                close(currPipe);

                //this_thread::sleep_for(chrono::milliseconds(5));

                result = doOper(prevVal, currVal, input, i);
                cout<< "for p"<< inPipe<<  " to p"<< outPipe<<  ", a is "<< prevVal<< " and b is "<< currVal<< " result is "<< result<< endl;

                write(pipes[outPipe][1], &result, sizeof(result));

            }else{      //input
                int prevVal;
                int prevPipeLoc = whichPipe(updating, i-1);
                int prevPipe = pipes[prevPipeLoc][0];

                close(pipes[prevPipeLoc][1]);
                read(prevPipe, &prevVal, sizeof(prevPipeLoc));
                close(prevPipe);

                //this_thread::sleep_for(chrono::milliseconds(5));

                int val = whatVal(input, inputValList, i);

                result = doOper(prevVal, val, input, i);
                cout<< "input to p"<< outPipe<<  " a is "<< prevVal<< " and b is "<< val<< " result is "<< result<< endl;

                write(pipes[outPipe][1], &result, sizeof(result));
            }
        }else if(isPipe(input, i)){
            int inPipe = whichPipe(input, i);

            int prevResult;
            int prevPipe = pipes[inPipe][0];
            close(pipes[inPipe][1]);
            read(prevPipe, &prevResult, sizeof(prevResult));
            close(prevPipe);

            //this_thread::sleep_for(chrono::milliseconds(5));

            cout<< "making p"<< inPipe<< " = p"<< outPipe<< " is "<< prevResult<< endl;

            write(pipes[outPipe][1], &prevResult,sizeof(prevResult));
        }else{          //if inputting input val directly into pipe
            int val = whatVal(input, inputValList, i);

            cout<< "input val to p"<< outPipe<< " is "<< val<< endl;

            write(pipes[outPipe][1], &val, sizeof(val));
        }
    }

    /*
    for(int i = 0; i < 3; i++){
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    */
    //waiting for all child processes
    for(int i =0; i< pipeSize; i++){
        wait(nullptr);
    }


    return 0;
}
