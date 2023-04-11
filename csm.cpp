#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

#define all(x) x.begin(), x.end()

#define FILE_SIZE 4
#define DAYS_IN_MONTH 30
#define HOURS_IN_DAY 24
#define DIRECTORY_DELIM "/"
#define CSV_DELIM ','
#define EXPERTISE_SIZE 4
#define EXPERTISE_COLUMN_SIZE 6
#define EMPLOYEE_COLUMN_SIZE 4
#define TEAM_COLUMN_SIZE 5

enum FILE_NAME {SALARY_CONFIG_FILE, EMPLOYEE_FILE, TEAMS_FILE, WORKING_HOURS_FILE};
enum LEVEL {JUNIOR, EXPERT, SENIOR, TEAMLEAD};
enum EMPLOYEE_COLUMN {EMP_ID, EMP_NAME, EMP_AGE, EMP_LEVEL};
enum EXPERTISE_COLUMN {EXP_LEVEL, EXP_BASE_SALARY, EXP_SALARY_PER_HOUR, EXP_SALARY_PER_EXTRA_HOUR, EXP_OFFICIAL_WORKING_HOURS, EXP_TAX_PERCENTAGE};
enum TEAM_COLUMN {TEAM_ID, TEAM_HEAD_ID, TEAM_MEMBER_IDS, TEAM_BONUS_MIN_WORKING_HOURS, TEAM_BONUS_WORKING_HOURS_MAX_VARIANCE};

using namespace std;
const string FILE_NAMES[] = {"salary_configs.csv" ,"employees.csv", "teams.csv", "working_hours.csv"};
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
        bool isIdLessThan(Employee b);
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

bool Employee::isIdLessThan(Employee b){
    return (this -> id) < b.id;
}

bool employeeCmpById(Employee* a, Employee* b){
    return a -> isIdLessThan(*b);
}

class Team{
    public:
        Team(int id, Employee* head, vector<Employee*> members, int bonusMinWorkingHours, double bonusWorkingHoursMaxVariance);
    private:
        int id;
        Employee* head;
        vector<Employee*> members;
        int bonusMinWorkingHours;
        double bonusWorkingHoursMaxVariance;
};


Team::Team(int id, Employee* head, vector<Employee*> members, int bonusMinWorkingHours, double bonusWorkingHoursMaxVariance){
    this -> id = id;
    this -> head = head;
    this -> members = members;
    this -> bonusMinWorkingHours = bonusMinWorkingHours;
    this -> bonusWorkingHoursMaxVariance = bonusWorkingHoursMaxVariance;
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

class PedarSahab{
    public:
        PedarSahab();
        Expertise *getExpertiseByInd(int ind);
        void sortEmployeesList();
        void addEmployee(string id, string name, string age, string expertise);
        void updateExpertise(string level, string baseSalary, string salaryPerHour, string salaryPerExtraHour, string officialWorkingHours, string taxPercentage);
    private:
        vector<Employee*> employees;
        vector<Team*> teams;
        Expertise expertise[EXPERTISE_SIZE];
};

PedarSahab::PedarSahab(){}

Expertise *PedarSahab::getExpertiseByInd(int ind){
    return &(expertise[ind]);
}

void PedarSahab::sortEmployeesList(){
    sort(all(employees), employeeCmpById);
}

void PedarSahab::addEmployee(string id, string name, string age, string expertise){
    employees.push_back(new Employee(stoi(id), name, stoi(age), getExpertiseByInd(getExpertiseToken(expertise))));
    this -> sortEmployeesList();
}

void PedarSahab::updateExpertise(string level, string baseSalary, string salaryPerHour, string salaryPerExtraHour, string officialWorkingHours, string taxPercentage){
    expertise[getExpertiseToken(level)] = Expertise((LEVEL)getExpertiseToken(level), stoi(baseSalary), stoi(salaryPerHour), stoi(salaryPerExtraHour), stoi(officialWorkingHours), stoi(taxPercentage));
}

vector<string> splitString(string str, char delim){
     vector<string> elements;
     istringstream strStream(str);
     string element;
     while(getline(strStream, element, delim)){
         elements.push_back(element);
     }
     return elements;
}

vector<vector<string>> readCSV(string fileName){
   ifstream inputFile(fileName);
   vector<vector<string>> readedFile;
   string lineOfFile;

   while(getline(inputFile, lineOfFile)){
       readedFile.push_back(splitString(lineOfFile, CSV_DELIM));
   }

   inputFile.close();
   return readedFile;
}

void readEmployees(vector<vector<string>> employeeReadFile, PedarSahab &pedarSahab){
    for(int i = 0; i < (int)employeeReadFile.size(); i++){
        string id;
        string name;
        string age;
        string expertise;
        for (int j = 0; j < EMPLOYEE_COLUMN_SIZE; j++) {
            if(j == EMP_ID){
                id = employeeReadFile[i][j];
            }
            if(j == EMP_NAME){
                name = employeeReadFile[i][j];
            }
            if(j == EMP_AGE){
                age = employeeReadFile[i][j];
            }
            if(j == EMP_LEVEL){
                expertise = employeeReadFile[i][j];
            }
        }
        pedarSahab.addEmployee(id, name, age, expertise);
    }
}

void readExpertise(vector<vector<string>> salaryConfigsReadFile, PedarSahab &pedarSahab){
    for(int i = 0; i < (int)salaryConfigsReadFile.size(); i++){
        string level;
        string baseSalary;
        string salaryPerHour;
        string salaryPerExtraHour;
        string officialWorkingHours;
        string taxPercentage;
        for(int j = 0; j < EXPERTISE_COLUMN_SIZE; j++){
            if(j == EXP_LEVEL){
                level = salaryConfigsReadFile[i][j];
            }
            if(j == EXP_BASE_SALARY){
                baseSalary = salaryConfigsReadFile[i][j];
            }
            if(j == EXP_SALARY_PER_HOUR){
                salaryPerHour = salaryConfigsReadFile[i][j];
            }
            if(j == EXP_SALARY_PER_EXTRA_HOUR){
                salaryPerExtraHour = salaryConfigsReadFile[i][j];
            }
            if(j == EXP_OFFICIAL_WORKING_HOURS){
                officialWorkingHours = salaryConfigsReadFile[i][j];
            }
            if(j == EXP_TAX_PERCENTAGE){
                taxPercentage = salaryConfigsReadFile[i][j];
            }
        }
        pedarSahab.updateExpertise(level, baseSalary, salaryPerHour, salaryPerExtraHour, officialWorkingHours, taxPercentage);
    }
}

void readTeams(vector<vector<string>> teamsReadFile, PedarSahab &pedarSahab){

}

PedarSahab readInput(string assetsFolder){
    PedarSahab pedarSahab;

    for(int i = 0; i < FILE_SIZE; i++){
        if(i == EMPLOYEE_FILE){
            readEmployees(readCSV(assetsFolder + FILE_NAMES[i]), pedarSahab);
        }
        if(i == SALARY_CONFIG_FILE){
            readExpertise(readCSV(assetsFolder + FILE_NAMES[i]), pedarSahab);
        }
        if (i == TEAMS_FILE){
            vector<vector<string>> teamsFile = readCSV(assetsFolder + FILE_NAMES[i]);
            //handling
        }
        if (i == WORKING_HOURS_FILE){
            vector<vector<string>> workingHoursFile = readCSV(assetsFolder + FILE_NAMES[i]);
            //handling
        }
    }
    return pedarSahab;
}

int main(int argc, char *argv[]) {
    readInput((string)*argv + DIRECTORY_DELIM);
    return 0;
}
