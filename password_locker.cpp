#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <limits>

#ifdef _WIN32
#include <conio.h>
#include <windows.h> 
#else
#include <unistd.h> 
#endif

const std::string FILE_NAME = "passwords.dat";  
const std::string ADMIN_PASSWORD = "Your_Pass"; // Yetkili giriş şifresi

std::map<std::string, std::string> passwordDatabase; // Şifreler (Kategori -> Şifre)


std::string encryptDecrypt(const std::string &data, char key = 'K')
{
    std::string output = data;
    for (size_t i = 0; i < data.size(); i++)
    {
        output[i] ^= key
    }
    return output;
}


void loadPasswords()
{
    std::ifstream file(FILE_NAME);
    if (!file)
    {
        std::cout << "⚠ Password file not found! Creating new file...\n";
        return;
    }

    std::string category, password;
    while (file >> category >> password)
    {
        passwordDatabase[category] = encryptDecrypt(password);
    }
    file.close();
}

// 📌 Şifreleri Dosyaya Kaydetme
void savePasswords()
{
    std::ofstream file(FILE_NAME);
    for (const auto &pair : passwordDatabase)
    {
        file << pair.first << " " << encryptDecrypt(pair.second) << "\n";
    }
    file.close();
}


bool fileExists(const std::string &filename)
{
    std::ifstream file(filename);
    return file.good();
}


void protectFile()
{
#ifdef _WIN32
    system(("attrib +h " + FILE_NAME).c_str());
#else
    system(("chmod 600 " + FILE_NAME).c_str()); 
#endif
}

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
            std::cout << '*';
        }
    }
    std::cout << std::endl;
    return password;
#else
    return getpass("Password: ");
#endif
}

// 📌 Yetkili Giriş
bool adminLogin()
{
    std::cout << "Admin Login\n";
    std::string inputPassword = getPasswordInput();
    return inputPassword == ADMIN_PASSWORD;
}

// 📌 Yeni Şifre Ekle
void addPassword()
{
    std::string category, password;
    std::cout << "Enter category (e.g., facebook, gmail, bank): ";
    std::getline(std::cin, category);

    std::cout << "Enter password: ";
    password = getPasswordInput();

    passwordDatabase[category] = password;
    savePasswords();
    std::cout << "✅ Password saved successfully!\n";
}

// 📌 Şifreyi Görüntüle
void retrievePassword()
{
    std::string category;
    std::cout << "Enter category to retrieve password: ";
    std::getline(std::cin, category);

    if (passwordDatabase.find(category) != passwordDatabase.end())
    {
        std::cout << "🔑 Password for " << category << ": " << passwordDatabase[category] << "\n";
    }
    else
    {
        std::cout << "⚠ No password found for this category!\n";
    }
}

// 📌 Ana Program
int main()
{
    if (!fileExists(FILE_NAME))
    {
        std::ofstream file(FILE_NAME);
        file.close();
    }

    protectFile();   // Dosyayı gizle ve koru
    loadPasswords(); // Şifreleri yükle

    if (!adminLogin())
    {
        std::cout << "❌ Incorrect password! Exiting...\n";
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
            std::cout << "🔒 Exiting...\n";
            break;
        }
        else
        {
            std::cout << "⚠ Invalid choice, try again.\n";
        }
    }

    return 0;
}
