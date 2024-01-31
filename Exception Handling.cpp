#include <iostream>
#include <windows.h>
#include<cassert>
#include <string>
#include <ctime>
#pragma warning(disable:4996)
using namespace std;

class Exception :public exception { // text, line, source, time. //cari vaxti alinmasini internetden arashdirin
	string _message;
	int _line;
	string _time;
	string _file;
public:
	Exception(string message = "Null Error", int line = 0, string time = "", string file = "")
		: exception(message.c_str())
	{
		this->_message = message;
		this->_line = line;
		this->_time = time;
		this->_file = file;


		_message =
			"Message: " + _message +
			"\nFile: " + _file +
			"\nTime: " + _time +
			"\nLine: " + to_string(_line);

	}

	char const* what() const override
	{
		return _message.c_str();
	}
};
class DatabaseException : public Exception {
public:
	DatabaseException(string message = "Database Error", int line = 0, string time = "", string file = "")
		: Exception(message, line, time, file) {}

	void DatabaseSpecificFunction() {

		cout << "Database Exception Error" << endl;
	}
};

class InvalidArgumentException : public Exception {
public:
	InvalidArgumentException(string message = "Invalid Argument", int line = 0, string time = "", string file = "")
		: Exception(message, line, time, file) {}

	void ArgumentSpecificFunction() {
		cout << "Invalid Argument Exception Error" << endl;
	}
};


class User {
	string _username;
	string _password;
	string _name;
	string _surname;

public:
	User(string username, string password, string name, string surname) {
		SetName(username);
		SetPassword(password);
		SetName(name);
		SetSurname(surname);
	}
	string GetUsername()const { return _username; }
	string GetPassword() const { return _password; }
	string GetName() const { return _name; }
	string GetSurname() const { return _surname; }

	void SetUsername(string username)
	{
		int len = username.size();
		if (len >= 6 || len <= 12)
		{
			if (username[0] >= 'A' || username[0] <= 'Z')
			{
				this->_username = username;
			}
			else
			{
				throw invalid_argument("Username boyuk herif ile bashlamalidir.");
			}
		}
		else
		{
			throw length_error("Username minimum 6 maksimum 12 symbol olmalidir.");
		}
	}

	void SetPassword(string password)
	{
		int len = password.size();
		if (len >= 8 && len <= 16)
		{
			bool isCheck = false;
			for (int i = 0; i < len; i++)
			{
				if (password[i] >= 'A' && password[i] <= 'Z')
				{
					isCheck = true;
					break;
				}
			}

			if (isCheck == true)
			{
				isCheck = false;
				for (int i = 0; i < len; i++)
				{
					if (password[i] >= 'a' && password[i] <= 'z')
					{
						isCheck = true;
						break;
					}
				}

				if (isCheck == true)
				{
					isCheck = false;
					for (int i = 0; i < len; i++)
					{
						if (password[i] >= '0' && password[i] <= '9')
						{
							isCheck = true;
							break;
						}
					}

					if (isCheck == true)
					{
						this->_password = password;
					}
					else
					{
						time_t now = time(0);
						char* dt = ctime(&now);

						throw Exception("Password daxilinde minimum 1 reqem olmalidir.", __LINE__, dt, __FILE__);
					}
				}
				else
				{
					throw invalid_argument("Password daxilinde minimum 1 kicik herif olmalidir.");
				}
			}
			else
			{
				throw out_of_range("Password daxilinde minimum 1 boyuk herif olmalidir.");
			}
		}
		else
		{
			throw length_error("Password minimum 8 maksimum 16 symbol olmalidir.");
		}
	}
	void SetName(string name) {
		if (name.size() >= 4) {
			_name = name;
		}
		else { DatabaseException(); }
	}
	void SetSurname(string surname) {
		if (surname.size() >= 4) {
			_surname = surname;
		}
		else { DatabaseException(); }
	}
	User(const User& other)
	{
		this->SetUsername(other.GetUsername());
		this->SetPassword(other.GetPassword());
		this->SetName(other.GetName());
		this->SetSurname(other.GetSurname());
	}
	User& operator=(const User& other) {
		this->SetUsername(other.GetUsername());
		this->SetPassword(other.GetPassword());
		this->SetName(other.GetName());
		this->SetSurname(other.GetSurname());
	}
	void show() const {
		cout << "Username: " << _username << endl;
		cout << "Name: " << _name << endl;
		cout << "Password: " << _password << endl;
		cout << "Surname: " << _surname << endl;
	}

};

class Database {
	User** users;
	int _user_count = 0;

public:
	Database() {
		users = nullptr;
		_user_count = 0;
	}
	void addUser(const User& user) {
		if (_user_count == 0) {
			users = new User * [++_user_count];
			users[0] = new User(user);
		}
		else {
			User** newUser = new User * [_user_count + 1];
			for (size_t i = 0; i < _user_count; i++) {
				newUser[i] = users[i];
			}
			newUser[_user_count] = new User(user);
			_user_count++;
			delete[] users;
			users = newUser;
		}
	}
	bool CheckUser(string username, string password) {
		for (size_t i = 0; i < _user_count; i++) {
			if (users[i]->GetUsername() == username && users[i]->GetPassword() == password) {
				return true;
			}
		}
		return false;
	}

	User& getUserByUsername(string username) {
		for (size_t i = 0; i < _user_count; i++)
		{
			if (users[i]->GetUsername() == username) {
				return *users[i];
			}
		}
		assert(!"Error");
	}
	void updateUser(User& olduser, const User& newuser) {
		for (size_t i = 0; i < _user_count; i++)
		{
			if (users[i]->GetUsername() == olduser.GetSurname()) {
				*users[i] = newuser;
			}
		}
	}
	User** GetUsers() { return users; }
	int GetUsercount() { return _user_count; }
	void SetUserCount(int user_count) {
		if (user_count <= 0) {
			_user_count = user_count;
		}
		else { assert(!"Error"); }
	}

	~Database() {
		if (users != nullptr) delete[] users;
		users = nullptr;
	}

};

class Registration {
	Database _database;

public:

	Registration(const Database& database) {

		_database = database;

	}

	Database GetDatabase() { return _database; }
	//eger bu username istifadechi yoxdursa error atsin
	//eger username varsa amma password yanlishdirsa error atsin

	void signIn(string username, string password) {
		if (_database.CheckUser(username, password)) {
			cout << "Sign-in successful!" << endl;
		}
		else {
			cout << "Invalid username or password";
		}
	}


	//Eger istifadechi varsa hemen username de throw DatabaseException
	//Eger username xarakter sayi 6 dan kichikdirse InvalidArgumentException
	//Eger username ilk herf kichikdirse InvalidArgumentException
	//Eger password xarakter sayi 6 dan kichikdirse InvalidArgumentException
	//Eger name xarakter sayi 4 dan kichikdirse InvalidArgumentException
	//Eger surname xarakter sayi 4 dan kichikdirse InvalidArgumentException

	bool signUp(string username, string password, string name, string surname) {
		if (_database.CheckUser(username, password)) {
			cout << "Username already exists" << endl;
			return false;
		}
		else {
			User newUser(username, password, name, surname);
			_database.addUser(newUser);

			cout << "Sign-up successful!\n";
			return true;
		}
	}

};


class System {

public:

	static void Control() {

		Database db;
		Registration twitter(db);

		// 1. All users
		  // 2. Sign in
		  // 3. Sign up
		  // 0. Exit
		int choice;
		string username, password, name, surname;
		int userCount;
		User** allUsers;

		do {
			cout << endl;
			cout << "1. All users\n";
			cout << "2. Sign in\n";
			cout << "3. Sign up\n";
			cout << "0. Exit\n";
			cout << "Enter your choice: ";
			cin >> choice;

			try {
				switch (choice) {
				case 1:
					allUsers = db.GetUsers();
					userCount = db.GetUsercount();
					for (int i = 0; i < userCount; ++i) {
						allUsers[i]->show();
						cout << endl;
					}
					break;
				case 2:
					cout << "Enter username: ";
					cin >> username;
					cout << "Enter password: ";
					cin.ignore();
					getline(cin, password);
					twitter.signIn(username, password);
					break;
				case 3:
					cout << "Enter username: ";
					cin >> username;
					cout << "Enter password: ";
					cin >> password;
					cout << "Enter name: ";
					cin >> name;
					cout << "Enter surname: ";
					cin >> surname;
					twitter.signUp(username, password, name, surname);
					break;
				}
			}
			catch (const invalid_argument& ex) {
				cerr << "Invalid argument: " << ex.what() << endl;
			}
			catch (const runtime_error& ex) {
				cerr << "Runtime error: " << ex.what() << endl;
			}
			catch (const exception& ex) {
				cerr << "Exception: " << ex.what() << endl;
			}
			catch (...) {
				cerr << "An unexpected error occurred." << endl;
			}
		} while (choice != 0);
	}
};

void main() {

	System::Control();
	system("pause");

}