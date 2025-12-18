#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

struct Player {
    string name;
    int score;
};

const string FILE_NAME = "scores.txt";

// --- HELPER FUNCTIONS ---

Player makePlayer(string name, int score)
{
    Player p;
    p.name = name;
    p.score = score;
    return p;
}

void loadLeaderboard(vector<Player>& leaderboard)
{
    leaderboard.clear();
    ifstream inFile(FILE_NAME.c_str());

    if (inFile.is_open())
    {
        string n;
        int s;
        while (inFile >> n >> s)
        {
            leaderboard.push_back(makePlayer(n, s));
        }
        inFile.close();
    }

    if (leaderboard.empty())
    {
        leaderboard.push_back(makePlayer("AI_MASTER", 18));
        leaderboard.push_back(makePlayer("QUIZ_BOT", 15));
        leaderboard.push_back(makePlayer("NOOB_BOT", 10));
        leaderboard.push_back(makePlayer("GUEST", 5));
    }
}

void saveLeaderboard(const vector<Player>& leaderboard)
{
    ofstream outFile(FILE_NAME.c_str());
    if (outFile.is_open()) {
        for (int i = 0; i < leaderboard.size(); i++)
        {
            outFile << leaderboard[i].name << " " << leaderboard[i].score << endl;
        }
        outFile.close();
    }
}

void updateLeaderboard(vector<Player>& leaderboard, string playerName, int playerScore)
{
    leaderboard.push_back(makePlayer(playerName, playerScore));


    for (int i = 0; i < leaderboard.size(); i++)
    {
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
    {
        leaderboard.resize(5);
    }

    saveLeaderboard(leaderboard);
}

// Display Leaderboard Function ---
void displayLeaderboard(const vector<Player>& leaderboard) {
    cout << "\n   ==================================\n";
    cout << "   ||      HIGH SCORE RANKING      ||\n";
    cout << "   ==================================\n";
    for (int i = 0; i < leaderboard.size(); i++)
    {
        cout << "      " << i + 1 << ". " << leaderboard[i].name << "\t\t" << leaderboard[i].score << "\n";
    }
    cout << "   ==================================\n";
}

// Display Rules Function ---
void showRules() {
    cout << "\n==================================================\n";
    cout << "                  GAME RULES                      \n";
    cout << "==================================================\n";
    cout << "1. There are 4 subjects to choose from.\n";
    cout << "2. Each subject has 5 questions.\n";
    cout << "3. Each correct answer gives you 1 point.\n";
    cout << "4. No negative marking for wrong answers.\n";
    cout << "5. Try to beat the High Scores!\n";
    cout << "6. Do not use spaces in your name when saving.\n";
    cout << "==================================================\n";
    cout << "Press Enter to return to menu...";
    cin.ignore();
    cin.get();
}

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

bool askQuestion(const Question& q)
{
    cout << "\n------------------------------------------------\n";
    cout << "Q: " << q.text << endl;
    cout << "------------------------------------------------\n";

    for (int i = 0; i < 4; i++)
    {
        cout << "[" << i + 1 << "] " << q.options[i] << endl;
    }

    int answer;
    cout << "\nYour answer (1-4): ";
    cin >> answer;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "!Invalid input.\n";
        return false;
    }

    if (answer < 1 || answer > 4)
    {
        cout << "Wrong! Invalid Option.\n";
        cout << "    The correct answer is:    " << q.options[q.correctOption] << "\n";
        return false;
    }

    if (answer - 1 == q.correctOption)
    {
        cout << "\t Correct!";
        return true;
    }
    else
    {
        cout << "Wrong!";
        cout << "    The correct answer is:    " << q.options[q.correctOption] << "\n";
        return false;
    }
}

int runSubjectQuiz(const Subject& subj)
{
    cout << "\n\n************************************************\n";
    cout << "                SUBJECT: " << subj.name << "\n";
    cout << "************************************************\n";

    int score = 0;
    for (int i = 0; i < 5; i++)
    {
        if (askQuestion(subj.questions[i])) score++;
    }
    cout << "\nFinished " << subj.name << "! Score: " << score << "/5\n";
    cout << "Press Enter to return to menu...";
    cin.ignore();
    cin.get();
    return score;
}

// --- MENU ---
int showMenu(Subject subjects[], int n)
{
    cout << "\n\n========================================\n";
    cout << "||           QUIZ MAIN MENU           ||\n";
    cout << "========================================\n";
    for (int i = 0; i < n; i++)
    {
        cout << "||  " << i + 1 << ". " << subjects[i].name << "\n";
    }

    cout << "||  " << n + 1 << ". View Rules\n";
    cout << "||  " << n + 2 << ". View Leaderboard\n";
    cout << "----------------------------------------\n";
    cout << "||  " << n + 3 << ". Exit & Report\n";
    cout << "========================================\n";
    int choice;
    cout << "Choose option: ";
    cin >> choice;
    return choice;
}

int main()
{
    vector<Player> leaderboard;
    loadLeaderboard(leaderboard);

    Subject subjects[4] =
    {
        {"Math", {{"What is 2+2?", {"1","2","3","4"}, 3}, {"Square root of 16?", {"2","4","6","8"}, 1}, {"5*6?", {"11","30","25","20"}, 1}, {"10/2?", {"2","4","5","10"}, 2}, {"What is 7-3?", {"2","3","4","5"}, 2}}},
        {"Science", {{"Water formula?", {"H2O","CO2","O2","NaCl"}, 0}, {"Planet known as Red Planet?", {"Earth","Mars","Venus","Jupiter"}, 1}, {"Gas we breathe?", {"CO2","O2","N2","H2"}, 1}, {"Human heart chambers?", {"2","3","4","5"}, 2}, {"Speed of light?", {"3x10^8 m/s","3x10^6 m/s","300 m/s","None"}, 0}}},
        {"History", {{"Who discovered America?", {"Columbus","Newton","Einstein","Edison"}, 0}, {"First US President?", {"Lincoln","Washington","Jefferson","Adams"}, 1}, {"World War II ended in?", {"1940","1945","1950","1939"}, 1}, {"Great Wall is in?", {"India","China","Japan","Korea"}, 1}, {"Pyramids are in?", {"Mexico","Egypt","Peru","Iraq"}, 1}}},
        {"Computer", {{"CPU stands for?", {"Central Processing Unit","Computer Personal Unit","Control Process Unit","None"}, 0}, {"Binary system base?", {"2","8","10","16"}, 0}, {"Founder of Microsoft?", {"Jobs","Gates","Zuckerberg","Musk"}, 1}, {"HTML used for?", {"Design","Structure","Database","Networking"}, 1}, {"C++ developed by?", {"Bjarne Stroustrup","Dennis Ritchie","James Gosling","Guido"}, 0}}}
    };

    int totalScore[4] = { 0,0,0,0 };
    bool attempted[4] = { false, false, false, false };
    bool hasPlayedAny = false;

    cout << "========================================================\n";
    cout << "          WELCOME TO QUIZ MANAGEMENT SYSTEM             \n";
    cout << "========================================================\n";
    cout << "Test your knowledge and climb the leaderboard!\n";

    while (true)
    {
        int choice = showMenu(subjects, 4);


        if (choice == 7) break;

        if (choice >= 1 && choice <= 4)
        {
            int score = runSubjectQuiz(subjects[choice - 1]);
            totalScore[choice - 1] = score;
            attempted[choice - 1] = true;
            hasPlayedAny = true;
        }
        else if (choice == 5)
        {
            showRules();
        }
        else if (choice == 6)
        {
            displayLeaderboard(leaderboard);
            cout << "\nPress Enter to return...";
            cin.ignore();
            cin.get();
        }
        else
        {
            cout << "Invalid choice. Try again.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }

    cout << "\n\n########################################\n";
    cout << "#           FINAL REPORT CARD          #\n";
    cout << "########################################\n\n";

    int overall = 0;
    for (int i = 0; i < 4; i++)
    {
        overall += totalScore[i];
        cout << subjects[i].name << ":\t" << (subjects[i].name.length() < 8 ? "\t" : "")
            << totalScore[i] << "/5" << (!attempted[i] ? " (Not Attempted)" : "") << "\n";
    }

    cout << "\nOVERALL SCORE : " << overall << "/20\n";
    cout << "PERCENTAGE    : " << ((double)overall / 20.0 * 100.0) << "%\n";
    cout << "----------------------------------------\n";


    if (hasPlayedAny)
    {
        string playerName;
        cout << "\n>>> Great Job! Enter your name (No spaces allowed): ";
        cin >> playerName;

        updateLeaderboard(leaderboard, playerName, overall);
    }
    else
    {
        cout << "\n(You didn't take any quizzes, so score was not saved.)\n";
    }


    displayLeaderboard(leaderboard);
    cout << "\nThanks for playing!\n";

    return 0;
}
