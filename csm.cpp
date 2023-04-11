#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#define FILE_SIZE 4

enum FILENAME {EMPLOYEE_FILE, SALARY_CONFIG_FILE, TEAMS_FILE, WORKING_HOURS_FILE};
using namespace std;
const string FILENAMES[] = {"employees.csv", "salary_configs.csv", "teams.csv", "working_hours.csv"};

vector<vector<string>> readCSV(string fileName){
   ifstream inputFile(fileName);
   vector<vector<string>> readedFile;
   string lineOfFile;

   while(getline(inputFile, lineOfFile)){
       vector<string> elements;
       istringstream line(lineOfFile);
       string fileElement;
       while(getline(inputFile, fileElement, ',')){
           elements.push_back(fileElement);
       }
       readedFile.push_back(elements);
   }

   inputFile.close();
   return readedFile;
}


void readInput(){
    for(int i = 0; i < FILE_SIZE; i++){
        if(i == EMPLOYEE_FILE){
            vector<vector<string>> employeeReadFile = readCSV(FILENAMES[i]);
            //handling
        }
        if(i == SALARY_CONFIG_FILE){
            vector<vector<string>> salaryConfigsFile;
            //handling
        }
        if (i == TEAMS_FILE){
            vector<vector<string>> teamsFile;
            //handling
        }
        if (i == WORKING_HOURS_FILE){
            vector<vector<string>> workingHoursFile;
            //handling
        }
    }
}

int main(int argc, char *argv[]) {

    return 0;
}
