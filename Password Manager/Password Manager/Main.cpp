#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cctype>
#include <map>
#include <sstream>
#include <limits> 
#include <stdexcept> 

std::string generateRandomPassword(int length) {
    const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+";
    std::string password;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = 0; i < length; ++i) {
        password += charset[std::rand() % charset.size()];
    }
    return password;
}

std::string encrypt(const std::string& text, int key) {
    std::string result = text;
    for (char& c : result) {
        if (std::isalpha(c)) {
            char base = std::isupper(c) ? 'A' : 'a';
            c = (c - base + key) % 26 + base;
        }
    }
    return result;
}

std::string decrypt(const std::string& text, int key) {
    return encrypt(text, 26 - key);
}

void storePassword(const std::string& site, const std::string& username, const std::string& password) {
    std::ofstream file("C:\\mang.txt", std::ios::app);
    if (file.is_open()) {
        std::string encryptedSite = encrypt(site, 3);
        std::string encryptedUsername = encrypt(username, 3);
        std::string encryptedPassword = encrypt(password, 3);
        file << encryptedSite << " " << encryptedUsername << " " << encryptedPassword << std::endl;
        std::cout << "Password stored successfully." << std::endl;
    }
    else {
        std::cerr << "Error: Unable to open file for storing password." << std::endl;
        system("cls");
    }
}

std::map<std::string, std::pair<std::string, std::string>> retrievePasswords() {
    std::ifstream file("C:\\mang.txt");
    std::map<std::string, std::pair<std::string, std::string>> passwords;
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string encryptedSite, encryptedUsername, encryptedPassword;
            if (iss >> encryptedSite >> encryptedUsername >> encryptedPassword) {
                std::string site = decrypt(encryptedSite, 3);
                std::string username = decrypt(encryptedUsername, 3);
                std::string password = decrypt(encryptedPassword, 3);
                passwords[site] = std::make_pair(username, password);
            }
        }
    }
    else {
        std::cerr << "Error: Unable to open password file for retrieval." << std::endl;
    }
    return passwords;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    int choice;
    std::map<std::string, std::pair<std::string, std::string>> passwords;
    do {
        clearScreen();
        passwords = retrievePasswords();
        std::cout << "\nSecure Password Manager\n";
        std::cout << "1. Store New Password\n";
        std::cout << "2. Retrieve Password\n";
        std::cout << "3. Generate Random Password\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1: {
            std::string site, username, password;
            std::cout << "Enter website or service name: ";
            std::getline(std::cin, site);
            std::cout << "Enter username: ";
            std::getline(std::cin, username);
            std::cout << "Enter password: ";
            std::getline(std::cin, password);
            storePassword(site, username, password);
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
            break;
        }
        case 2: {
            std::string site;
            std::cout << "Enter website or service name: ";
            std::getline(std::cin, site);
            auto it = passwords.find(site);
            if (it != passwords.end()) {
                std::cout << "Username: " << it->second.first << std::endl;
                std::cout << "Password: " << it->second.second << std::endl;
            }
            else {
                std::cout << "Password not found for the given site." << std::endl;
            }
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
            break;
        }
        case 3: {
            int length;
            std::cout << "Enter password length: ";
            std::cin >> length;
            std::cout << "Generated Password: " << generateRandomPassword(length) << std::endl;
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }
        case 4:
            std::cout << "Exiting program." << std::endl;
            break;
        default:
            std::cerr << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 4);

    return 0;
}