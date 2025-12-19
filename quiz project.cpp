/******************************************************************************************
*                               QUIZ MANAGEMENT SYSTEM                                   *
*-----------------------------------------------------------------------------------------*
* Language   : C++                                                                         *
* Type       : Console Based Application                                                   *
* Description:                                                                             *
*   This program is a fully functional quiz system that allows users to:                  *
*   - Attempt quizzes from multiple subjects                                               *
*   - Get instant feedback on answers                                                      *
*   - View final report card                                                               *
*   - Save scores permanently using file handling                                          *
*   - Maintain a leaderboard with sorting                                                  *
*                                                                                          *
* Concepts Used:                                                                           *
*   - Structures                                                                           *
*   - Arrays                                                                               *
*   - Vectors                                                                              *
*   - File Handling                                                                        *
*   - Functions                                                                            *
*   - Sorting Algorithms                                                                   *
*   - Input Validation                                                                     *
*                                                                                          *
******************************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

/******************************************************************************************
                                    STRUCT DEFINITIONS
******************************************************************************************/

struct Player
{
    string name;
    int score;
};

struct Question
{
    string text;
    string options[4];
    int correctOption;
};

struct Subject
{
    string name;
    Question questions[5];
};

/******************************************************************************************
                                    GLOBAL CONSTANTS
******************************************************************************************/

const string FILE_NAME = "scores.txt";
const int TOTAL_SUBJECTS = 4;
const int QUESTIONS_PER_SUBJECT = 5;

/******************************************************************************************
                                    FUNCTION PROTOTYPES
******************************************************************************************/

void loadLeaderboard(vector<Player>& leaderboard);
void saveLeaderboard(const vector<Player>& leaderboard);
void updateLeaderboard(vector<Player>& leaderboard, string name, int score);

void showWelcomeScreen();
void showRules();
void showLeaderboardOnly(const vector<Player>& leaderboard);

bool askQuestion(const Question& q);
int runSubjectQuiz(const Subject& subj);
int showMainMenu(Subject subjects[], int n);

/******************************************************************************************
                                    FILE HANDLING
******************************************************************************************/

void loadLeaderboard(vector<Player>& leaderboard) {
    leaderboard.clear();

    ifstream inFile(FILE_NAME);

    if (inFile.is_open()) {
        string n;
        int s;
        while (inFile >> n >> s) {
            leaderboard.push_back({ n, s });
        }
        inFile.close();
    }

    if (leaderboard.empty()) {
        leaderboard.push_back({ "AI_MASTER", 18 });
        leaderboard.push_back({ "QUIZ_BOT", 15 });
        leaderboard.push_back({ "NOOB_BOT", 10 });
        leaderboard.push_back({ "GUEST", 5 });
    }
}

void saveLeaderboard(const vector<Player>& leaderboard) {
    ofstream outFile(FILE_NAME);

    if (outFile.is_open()) {
        for (auto p : leaderboard) {
            outFile << p.name << " " << p.score << endl;
        }
        outFile.close();
    }
}

void updateLeaderboard(vector<Player>& leaderboard, string name, int score) {

    leaderboard.push_back({ name, score });

    for (int i = 0; i < leaderboard.size(); i++) {
        for (int j = i + 1; j < leaderboard.size(); j++)
        {
            if (leaderboard[j].score > leaderboard[i].score)
            {
                Player temp = leaderboard[i];
                leaderboard[i] = leaderboard[j];
                leaderboard[j] = temp;
            }
        }
    }

    if (leaderboard.size() > 5)
        leaderboard.resize(5);

    saveLeaderboard(leaderboard);
}

/******************************************************************************************
                                    UI FUNCTIONS
******************************************************************************************/

void showWelcomeScreen() {
    cout << "\n====================================================\n";
    cout << "        WELCOME TO QUIZ MANAGEMENT SYSTEM           \n";
    cout << "====================================================\n";
    cout << "Test your knowledge and climb the leaderboard!\n";
}

void showRules()
{
    cout << "\n-------------------- RULES -------------------------\n";
    cout << "1. Each subject has 5 questions\n";
    cout << "2. Each correct answer gives 1 point\n";
    cout << "3. No negative marking\n";
    cout << "4. Each subject can be attempted once\n";
    cout << "5. Final score will be saved\n";
    cout << "----------------------------------------------------\n";
}

void showLeaderboardOnly(const vector<Player>& leaderboard)
{
    cout << "\n============= LEADERBOARD =============\n";
    for (int i = 0; i < leaderboard.size(); i++)
    {
        cout << i + 1 << ". " << leaderboard[i].name
            << "  --->  " << leaderboard[i].score << endl;
    }
    cout << "=======================================\n";
}

/******************************************************************************************
                                    QUIZ FUNCTIONS
******************************************************************************************/

bool askQuestion(const Question& q)
{

    cout << "\n-------------------------------------------------\n";
    cout << q.text << endl;
    cout << "-------------------------------------------------\n";

    for (int i = 0; i < 4; i++)
    {
        cout << i + 1 << ". " << q.options[i] << endl;
    }

    int ans;
    cout << "Enter answer (1-4): ";
    cin >> ans;

    if (cin.fail() || ans < 1 || ans > 4)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Answer counted as wrong.\n";
        return false;
    }

    if (ans - 1 == q.correctOption)
    {
        cout << "Correct!\n";
        return true;
    }

    cout << "Wrong! Correct Answer: "
        << q.options[q.correctOption] << endl;
    return false;
}

int runSubjectQuiz(const Subject& subj)
{

    cout << "\n*********** SUBJECT: " << subj.name << " ***********\n";

    int score = 0;

    for (int i = 0; i < QUESTIONS_PER_SUBJECT; i++) {
        if (askQuestion(subj.questions[i]))
            score++;
    }

    cout << "Subject Completed. Score: "
        << score << "/" << QUESTIONS_PER_SUBJECT << endl;

    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();

    return score;
}

/******************************************************************************************
                                    MAIN MENU
******************************************************************************************/

int showMainMenu(Subject subjects[], int n)
{

    cout << "\n================ MAIN MENU ================\n";

    for (int i = 0; i < n; i++)
    {
        cout << i + 1 << ". " << subjects[i].name << endl;
    }

    cout << n + 1 << ". View Rules\n";
    cout << n + 2 << ". View Leaderboard\n";
    cout << n + 3 << ". Exit & Report\n";

    cout << "Choose option: ";
    int choice;
    cin >> choice;

    return choice;
}

/******************************************************************************************
                                    MAIN FUNCTION
******************************************************************************************/

int main()
{

    vector<Player> leaderboard;
    loadLeaderboard(leaderboard);

    Subject subjects[TOTAL_SUBJECTS] =
    {
        {"Math", {
            {"2+2 = ?", {"1","2","3","4"}, 3},
            {"5*6 = ?", {"11","30","25","20"}, 1},
            {"10/2 = ?", {"2","4","5","10"}, 2},
            {"7-3 = ?", {"2","3","4","5"}, 2},
            {"Square root of 16 = ?", {"2","4","6","8"}, 1}
                }
        },
        {"Science",
            {
            {"What is the Water formula?", {"H2O","CO2","O2","NaCl"}, 0},
            {"Which planet is called Red Planet?", {"Earth","Mars","Venus","Jupiter"}, 1},
            {"The Gas we breathe?", {"CO2","O2","N2","H2"}, 1},
            {"How many chambers a heart have?", {"2","3","4","5"}, 2},
            {"What is the Speed of light?", {"3x10^8","3x10^6","300","None"}, 0}
           }
        },
        {"History",
            {
            {"America was discovered by?", {"Columbus","Newton","Einstein","Edison"}, 0},
            {"who First US President?", {"Lincoln","Washington","Jefferson","Adams"}, 1},
            {"World War 2 ended in?", {"1940","1945","1950","1939"}, 1},
            {"Great Wall is situated in?", {"India","China","Japan","Korea"}, 1},
            {"Pyramids are situated in?", {"Mexico","Egypt","Peru","Iraq"}, 1}
           }
        },
        {"Computer",
            {
            {"CPU stands for?", {"Central Processing Unit","Personal","Control","None"}, 0},
            {"Binary base is?", {"2","8","10","16"}, 0},
            {"Microsoft founder is ?", {"Jobs","Gates","Zuckerberg","Musk"}, 1},
            {"HTML is a: ?", {"Design","Structure","DB","Net"}, 1},
            {"C++ was designed by?", {"Bjarne","Dennis","James","Guido"}, 0}
           }
        }
    };

    int scores[TOTAL_SUBJECTS] = { 0 };
    bool attempted[TOTAL_SUBJECTS] = { false };
    bool played = false;

    showWelcomeScreen();

    while (true)
    {

        int choice = showMainMenu(subjects, TOTAL_SUBJECTS);

        if (choice >= 1 && choice <= TOTAL_SUBJECTS)
        {
            if (!attempted[choice - 1])
            {
                scores[choice - 1] = runSubjectQuiz(subjects[choice - 1]);
                attempted[choice - 1] = true;
                played = true;
            }
            else
            {
                cout << "Subject already attempted!\n";
            }
        }
        else if (choice == TOTAL_SUBJECTS + 1)
        {
            showRules();
        }
        else if (choice == TOTAL_SUBJECTS + 2)
        {
            showLeaderboardOnly(leaderboard);
        }
        else if (choice == TOTAL_SUBJECTS + 3)
        {
            break;
        }
        else
        {
            cout << "Invalid choice!\n";
        }
    }

    int total = 0;
    for (int i = 0; i < TOTAL_SUBJECTS; i++)
        total += scores[i];

    cout << "\nFINAL SCORE: " << total << "/20\n";

    if (played)
    {
        string name;
        cout << "Enter your name: ";
        cin >> name;
        updateLeaderboard(leaderboard, name, total);
    }

    showLeaderboardOnly(leaderboard);

    cout << "\nThank you for using the Quiz System!\n";
    return 0;
}