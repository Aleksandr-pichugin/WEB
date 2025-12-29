#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cctype>

using namespace std;

struct UserStats {
    string username;
    int totalAttempts = 0;
    int correctAttempts = 0;
    double totalTime = 0.0;
    int totalChars = 0;
};

class KeyboardTrainer {
private:
    UserStats currentUser;
    vector<string> texts = {
        "The quick brown fox jumps over the lazy dog.",
        "Programming is fun and challenging.",
        "C++ is a powerful programming language.",
        "Practice makes perfect in typing skills.",
        "Type fast and accurately to improve your skills."
    };

    void saveStats() {
        ofstream outFile("stats.txt");
        outFile << currentUser.username << endl;
        outFile << currentUser.totalAttempts << endl;
        outFile << currentUser.correctAttempts << endl;
        outFile << currentUser.totalTime << endl;
        outFile << currentUser.totalChars << endl;
        outFile.close();
    }

    bool loadStats(const string& username) {
        ifstream inFile("stats.txt");
        if (!inFile) return false;

        string name;
        inFile >> name;
        if (name != username) return false;

        inFile >> currentUser.totalAttempts;
        inFile >> currentUser.correctAttempts;
        inFile >> currentUser.totalTime;
        inFile >> currentUser.totalChars;

        currentUser.username = username;
        inFile.close();
        return true;
    }

public:
    KeyboardTrainer() {}

    void start() {
        cout << "=== Клавиатурный тренажёр ===\n";
        login();

        while (true) {
            cout << "\n1. Начать тренировку\n";
            cout << "2. Посмотреть статистику\n";
            cout << "3. Сменить пользователя\n";
            cout << "4. Выход\n";
            cout << "Выберите действие: ";

            int choice;
            cin >> choice;

            // Очищаем буфер ввода (удаляем \n и др. символы)
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
            case 1:
                runTraining();
                break;
            case 2:
                showStats();
                break;
            case 3:
                login();
                break;
            case 4:
                cout << "До свидания!\n";
                return;
            default:
                cout << "Неверный выбор!\n";
            }
        }
    }

private:
    void login() {
        string username;
        cout << "Введите имя пользователя: ";
        getline(cin, username);  // Используем getline для поддержки пробелов в имени

        currentUser.username = username;
        if (!loadStats(username)) {
            cout << "Новый пользователь. Добро пожаловать, " << username << "!\n";
            currentUser.totalAttempts = 0;
            currentUser.correctAttempts = 0;
            currentUser.totalTime = 0.0;
            currentUser.totalChars = 0;
        }
        else {
            cout << "Добро пожаловать обратно, " << username << "!\n";
        }
    }

    void runTraining() {
        srand(time(0));
        string text = texts[rand() % texts.size()];

        cout << "\nНаберите следующий текст:\n";
        cout << text << "\n";

        string input;
        cout << "Ваш ввод: ";
        auto start = clock();
        getline(cin, input);  // Теперь корректно считывает всю строку
        auto end = clock();

        double timeTaken = double(end - start) / CLOCKS_PER_SEC;
        int chars = text.length();

        bool correct = (input == text);

        currentUser.totalAttempts++;
        if (correct) {
            currentUser.correctAttempts++;
            currentUser.totalTime += timeTaken;
            currentUser.totalChars += chars;
            cout << "Правильно! Время: " << timeTaken << " сек. Скорость: "
                << (chars / timeTaken) << " символов/сек\n";
        }
        else {
            cout << "Ошибка! Правильный текст: " << text << "\n";
        }

        saveStats();
    }

    void showStats() {
        cout << "\n=== Статистика пользователя " << currentUser.username << " ===\n";
        cout << "Всего попыток: " << currentUser.totalAttempts << "\n";
        cout << "Правильных попыток: " << currentUser.correctAttempts << "\n";

        if (currentUser.totalAttempts > 0) {
            double accuracy = (currentUser.correctAttempts * 100.0) / currentUser.totalAttempts;
            cout << "Точность: " << accuracy << "%\n";

            if (currentUser.totalChars > 0 && currentUser.totalTime > 0) {
                double speed = currentUser.totalChars / currentUser.totalTime;
                cout << "Средняя скорость: " << speed << " символов/сек\n";
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    KeyboardTrainer trainer;
    trainer.start();
    return 0;
}