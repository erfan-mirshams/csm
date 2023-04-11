#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#define FILE_SIZE 4
#define DAYS_IN_MONTH 30
#define HOURS_IN_DAY 24
#define ASSETS_FOLDER "assets/"
#define EXPERTISE_SIZE 4
#define EXPERTISE_COLUMN_SIZE 6
#define EMPLOYEE_COLUMN_SIZE 4

enum FILE_NAME {SALARY_CONFIG_FILE, EMPLOYEE_FILE, TEAMS_FILE, WORKING_HOURS_FILE};
enum LEVEL {JUNIOR, EXPERT, SENIOR, TEAMLEAD};
enum EMPLOYEE_COLUMN {EMP_ID, EMP_NAME, EMP_AGE, EMP_LEVEL};
enum EXPERTISE_COLUMN {EXP_LEVEL, EXP_BASE_SALARY, EXP_SALARY_PER_HOUR, EXP_SALARY_PER_EXTRA_HOUR, EXP_OFFICIAL_WORKING_HOURS, EXP_TAX_PERCENTAGE};
using namespace std;
const string FILE_NAMES[] = {"employees.csv", "salary_configs.csv", "teams.csv", "working_hours.csv"};
const string LEVEL_NAMES[] = {"junior", "expert", "senior", "team_lead"};

class Expertise{
    public:
        Expertise();
        Expertise(LEVEL level, int baseSalary, int salaryPerHour, int SalaryPerExtraHour, int officialWorkingHours, int taxPercentage);
        void updateLevel(int level);
    private:
        LEVEL level;
        int baseSalary;
        int salaryPerHour;
        int salaryPerExtraHour;
        int officialWorkingHours;
        int taxPercentage;
};

Expertise::Expertise(){}

Expertise::Expertise(LEVEL level, int baseSalary, int salaryPerHour, int salaryPerExtraHour, int officialWorkingHours, int taxPercentage){
            this -> level = level;
            this -> baseSalary = baseSalary;
            this -> salaryPerHour = salaryPerHour;
            this -> salaryPerExtraHour = salaryPerExtraHour;
            this -> officialWorkingHours = officialWorkingHours;
            this -> taxPercentage = taxPercentage;
}

class Employee{
    public:
        Employee(int id, string name, int age, Expertise* expertise);
    private:
        int id;
        string name;
        int age;
        Expertise *expertise;
        int isWorking[DAYS_IN_MONTH][HOURS_IN_DAY];
};

Employee::Employee(int id, string name, int age, Expertise* expertise){
    this -> id = id;
    this -> name = name;
    this -> age = age;
    this -> expertise = expertise;
}

class Team{
    private:
        int id;
        int headId;
        vector<Employee*> members;
        int bonusMinWorkingHours;
        double bonusWorkingHoursVariance;
};

class PedarSahab{
    public:
        PedarSahab();
        Expertise *getExpertiseByInd(int ind);
        void addEmployee(int id, string name, int age, Expertise* expertise);
        void updateExpertise(int ind, LEVEL level, int baseSalary, int salaryPerHour, int salaryPerExtraHour, int officialWorkingHours, int taxPercentage){
            expertise[ind] = Expertise(level, baseSalary, salaryPerHour, salaryPerExtraHour, officialWorkingHours, taxPercentage);
        }
    private:
        vector<Employee*> employees;
        vector<Team*> teams;
        Expertise expertise[EXPERTISE_SIZE];
};

PedarSahab::PedarSahab(){

}

Expertise *PedarSahab::getExpertiseByInd(int ind){
    return &(expertise[ind]);
}

void PedarSahab::addEmployee(int id, string name, int age, Expertise *expertise){
    employees.push_back(new Employee(id, name, age, expertise));
}

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

int getExpertiseToken(string tokenStr){
    int token;
    if(tokenStr == LEVEL_NAMES[JUNIOR]){
        token = JUNIOR;
    }
    if(tokenStr == LEVEL_NAMES[EXPERT]){
        token = EXPERT;
    }
    if(tokenStr == LEVEL_NAMES[SENIOR]){
        token = SENIOR;
    }
    if(tokenStr == LEVEL_NAMES[TEAMLEAD]){
        token = TEAMLEAD;
    }
    return token;
}

void readEmployees(vector<vector<string>> employeeReadFile, PedarSahab &pedarSahab){
    for(int i = 0; i < (int)employeeReadFile.size(); i++){
        int id;
        string name;
        int age;
        Expertise *expertise;
        for (int j = 0; j < EMPLOYEE_COLUMN_SIZE; j++) {
            if(j == EMP_ID){
                id = stoi(employeeReadFile[i][j]);
            }
            if(j == EMP_NAME){
                name = employeeReadFile[i][j];
            }
            if(j == EMP_AGE){
                age = stoi(employeeReadFile[i][j]);
            }
            if(j == EMP_LEVEL){
                expertise = pedarSahab.getExpertiseByInd(getExpertiseToken(employeeReadFile[i][j]));
            }
        }
        pedarSahab.addEmployee(id, name, age, expertise);
    }
}

void readExpertise(vector<vector<string>> salaryConfigsReadFile, PedarSahab &pedarSahab){
    for(int i = 0; i < (int)salaryConfigsReadFile.size(); i++){
        LEVEL level;
        int baseSalary;
        int salaryPerHour;
        int salaryPerExtraHour;
        int officialWorkingHours;
        int taxPercentage;
        for(int j = 0; j < EXPERTISE_COLUMN_SIZE; j++){
            if(j == EXP_LEVEL){
                level = (LEVEL)getExpertiseToken(salaryConfigsReadFile[i][j]);
            }
            if(j == EXP_BASE_SALARY){
                baseSalary = stoi(salaryConfigsReadFile[i][j]);
            }
            if(j == EXP_SALARY_PER_HOUR){
                salaryPerHour = stoi(salaryConfigsReadFile[i][j]);
            }
            if(j == EXP_SALARY_PER_EXTRA_HOUR){
                salaryPerExtraHour = stoi(salaryConfigsReadFile[i][j]);
            }
            if(j == EXP_OFFICIAL_WORKING_HOURS){
                officialWorkingHours = stoi(salaryConfigsReadFile[i][j]);
            }
            if(j == EXP_TAX_PERCENTAGE){
                taxPercentage = stoi(salaryConfigsReadFile[i][j]);
            }
        }
        pedarSahab.updateExpertise(i, level, baseSalary, salaryPerHour, salaryPerExtraHour, officialWorkingHours, taxPercentage);
    }
}

PedarSahab readInput(){
    PedarSahab pedarSahab;

    for(int i = 0; i < FILE_SIZE; i++){
        if(i == EMPLOYEE_FILE){
            readEmployees(readCSV(ASSETS_FOLDER + FILE_NAMES[i]), pedarSahab);
        }
        if(i == SALARY_CONFIG_FILE){
            vector<vector<string>> salaryConfigsFile = readCSV(ASSETS_FOLDER + FILE_NAMES[i]);
            //handling
        }
        if (i == TEAMS_FILE){
            vector<vector<string>> teamsFile = readCSV(ASSETS_FOLDER + FILE_NAMES[i]);
            //handling
        }
        if (i == WORKING_HOURS_FILE){
            vector<vector<string>> workingHoursFile = readCSV(ASSETS_FOLDER + FILE_NAMES[i]);
            //handling
        }
    }
    return pedarSahab;
}

int main(int argc, char *argv[]) {

    return 0;
}
