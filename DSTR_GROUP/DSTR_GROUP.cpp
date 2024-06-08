#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <cctype>
using namespace std;

struct Discarded {
    string question;
    string choice1;
    string choice2;
    string answer;

    Discarded* next;
    Discarded* prev;

    Discarded(string q, string c1, string c2, string a)
        : question(q), choice1(c1), choice2(c2), answer(a), next(nullptr), prev(nullptr) {}
};

Discarded* dhead = nullptr;
Discarded* dtail = nullptr;

void AddDiscarded(string question, string choice1, string choice2, string answer) {
    Discarded* newDiscarded = new Discarded(question, choice1, choice2, answer);
    if (!dhead) {
        dhead = dtail = newDiscarded;
    }
    else {
        newDiscarded->next = dhead;
        dhead->prev = newDiscarded;
        dhead = newDiscarded;
    }
}

void DeleteDiscarded(Discarded* target) {
    if (!target) return;
    if (target->prev) target->prev->next = target->next;
    if (target->next) target->next->prev = target->prev;
    if (target == dhead) dhead = target->next;
    if (target == dtail) dtail = target->prev;
    delete target;
}

struct Question {
    string question;
    string choice1;
    string choice2;
    string answer;

    Question* next;
    Question* prev;

    Question(string q, string c1, string c2, string a)
        : question(q), choice1(c1), choice2(c2), answer(a), next(nullptr), prev(nullptr) {}
};

Question* qhead = nullptr;
Question* qtail = nullptr;

void AddQuestion(string question, string choice1, string choice2, string answer) {
    Question* newQuestion = new Question(question, choice1, choice2, answer);
    if (!qhead) {
        qhead = qtail = newQuestion;
    }
    else {
        newQuestion->next = qhead;
        qhead->prev = newQuestion;
        qhead = newQuestion;
    }
}

void DeleteQuestion(Question* target) {
    if (!target) return;
    if (target->prev) target->prev->next = target->next;
    if (target->next) target->next->prev = target->prev;
    if (target == qhead) qhead = target->next;
    if (target == qtail) qtail = target->prev;
    delete target;
}

void ReadFile() {
    ifstream file("Question.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string question, choice1, choice2, answer;
        getline(ss, question, ',');
        getline(ss, choice1, ',');
        getline(ss, choice2, ',');
        getline(ss, answer);
        AddQuestion(question, choice1, choice2, answer);
    }
    file.close();
}

int RandomIndex(int min, int max) {
    return min + rand() % (max - min + 1);
}

Question* SelectRandomQuestion() {
    int numQuestions = 0;
    Question* current = qhead;
    while (current) {
        numQuestions++;
        current = current->next;
    }

    if (numQuestions == 0) return nullptr;

    int randomIndex = RandomIndex(1, numQuestions);
    current = qhead;
    for (int i = 1; i < randomIndex; ++i) {
        current = current->next;
    }

    return current;
}
/*
struct Student {
    string tpNum;
    double totalScore;

    Student* next;
    Student* prev;

    Student(string tp) : tpNum(tp), totalScore(0), next(nullptr), prev(nullptr) {}
};*/

struct Student {
    string tpNum;
    double totalScore;
    string questions[3];
    double scores[3];

    Student* next;
    Student* prev;


    Student(string tp) : tpNum(tp), totalScore(0), next(nullptr), prev(nullptr) {
        for (int i = 0; i < 3; ++i) {
            questions[i] = "";
            scores[i] = 0;
        }
    }

    void addScore(int round, const string& question, int score) {
        if (round > 0 && round <= 3) {
            questions[round - 1] = question;
            scores[round - 1] = score;
            totalScore += score;
        }
    }

    void displayScores() const {
        cout << tpNum << ":\n";
        for (int i = 0; i < 3; ++i) {
            cout << "  Round " << i + 1 << ": " << questions[i] << " - " << scores[i] << " points\n";
        }
        cout << "  Overall Score: " << totalScore << " points\n";
    }
};


Student* shead = nullptr;
Student* stail = nullptr;

void AddStudent(int tpNum) {
    string tpStr = "TP" + to_string(tpNum);
    tpStr = tpStr.insert(2, 3 - to_string(tpNum).length(), '0');
    Student* newStudent = new Student(tpStr);
    if (!shead) {
        shead = stail = newStudent;
    }
    else if (tpNum <= stoi(shead->tpNum.substr(2))) {
        shead->prev = newStudent;
        newStudent->next = shead;
        shead = newStudent;
    }
    else if (tpNum >= stoi(stail->tpNum.substr(2))) {
        stail->next = newStudent;
        newStudent->prev = stail;
        stail = newStudent;
    }
    else {
        Student* current = shead->next;
        while (current && tpNum > stoi(current->tpNum.substr(2))) {
            current = current->next;
        }
        newStudent->next = current;
        newStudent->prev = current->prev;
        current->prev->next = newStudent;
        current->prev = newStudent;
    }
}

void PrintStudents() {
    Student* current = shead;
    while (current) {
        cout << "[" << current->tpNum << "] Total Score: " << current->totalScore << endl;
        current = current->next;
    }
}

void TrimAndLowerCase(string& str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    str = (start == string::npos) ? "" : str.substr(start, end - start + 1);
    for (char& c : str) c = tolower(c);
}

int GetNumberOfPlayers() {
    int numStudents;
    cout << "Enter the number of students playing: ";
    cin >> numStudents;
    cin.ignore();
    return numStudents;
}

void AddStudents(int numStudents) {
    for (int i = 1; i <= numStudents; ++i) {
        AddStudent(i);
    }
}

//Binary Search Tree
struct TreeNode {
    string tpNum;
    double totalScore;
    TreeNode* left;
    TreeNode* right;

    TreeNode(string tp, double score) : tpNum(tp), totalScore(score), left(nullptr), right(nullptr) {}
};

void InsertTreeNode(TreeNode*& root, string tpNum, double totalScore) {
    if (!root) {
        root = new TreeNode(tpNum, totalScore);
    }
    else if (totalScore > root->totalScore) {
        InsertTreeNode(root->right, tpNum, totalScore);
    }
    else {
        InsertTreeNode(root->left, tpNum, totalScore);
    }
}

void InOrderTraversal(TreeNode* root, int& count, int& n) {
    if (!root) return;
    InOrderTraversal(root->right, count, n);
    if (count < 30) { // Check if already found top 30 winners
        cout << n++ << " place: " << root->tpNum << " - Total Score: " << root->totalScore << endl;
        count++;
    }
    InOrderTraversal(root->left, count, n);
}


void DisplayTop30Winners(TreeNode* root) {
    int count = 0;
    int n = 1; // for placement counter usage
    cout << "Top 30 Winners:" << endl;
    InOrderTraversal(root, count, n);
}


TreeNode* SearchTreeNode(TreeNode* root, string tpNum) {
    if (!root || root->tpNum == tpNum) {
        return root;
    }
    if (tpNum < root->tpNum) {
        return SearchTreeNode(root->left, tpNum);
    }
    else {
        return SearchTreeNode(root->right, tpNum);
    }
}

int CountStudentsVisitedUpTo30(TreeNode* root) {
    int count = 0;
    int n = 0;
    InOrderTraversal(root, count, n);
    return count;
}

bool IsTop30Winner(TreeNode* root, string tpNum) {
    if (!root) return false; // If tree is empty, return false
    TreeNode* result = SearchTreeNode(root, tpNum); // Search for the student
    if (result) {
        int count = 0;
        int n = 1; // for placement counter usage
        InOrderTraversal(root, count, n); // Count the number of students visited (up to 30)
        return count <= 30; // If count is less than or equal to 30, student is in top 30
    }
    else {
        return false; // false if not found in the tree
    }
}

void SearchAndDisplayStudent(TreeNode* root, string tpNum) {
    bool foundInTop30 = IsTop30Winner(root, tpNum); // check true or false for input TpNum
    if (foundInTop30) {
        cout << "Student " << tpNum << " found and is among the top 30 winners." << endl;
    }
    else {
        cout << "Student " << tpNum << " not found or not among the top 30 winners." << endl;
    }
}


void SearchByOverallScore(Student* shead) {
    if (!shead) {
        cout << "No students available to search.\n";
        return;
    }

    double maxScore = -1;
    double minScore = 101;
    Student* maxStudent = nullptr;
    Student* minStudent = nullptr;

    Student* current = shead;
    while (current) {
        if (current->totalScore > maxScore) {
            maxScore = current->totalScore;
            maxStudent = current;
        }
        if (current->totalScore < minScore) {
            minScore = current->totalScore;
            minStudent = current;
        }
        current = current->next;
    }

    if (maxStudent) {
        cout << "Highest Scoring Student: " << maxStudent->tpNum << " with " << maxStudent->totalScore << " points.\n";
    }

    if (minStudent) {
        cout << "Lowest Scoring Student: " << minStudent->tpNum << " with " << minStudent->totalScore << " points.\n";
    }
}
void TrimAndLowerCaseForSearch(string& str) {
    // Remove leading and trailing whitespace
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    str = (start == string::npos) ? "" : str.substr(start, end - start + 1);

    // Convert to lowercase
    transform(str.begin(), str.end(), str.begin(), ::tolower);
}

bool SearchByQuestion(Student* shead, const string& queryQuestion) {
    if (!shead) {
        cout << "No students available to search.\n";
        return false;
    }

    string cleanedQueryQuestion = queryQuestion;
    TrimAndLowerCaseForSearch(cleanedQueryQuestion);

    bool found = false;
    Student* current = shead;
    while (current) {
        for (int i = 0; i < 3; ++i) {
            string cleanedStudentQuestion = current->questions[i];
            TrimAndLowerCaseForSearch(cleanedStudentQuestion);
            if (cleanedStudentQuestion == cleanedQueryQuestion) {
                found = true;
                cout << "Student " << current->tpNum << " answered question '" << current->questions[i] << "' with score: " << current->scores[i] << "\n";
            }
        }
        current = current->next;
    }

    if (!found) {
        cout << "No students have answered the question: " << queryQuestion << "\n";
    }
    return found;
}


string GetTPNum() {
    string tpNum;
    cout << "Enter TP number to search: ";
    cin >> tpNum;
    return tpNum;
}

void DisplayDetailedLeaderboard() {
    cout << "\nDetailed Leaderboard:" << endl;
    Student* current = shead;
    while (current) {
        cout << current->tpNum << ":" << endl;
        for (int i = 0; i < 3; ++i) {
            cout << "  Round " << i + 1 << ": " << current->questions[i] << " - " << current->scores[i] << " points" << endl;
        }
        cout << "  Overall Score: " << current->totalScore << " points\n" << endl;
        current = current->next;
    }
}

void StartGame() {
    ReadFile();
    int numStudents = GetNumberOfPlayers();
    AddStudents(numStudents);

    srand(time(nullptr)); // Seed the random number generator

    for (int round = 1; round <= 3; ++round) {
        cout << "Round " << round << " starts:" << endl;

        Student* currentStudent = shead;
        while (currentStudent) {
            cout << currentStudent->tpNum << " (Round " << round << "):" << endl;
            if (!qhead && !dhead) {
                cout << "No more questions available." << endl;
                break;
            }

            int choice;
            while (true) {
                cout << "1. Answer Deck" << endl;
                cout << "2. Discarded Deck:" << endl;
                if (dhead) {
                    Discarded* temp = dhead;
                    int index = 1;
                    while (temp) {
                        cout << index++ << ". " << temp->question << endl;
                        temp = temp->next;
                    }
                }
                else {
                    cout << "No questions in the Discarded Deck." << endl;
                }

                cout << "Choose question deck (1: Answer Deck, 2: Discarded Deck): ";
                cin >> choice;
                cin.ignore();

                if (choice == 1 && qhead) {
                    break;
                }
                else if (choice == 2 && dhead) {
                    break;
                }
                else {
                    cout << "Invalid choice or no questions available. Please choose again." << endl;
                }
            }

            if (choice == 1 && qhead) {
                Question* q = SelectRandomQuestion();
                if (!q) {
                    cout << "No more questions available." << endl;
                    break;
                }

                string answer;
                cout << q->question << " " << q->choice1 << ", " << q->choice2 << endl;
                cout << "Your Answer: ";
                getline(cin, answer);

                TrimAndLowerCase(answer);
                TrimAndLowerCase(q->answer);

                if (answer == q->answer) {
                    currentStudent->totalScore += 10;
                    currentStudent->scores[round - 1] = 10; // Record score for the round
                    currentStudent->questions[round - 1] = q->question; // Record question for the round
                    cout << "Correct! You got 10 points." << endl;
                    DeleteQuestion(q);
                }
                else {
                    AddDiscarded(q->question, q->choice1, q->choice2, q->answer);
                    cout << "Incorrect! The question is moved to the Discarded Deck." << endl;
                }
            }
            else if (choice == 2 && dhead) {
                Discarded* d = dhead;
                string answer;
                cout << d->question << " " << d->choice1 << ", " << d->choice2 << endl;
                cout << "Your Answer: ";
                getline(cin, answer);

                TrimAndLowerCase(answer);
                TrimAndLowerCase(d->answer);

                if (answer == d->answer) {
                    currentStudent->totalScore += 8;
                    currentStudent->scores[round - 1] = 8; // Record score for the round
                    currentStudent->questions[round - 1] = d->question; // Record question for the round
                    cout << "Correct! You got 8 points." << endl;
                    DeleteDiscarded(dhead);
                }
                else {
                    cout << "Incorrect! The question remains in the Discarded Deck." << endl;
                }
            }
            else {
                cout << "Invalid choice or no questions available. Please choose again." << endl;
            }

            currentStudent = currentStudent->next;
        }
        cout << "End of Round " << round << endl;
    }

    TreeNode* root = nullptr;

    // Calculate total scores and insert into BST
    for (Student* current = shead; current; current = current->next) {
        InsertTreeNode(root, current->tpNum, current->totalScore);
    }

    PrintStudents();
    DisplayTop30Winners(root);


   // Display detailed scores on the leaderboard
    DisplayDetailedLeaderboard();

    // Search for a student
    string searchTpNum = GetTPNum();
    SearchAndDisplayStudent(root, searchTpNum);


    int choice;
    cout << "\nDo you want to search for the strongest or weakest students by overall score? (1 for yes, 0 for no): ";
    cin >> choice;
    if (choice == 1) {
        SearchByOverallScore(shead);
    }

    string query;
    cin.ignore(); // Clear the input buffer before starting the search loop
    while (true) {
        cout << "Enter a question to search (or 'exit' to stop):";
        getline(cin, query);
        if (query == "exit") break;
        if (!query.empty()) { // Ensure query is not empty before searching
            SearchByQuestion(shead, query);
        }
    }
}


int main() {
    StartGame();
    return 0;
}
