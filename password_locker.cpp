#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <limits>
#include <cstring>  // strcpy için
#include <unistd.h> // getpass için (Linux/Mac)

#ifdef _WIN32
#include <conio.h> // Windows için getch kullanacağız
#endif

const std::string ADMIN_PASSWORD = "YOUR_ADMIN_PASSWORD"; // Buraya admin şifrenizi girin

std::map<std::string, std::string> passwordDatabase; // Kategori -> Şifre eşlemesi

std::string getPasswordInput()
{
#ifdef _WIN32
    std::string password;
    char ch;
    std::cout << "Password: ";
    while ((ch = _getch()) != '\r')
    { // Enter'a basılana kadar
        if (ch == '\b')
        { // Backspace
            if (!password.empty())
            {
                password.pop_back();
                std::cout << "\b \b";
            }
        }
        else
        {
            password.push_back(ch);
            std::cout << '*'; // Yıldız ile gizle
        }
    }
    std::cout << std::endl;
    return password;
#else
    return getpass("Password: "); // Linux/Mac için
#endif
}

bool adminLogin()
{
    std::cout << "Admin login\n";
    std::string inputPassword = getPasswordInput();
    return inputPassword == ADMIN_PASSWORD;
}

void addPassword()
{
    std::string category, password;

    std::cout << "Enter category (e.g., facebook, gmail, bank): ";
    std::getline(std::cin, category);

    std::cout << "Enter password: ";
    password = getPasswordInput();

    passwordDatabase[category] = password;
    std::cout << "Password saved successfully!\n";
}

void retrievePassword()
{
    std::string category;

    std::cout << "Enter category to retrieve password: ";
    std::getline(std::cin, category);

    if (passwordDatabase.find(category) != passwordDatabase.end())
    {
        std::cout << "Password for " << category << ": " << passwordDatabase[category] << "\n";
    }
    else
    {
        std::cout << "No password found for this category!\n";
    }
}

int main()
{
    if (!adminLogin())
    {
        std::cout << "Incorrect password! Exiting...\n";
        return 0;
    }

    int choice;
    while (true)
    {
        std::cout << "\n1 - Add New Password\n";
        std::cout << "2 - Retrieve Password\n";
        std::cout << "3 - Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1)
        {
            addPassword();
        }
        else if (choice == 2)
        {
            retrievePassword();
        }
        else if (choice == 3)
        {
            std::cout << "Exiting...\n";
            break;
        }
        else
        {
            std::cout << "Invalid choice, try again.\n";
        }
    }
    return 0;
}
