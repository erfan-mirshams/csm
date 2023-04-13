#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

#define all(x) x.begin(), x.end()

#define FILE_SIZE 4
#define DAYS_IN_MONTH 31
#define HOURS_IN_DAY 24
#define DIRECTORY_DELIM "/"
#define CSV_DELIM ','
#define MEMBER_ID_DELIM '$'
#define WORK_INTERVAL_DELIM '-'
#define EXPERTISE_SIZE 4
#define EXPERTISE_COLUMN_SIZE 6
#define EMPLOYEE_COLUMN_SIZE 4
#define TEAM_COLUMN_SIZE 5
#define WORKING_HOURS_COLUMN_SIZE 3
#define SKIPPED_LINE_CNT 1

enum FILE_NAME {SALARY_CONFIG_FILE, EMPLOYEE_FILE, TEAMS_FILE, WORKING_HOURS_FILE};
enum LEVEL {JUNIOR, EXPERT, SENIOR, TEAMLEAD};
enum EMPLOYEE_COLUMN {EMP_ID, EMP_NAME, EMP_AGE, EMP_LEVEL};
enum EXPERTISE_COLUMN {EXP_LEVEL, EXP_BASE_SALARY, EXP_SALARY_PER_HOUR, EXP_SALARY_PER_EXTRA_HOUR, EXP_OFFICIAL_WORKING_HOURS, EXP_TAX_PERCENTAGE};
enum TEAM_COLUMN {TEAM_ID, TEAM_HEAD_ID, TEAM_MEMBER_IDS, TEAM_BONUS_MIN_WORKING_HOURS, TEAM_BONUS_WORKING_HOURS_MAX_VARIANCE};
enum WORKIN_HOURS_COLUMN {WORK_H_EMPLOYEE_ID, WORK_H_DAY, WORK_H_WORKING_INTERVAL};

using namespace std;
const string FILE_NAMES[] = {"salary_configs.csv" ,"employees.csv", "teams.csv", "working_hours.csv"};
const string LEVEL_NAMES[] = {"junior", "expert", "senior", "team_lead"};

class Expertise{
    public:
        Expertise();
        Expertise(LEVEL _level, int _baseSalary, int _salaryPerHour, int _SalaryPerExtraHour, int _officialWorkingHours, int _taxPercentage);
        string getLevelName();
        void outputExpertise();
    private:
        LEVEL level;
        int baseSalary;
        int salaryPerHour;
        int salaryPerExtraHour;
        int officialWorkingHours;
        int taxPercentage;
};

Expertise::Expertise(){}

Expertise::Expertise(LEVEL _level, int _baseSalary, int _salaryPerHour, int _salaryPerExtraHour, int _officialWorkingHours, int _taxPercentage){
    level = _level;
    baseSalary = _baseSalary;
    salaryPerHour = _salaryPerHour;
    salaryPerExtraHour = _salaryPerExtraHour;
    officialWorkingHours = _officialWorkingHours;
    taxPercentage = _taxPercentage;
}

string Expertise::getLevelName(){
    return LEVEL_NAMES[level];
}

void Expertise::outputExpertise(){
    cout << LEVEL_NAMES[level] << endl;
    cout << "Base salary: " << baseSalary << " Salary per hour: " << salaryPerHour << " Salary per extra hour: "
         << salaryPerExtraHour << " Official working hours: " << officialWorkingHours << " tax: " << taxPercentage << endl;
}

class Employee{
    public:
        Employee(int _id, string _name, int _age, Expertise* _expertise);
        bool isIdLessThan(Employee b);
        int getId();
        void updateWorkingHours(int day, int intervalStart, int intervalFinish);
        void outputEmployee();
    private:
        int id;
        string name;
        int age;
        Expertise *expertise;
        bool isWorking[DAYS_IN_MONTH][HOURS_IN_DAY];
};

Employee::Employee(int _id, string _name, int _age, Expertise* _expertise){
    id = _id;
    name = _name;
    age = _age;
    expertise = _expertise;
    for(int i = 0; i < DAYS_IN_MONTH; i++){
        for(int j = 0; j < HOURS_IN_DAY; j++){
            isWorking[i][j] = false;
        }
    }
}

bool Employee::isIdLessThan(Employee b){
    return (this -> id) < b.id;
}

int Employee::getId(){
    return id;
}

void Employee::updateWorkingHours(int day, int intervalStart, int intervalFinish){
    for(int i = intervalStart; i < intervalFinish; i++){
        isWorking[day][i] = true;
    }
}

void Employee::outputEmployee(){
    cout << "Id and name: " << id << ' ' << name << " Age: " << age << " Expertise: " << expertise->getLevelName() << endl;
}

bool employeeCmpById(Employee* a, Employee* b){
    return a -> isIdLessThan(*b);
}

Employee* findEmployeeById(int id, const vector<Employee*> &employees){
    int l, r, mid;
    l = 0;
    r = (int)employees.size();
    while(r - l > 1){
        mid = (l + r) / 2;
        if(employees[mid] -> getId() <= id){
            l = mid;
        }
        else{
            r = mid;
        }
    }
    if(employees[l] -> getId() == id){
        return employees[l];
    }
    return NULL;
}

class Team{
    public:
        Team(int _id, Employee* _head, vector<Employee*> _members, int _bonusMinWorkingHours, double _bonusWorkingHoursMaxVariance);
        void outputTeam();
    private:
        int id;
        Employee* head;
        vector<Employee*> members;
        int bonusMinWorkingHours;
        double bonusWorkingHoursMaxVariance;
};

Team::Team(int _id, Employee* _head, vector<Employee*> _members, int _bonusMinWorkingHours, double _bonusWorkingHoursMaxVariance){
    id = _id;
    head = _head;
    members = _members;
    sort(all(members), employeeCmpById);
    bonusMinWorkingHours = _bonusMinWorkingHours;
    bonusWorkingHoursMaxVariance = _bonusWorkingHoursMaxVariance;
}

void Team::outputTeam(){
    cout << "Team id: " << id << "Head id: " << head -> getId() << endl;
    cout << "And there are " << (int)members.size() << " members which are:" << endl;
    for (auto member : members)
        cout << member -> getId() << ' ';
    cout << "Minimum working hours for getting bonus and variance thing: " << bonusMinWorkingHours << ' ' <<
        bonusWorkingHoursMaxVariance << endl;
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

vector<string> splitString(string str, char delim){
     vector<string> elements;
     istringstream strStream(str);
     string element;

     while(getline(strStream, element, delim)){
         elements.push_back(element);
     }

     return elements;
}

class PedarSahab{
    public:
        PedarSahab();
        Expertise *getExpertiseByInd(int ind);
        void sortEmployeesList();
        void addEmployee(string id, string name, string age, string expertise);
        void updateExpertise(string level, string baseSalary, string salaryPerHour, string salaryPerExtraHour, string officialWorkingHours, string taxPercentage);
        void addTeam(string teamId, string teamHeadId, string memberIds, string bonusMinWorkingHours, string bonusWorkingHoursMaxVariance);
        void updateEmployeeWorkingDay(string id, string day, string workInterval);
        void outputPedarSahab();
        void freeMemory();
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
}

void PedarSahab::updateExpertise(string level, string baseSalary, string salaryPerHour, string salaryPerExtraHour, string officialWorkingHours, string taxPercentage){
    expertise[getExpertiseToken(level)] = Expertise((LEVEL)getExpertiseToken(level), stoi(baseSalary), stoi(salaryPerHour), stoi(salaryPerExtraHour), stoi(officialWorkingHours), stoi(taxPercentage));
}

void PedarSahab::addTeam(string teamId, string teamHeadId, string memberIds, string bonusMinWorkingHours, string bonusWorkingHoursMaxVariance){
    int id = stoi(teamId);
    Employee* head = findEmployeeById(stoi(teamHeadId), employees);
    vector<Employee*> members;
    vector<string> memberIdList = splitString(memberIds, MEMBER_ID_DELIM);
    for(int i = 0; i < (int)memberIdList.size(); i++){
        members.push_back(findEmployeeById(stoi(memberIdList[i]), employees));
    }
    teams.push_back(new Team(id, head, members, stoi(bonusMinWorkingHours), stof(bonusWorkingHoursMaxVariance)));
}

void PedarSahab::updateEmployeeWorkingDay(string id, string day, string workInterval){
    Employee* employee = findEmployeeById(stoi(id), employees);
    vector<string> intervalStr = splitString(workInterval, WORK_INTERVAL_DELIM);
    employee -> updateWorkingHours(stoi(day), stoi(intervalStr[0]), stoi(intervalStr[1]));
}

void PedarSahab::outputPedarSahab(){
    cout << "Epertise: " << endl;
    for (int i = 0; i < EXPERTISE_SIZE; ++i)
        expertise[i].outputExpertise();
    cout << endl;
    
    cout << "Employees: " << endl;
    for (int i = 0; i < (int)employees.size(); ++i)
        employees[i] -> outputEmployee();
    cout << endl;
    
    cout << "Teams: " << endl;
    for (int i = 0; i < (int)teams.size(); ++i)
        teams[i] -> outputTeam();
    cout << endl;
}

void PedarSahab::freeMemory(){
    for(int i = 0; i < (int)employees.size(); i++){
        delete employees[i];
    }
    for(int i = 0; i < (int)teams.size(); i++){
        delete teams[i];
    }
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
    for(int i = SKIPPED_LINE_CNT; i < (int)employeeReadFile.size(); i++){
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
    for(int i = SKIPPED_LINE_CNT; i < (int)salaryConfigsReadFile.size(); i++){
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
    for(int i = SKIPPED_LINE_CNT; i < (int)teamsReadFile.size(); i++){
        string id;
        string headId;
        string memberIds;
        string bonusMinWorkingHours;
        string BonusWorkingHoursMaxVariance;
        for(int j = 0; j < TEAM_COLUMN_SIZE; j++){
            if(j == TEAM_ID){
                id = teamsReadFile[i][j];
            }
            if(j == TEAM_HEAD_ID){
                headId = teamsReadFile[i][j];
            }
            if(j == TEAM_MEMBER_IDS){
                memberIds = teamsReadFile[i][j];
            }
            if(j == TEAM_BONUS_MIN_WORKING_HOURS){
                bonusMinWorkingHours = teamsReadFile[i][j];
            }
            if(j == TEAM_BONUS_WORKING_HOURS_MAX_VARIANCE){
                BonusWorkingHoursMaxVariance = teamsReadFile[i][j];
            }
        }
        pedarSahab.addTeam(id, headId, memberIds, bonusMinWorkingHours, BonusWorkingHoursMaxVariance);
    }
}

void readWorkingHours(vector<vector<string>> workingHoursReadFile, PedarSahab &pedarSahab){
    for(int i = SKIPPED_LINE_CNT; i < (int)workingHoursReadFile.size(); i++){
        string id;
        string day;
        string workingInterval;
        for(int j = 0; j < WORKING_HOURS_COLUMN_SIZE; j++){
            if(j == WORK_H_EMPLOYEE_ID){
                id = workingHoursReadFile[i][j];
            }
            if(j == WORK_H_DAY){
                day = workingHoursReadFile[i][j];
            }
            if(j == WORK_H_WORKING_INTERVAL){
                workingInterval = workingHoursReadFile[i][j];
            }
        }
        pedarSahab.updateEmployeeWorkingDay(id, day, workingInterval);
    }
}

void readInput(string assetsFolder, PedarSahab &pedarSahab){
    for(int i = 0; i < FILE_SIZE; i++){
        if(i == EMPLOYEE_FILE){
            readEmployees(readCSV(assetsFolder + FILE_NAMES[i]), pedarSahab);
            pedarSahab.sortEmployeesList();
        }
        if(i == SALARY_CONFIG_FILE){
            readExpertise(readCSV(assetsFolder + FILE_NAMES[i]), pedarSahab);
        }
        if (i == TEAMS_FILE){
            readTeams(readCSV(assetsFolder + FILE_NAMES[i]), pedarSahab);
        }
        if (i == WORKING_HOURS_FILE){
            readWorkingHours(readCSV(assetsFolder + FILE_NAMES[i]), pedarSahab);
        }
    }
}

int main(int argc, char *argv[]) {
    PedarSahab pedarSahab;
    if(argc == 2){
        readInput((string)*(argv+1) + DIRECTORY_DELIM, pedarSahab);
    }
    pedarSahab.outputPedarSahab();

    pedarSahab.freeMemory();
    return 0;
}
