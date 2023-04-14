#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <utility>
#include <cmath>

#define all(x) x.begin(), x.end()

#define FILE_SIZE 4
#define DAYS_IN_MONTH 30
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
#define CMD_SIZE 11
#define PERCENTAGE_AMOUNT 100.0
#define LINE_SEPERATOR "---"
#define NA -1
#define NA_STR "N/A"

enum FILE_NAME {SALARY_CONFIG_FILE, EMPLOYEE_FILE, TEAMS_FILE, WORKING_HOURS_FILE};
enum LEVEL {JUNIOR, EXPERT, SENIOR, TEAMLEAD};
enum EMPLOYEE_COLUMN {EMP_ID, EMP_NAME, EMP_AGE, EMP_LEVEL};
enum EXPERTISE_COLUMN {EXP_LEVEL, EXP_BASE_SALARY, EXP_SALARY_PER_HOUR, EXP_SALARY_PER_EXTRA_HOUR, EXP_OFFICIAL_WORKING_HOURS, EXP_TAX_PERCENTAGE};
enum TEAM_COLUMN {TEAM_ID, TEAM_HEAD_ID, TEAM_MEMBER_IDS, TEAM_BONUS_MIN_WORKING_HOURS, TEAM_BONUS_WORKING_HOURS_MAX_VARIANCE};
enum WORKIN_HOURS_COLUMN {WORK_H_EMPLOYEE_ID, WORK_H_DAY, WORK_H_WORKING_INTERVAL};
enum CMD_INSTANCE {REPORT_SALARIES, REPORT_EMPLOYEE_SALARY, REPORT_TEAM_SALARY, REPORT_TOTAL_HOURS_PER_DAY, REPORT_EMPLOYEE_PER_HOUR, SHOW_SALARY_CONFIG, UPDATE_SALARY_CONFIG, ADD_WORKING_HOURS, DELETE_WORKING_HOURS, UPDATE_TEAM_BONUS, FIND_TEAMS_FOR_BONUS};

using namespace std;
const string FILE_NAMES[] = {"salary_configs.csv" ,"employees.csv", "teams.csv", "working_hours.csv"};
const string LEVEL_NAMES[] = {"junior", "expert", "senior", "team_lead"};
const string CMD_NAMES[] = {"report_salaries", "report_employee_salary", "report_team_salary", "report_total_hours_per_day", "report_employee_per_hour", "show_salary_config", "update_salary_config", "add_working_hours", "delete_working_hours", "update_team_bonus", "find_teams_for_bonus"};

class Expertise{
    public:
        Expertise();
        Expertise(LEVEL _level, int _baseSalary, int _salaryPerHour, int _SalaryPerExtraHour, int _officialWorkingHours, int _taxPercentage);
        string getLevelName() { return LEVEL_NAMES[level]; };
        void outputExpertise();
        int calculateSalary(int hours);
        int calculateTax(int salary);
        int calculateEarning(int hours);
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

int Expertise::calculateSalary(int hours){
    int salary = baseSalary;
    salary += salaryPerHour * hours;
    salary += max(0, hours - officialWorkingHours) * (salaryPerExtraHour - salaryPerHour);
    return salary;
}

int Expertise::calculateTax(int salary){
    double result = salary * taxPercentage / PERCENTAGE_AMOUNT;
    return (int)round(result);
}

int Expertise::calculateEarning(int hours){
    int salary = calculateSalary(hours);
    double result = salary * (PERCENTAGE_AMOUNT - taxPercentage) / PERCENTAGE_AMOUNT;
    return (int)round(result);
}

void Expertise::outputExpertise(){
    cout << LEVEL_NAMES[level] << endl;
    cout << "Base salary: " << baseSalary << " Salary per hour: " << salaryPerHour << " Salary per extra hour: "
         << salaryPerExtraHour << " Official working hours: " << officialWorkingHours << " tax: " << taxPercentage << endl;
}

class Employee{
    public:
        Employee(int _id, string _name, int _age, Expertise* _expertise);
        int getId() { return id; }
        void updateWorkingHours(int day, int intervalStart, int intervalFinish);
        void outputEmployee();
        string getBriefInfo();
        string getTeamIdStr();
        string getFullInfo();
        void setTeamId(int _id) { teamId = _id; };
        int getAbsentDays();
    private:
        int id;
        string name;
        int age;
        Expertise *expertise;
        bool isWorking[DAYS_IN_MONTH][HOURS_IN_DAY];
        int teamId;
        int totalWorkingHours();
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
    teamId = NA;
}

int Employee::totalWorkingHours(){
    int total = 0;
    for(int i = 0; i < DAYS_IN_MONTH; i++){
        for(int j = 0; j < HOURS_IN_DAY; j++){
            total += isWorking[i][j];
        }
    }
    return total;
}

int Employee::getAbsentDays(){
    int presentDays = 0;
    for(int i = 0; i < DAYS_IN_MONTH; i++){
        for(int j = 0; j < HOURS_IN_DAY; j++){
            if(isWorking[i][j]){
                presentDays++;
                break;
            }
        }
    }
    return DAYS_IN_MONTH - presentDays;
}

void Employee::updateWorkingHours(int day, int intervalStart, int intervalFinish){
    for(int i = intervalStart; i < intervalFinish; i++){
        isWorking[day][i] = true;
    }
}

void Employee::outputEmployee(){
    cout << "Id and name: " << id << ' ' << name << " Age: " << age << " Expertise: " << expertise->getLevelName() << endl;
}

string Employee::getBriefInfo(){
    ostringstream output;
    output << "ID: " << id << endl;
    output << "Name: " << name << endl;
    output << "Total Working Hours: " << totalWorkingHours() << endl;
    output << "Total Earning: " << expertise -> calculateEarning(totalWorkingHours()) << endl;
    return output.str();
}

string Employee::getTeamIdStr(){
    if(teamId == NA){
        return NA_STR;
    }
    return to_string(teamId);
}

string Employee::getFullInfo(){
    ostringstream output;
    output << "ID: " << id << endl;
    output << "Name: " << name << endl;
    output << "Age: " << age << endl;
    output << "Level: " << expertise -> getLevelName() << endl;
    output << "Team ID: " << getTeamIdStr() << endl;
    output << "Total Working Hours: " << totalWorkingHours() << endl;
    output << "Absent Days: " << getAbsentDays() << endl;
    output << "Salary: " << expertise -> calculateSalary(totalWorkingHours()) << endl;
    output << "Bonus: " << 0 << endl;
    output << "Tax: " << expertise -> calculateTax(expertise -> calculateSalary(totalWorkingHours())) << endl;
    output << "Total Earning: " << expertise -> calculateEarning(totalWorkingHours()) << endl;
    return output.str();
}

Employee* findEmployeeById(int id, const vector<Employee*> &employees, const set< pair<int, int> > &employeeInd){
    set< pair<int, int> >::iterator indPair = lower_bound(all(employeeInd), make_pair(id, 0));
    if(indPair -> first == id){
        return employees[indPair -> second];
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
        set< pair<int, int> > memberInd;
        int bonusMinWorkingHours;
        double bonusWorkingHoursMaxVariance;
        int bonus;
};

Team::Team(int _id, Employee* _head, vector<Employee*> _members, int _bonusMinWorkingHours, double _bonusWorkingHoursMaxVariance){
    id = _id;
    head = _head;
    members = _members;
    for(int i = 0; i < (int)members.size(); i++){
        memberInd.insert(make_pair(members[i] -> getId(), i));
        members[i] -> setTeamId(id);
    }
    bonusMinWorkingHours = _bonusMinWorkingHours;
    bonusWorkingHoursMaxVariance = _bonusWorkingHoursMaxVariance;
    bonus = 0;
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

string trimBoth(string str){
    int stPos = 0, fnPos = (int)str.size();
    for(int i = 0; i < (int)str.size(); i++){
        if(!isspace(str[i])){
            stPos = i;
            break;
        }
    }
    for(int i = (int)str.size() - 1; ~i; i--){
        if(!isspace(str[i])){
            fnPos = i + 1;
            break;
        }
    }
    return str.substr(stPos, fnPos - stPos);
}

string wordFromPosition(string::iterator &it, string::iterator fn) {
    while (isspace(*(it++)))
        ;
    --it;
    string::iterator i = it;
    string res = "";
    for (; i != fn; ++i) {
        if (isspace(*i)) {
            break;
        }
        res.push_back(*i);
    }
    swap(i, it);
    return res;
}

vector<string> splitIntoWords(string str){
    vector<string> elements;
    string::iterator it = str.begin();
    while(it != str.end()){
        elements.push_back(wordFromPosition(it, str.end()));
    }
    return elements;
}

vector<string> splitString(string str, char delim){
     vector<string> elements;
     istringstream strStream(str);
     string element;

     while(getline(strStream, element, delim)){
         elements.push_back(trimBoth(element));
     }

     return elements;
}

class PedarSahab{
    public:
        PedarSahab();
        Expertise *getExpertiseByInd(int ind);
        void addEmployee(string id, string name, string age, string expertise);
        void updateExpertise(string level, string baseSalary, string salaryPerHour, string salaryPerExtraHour, string officialWorkingHours, string taxPercentage);
        void addTeam(string teamId, string teamHeadId, string memberIds, string bonusMinWorkingHours, string bonusWorkingHoursMaxVariance);
        void updateEmployeeWorkingDay(string id, string day, string workInterval);
        void outputPedarSahab();
        string reportSalaries();
        string reportEmployeeSalary(string idStr);
        void freeMemory();
    private:
        vector<Employee*> employees;
        set< pair<int, int> > employeeInd;
        vector<Team*> teams;
        set< pair<int, int> > teamInd;
        Expertise expertise[EXPERTISE_SIZE];
};

PedarSahab::PedarSahab(){}

Expertise *PedarSahab::getExpertiseByInd(int ind){
    return &(expertise[ind]);
}

void PedarSahab::addEmployee(string id, string name, string age, string expertise){
    employees.push_back(new Employee(stoi(id), name, stoi(age), getExpertiseByInd(getExpertiseToken(expertise))));
    employeeInd.insert(make_pair(stoi(id), (int)employees.size() - 1));
}

void PedarSahab::updateExpertise(string level, string baseSalary, string salaryPerHour, string salaryPerExtraHour, string officialWorkingHours, string taxPercentage){
    expertise[getExpertiseToken(level)] = Expertise((LEVEL)getExpertiseToken(level), stoi(baseSalary), stoi(salaryPerHour), stoi(salaryPerExtraHour), stoi(officialWorkingHours), stoi(taxPercentage));
}

void PedarSahab::addTeam(string teamId, string teamHeadId, string memberIds, string bonusMinWorkingHours, string bonusWorkingHoursMaxVariance){
    int id = stoi(teamId);
    Employee* head = findEmployeeById(stoi(teamHeadId), employees, employeeInd);
    vector<Employee*> members;
    vector<string> memberIdList = splitString(memberIds, MEMBER_ID_DELIM);
    for(int i = 0; i < (int)memberIdList.size(); i++){
        members.push_back(findEmployeeById(stoi(memberIdList[i]), employees, employeeInd));
    }
    teams.push_back(new Team(id, head, members, stoi(bonusMinWorkingHours), stof(bonusWorkingHoursMaxVariance)));
}

void PedarSahab::updateEmployeeWorkingDay(string id, string day, string workInterval){
    Employee* employee = findEmployeeById(stoi(id), employees, employeeInd);
    vector<string> intervalStr = splitString(workInterval, WORK_INTERVAL_DELIM);
    employee -> updateWorkingHours(stoi(day) - 1, stoi(intervalStr[0]), stoi(intervalStr[1]));
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

string PedarSahab::reportSalaries(){
    ostringstream output;
    for(set< pair<int, int> >::iterator i = employeeInd.begin(); i != employeeInd.end(); ++i){
        output << employees[i -> second] -> getBriefInfo() << LINE_SEPERATOR << endl;
    }
    return output.str();
}

string PedarSahab::reportEmployeeSalary(string idStr){
    ostringstream output;
    Employee* emp = findEmployeeById(stoi(idStr), employees, employeeInd);
    if(emp == NULL){
        output << "EMPLOYEE_NOT_FOUNT" << endl;
        return output.str();
    }
    output << emp -> getFullInfo();
    return output.str();
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
    vector<vector<string>> readFile;
    string lineOfFile;

    while(getline(inputFile, lineOfFile)){
        readFile.push_back(splitString(lineOfFile, CSV_DELIM));
    }

    inputFile.close();
    return readFile;
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

void handleCommand(string cmdLine, PedarSahab &pedarSahab){
    vector<string> cmdWords = splitIntoWords(cmdLine);
    int cmdInd = CMD_SIZE;
    for(int i = 0; i < CMD_SIZE; i++){
        if(cmdWords[0] == CMD_NAMES[i]){
            cmdInd = i;
            break;
        }
    }

    if(cmdInd == REPORT_SALARIES){
        cout << pedarSahab.reportSalaries();
    }
    if(cmdInd == REPORT_EMPLOYEE_SALARY){
        cout << pedarSahab.reportEmployeeSalary(cmdWords[1]);
    }
    cout << "IND: " << cmdInd << endl;
}

int main(int argc, char *argv[]) {
    PedarSahab pedarSahab;
    if(argc == 2){
        readInput((string)argv[1] + DIRECTORY_DELIM, pedarSahab);
    }
    pedarSahab.outputPedarSahab();
    string cmdLine;
    while(getline(cin, cmdLine)){
        handleCommand(cmdLine, pedarSahab);
    }
    pedarSahab.freeMemory();
    return 0;
}
