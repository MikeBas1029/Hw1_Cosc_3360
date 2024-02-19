#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

//compile to test g++ -o test main.cpp
using namespace std;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//functions

bool checkOper(const vector<string>& input, int pos){
    char operations[4] = {'-', '+', '*', '/'};
    for(int i = 0; i < input.size(); i++){
        if (input.at(i).find(operations[0])) {
            return true;
        } else if (input.at(i).find(operations[1])) {
            return true;
        } else if (input.at(i).find(operations[2])) {
            return true;
        } else if (input.at(i).find(operations[3])) {
            return true;
        } else {
            return false;
        }
    }
}

bool isPipe(const vector<string>& input, int pos){
    if(input.at(pos).find('p')){
        return true;
    }
    return false;
}

int whichPipe(const vector<string>& input, int pos){
    return stoi(input.at(pos).substr(input.at(pos).find('p')+1));
}

int whichOut(const vector<string>& updated, int pos){
    return stoi(updated.at(pos).substr(updated.at(pos).find('p')+1));
}

int doOper(const vector<string>& input, int pos){

}

//maybe a overloaded doOper?

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

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //pipe action

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

    /*ChatGpt Example
     else if (i < whichOut(updating, i)) {
    // Open the read end of the previous pipe
    int prev_pipe_read = pipes[i - 1][0];

    // Read data from the previous pipe
    int prev_result;
    read(prev_pipe_read, &prev_result, sizeof(prev_result));

    // Perform operation using the data read from the previous pipe and the current input
    result = doOper(prev_result, inputValList.at(i));

    // Close the read end of the previous pipe after reading
    close(prev_pipe_read);
    }
     */



    //input values in each pipe
    int result;
    for(int i = 0; i < inputValList.size(); i++){
        if(checkOper(input, i) && i > 0){
            if(isPipe(input, i)) {
                if (i == whichOut(updating, i)) {
                    //write if it's in i correct order
                    result = doOper(input, i);

                } else if (i < whichOut(updating, i)) {
                    //write if p is not in i order
                    result = doOper(input, i);

                }
            }else{
                result = doOper(input, i);
                if (i == whichOut(updating, i)) {
                    //write
                    result = doOper(input, i);

                } else if (i < whichOut(updating, i)) {
                    //
                    result = doOper(input, i);

                }
            }
        }else if(isPipe(input, i)){
            int pos = whichPipe(input, i);
        }else{
            write(pipes[i][1], &inputValList.at(i), sizeof(inputValList.at(i)));
        }
    }

    //waiting for all child processes
    for(int i =0; i< 4; i++){
        wait(nullptr);
    }

    return 0;
}
