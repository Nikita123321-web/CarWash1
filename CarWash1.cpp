#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include "sqlite3.h"
#include <Windows.h>
using namespace std;

void viewTable(string nameTable, string id)
{
	string nameDatabase; // строковое значение string 
	ifstream in("config.txt"); // происходит чтение ввода в программу  из файла 
	if (in.is_open()) // условие if 
	{
		while (getline(in, nameDatabase)) // Функция getline предназначена для ввода данных из потока, например, для ввода данных из консольного окна.
		{
			ifstream file; // происходит чтение ввода в программу  из файла 
			file.open(nameDatabase); // открытие файла с помощью команды file.open и имя переменной nameDatabase
			file.close(); // закрытие файла с помощью команды file.close
			if (file)
			{
				int rc;
				char* error;
				sqlite3* db;
				rc = sqlite3_open(nameDatabase.c_str(), &db);
				if (rc)
				{
					cerr << "Ошибка подключения к базе данных: " << sqlite3_errmsg(db) << endl << endl;
					sqlite3_close(db);
					system("pause");

				}
				string sqlSelect;
				if (nameTable == "Orders") {
					if (id == "0") {
						sqlSelect = "SELECT orders.id as 'id_order', clients.id as 'id_client', clients.lastname, services.id as 'id_service', services.name, orders.day from clients, services, orders WHERE clients.id = orders.id_client AND services.id = orders.id_service ";
					}
					else {
						sqlSelect = "SELECT orders.id as 'id_order', clients.id as 'id_client', clients.lastname, services.id as 'id_service', services.name, orders.day from clients, services, orders WHERE clients.id = orders.id_client AND services.id = orders.id_service  AND orders.id = " + id;
						// select from имя таблицы в SQLite и  названия , которые будут в таблице в заказах 
					}
				}
				else if (nameTable != "Reports") {
					if (id == "0") {
						sqlSelect = "SELECT * FROM " + nameTable + ";";
					}
					else {
						sqlSelect = "SELECT * FROM " + nameTable + " WHERE id = " + id;
					}
				}
				cout << "" << endl;
				if ((id == "1") && (nameTable == "Reports")) {
					cout << "Подсчет выручки за текущий день:" << endl;
					sqlSelect = "SELECT SUM(cost) FROM services WHERE id IN (SELECT id_service FROM orders WHERE day = date('now'))";
				}
				else if ((id == "2") && (nameTable == "Reports")) {
					cout << "Подсчет выручки за текущий месяц:" << endl;
					sqlSelect = "SELECT SUM(cost) FROM services WHERE id IN (SELECT id_service FROM orders WHERE strftime('%m', day) = strftime('%m', date('now')) AND strftime('%Y', day) = strftime('%Y', date('now')))";
				}
				else if ((id == "3") && (nameTable == "Reports")) {
					cout << "Подсчет выручки за текущий год:" << endl;
					sqlSelect = "SELECT SUM(cost) FROM services WHERE id IN (SELECT id_service FROM orders WHERE strftime('%Y', day) = strftime('%Y', date('now')))";
				}
				else if ((id == "4") && (nameTable == "Reports")) {
					cout << "Подсчет количества посетителей за текущий день:" << endl;
					sqlSelect = "SELECT COUNT(*) FROM orders WHERE day = date('now')";
				}
				else if ((id == "5") && (nameTable == "Reports")) {
					cout << "Подсчет количества посетителей за текущий месяц:" << endl;
					sqlSelect = "SELECT COUNT(*) FROM orders WHERE strftime('%m', day) = strftime('%m', date('now')) AND strftime('%Y', day) = strftime('%Y', date('now'))";
				}
				else if ((id == "6") && (nameTable == "Reports")) {
					cout << "Подсчет количества посетителей за текущий год:" << endl;
					sqlSelect = "SELECT COUNT(*) FROM orders WHERE strftime('%Y', day) = strftime('%Y', date('now'))";
					//////////////////////////////////////////////////////        РАБОТА С SQ LITE 
				}
				char** results = NULL; // **  указатель на строковый двухмерный массив 
				int rows, columns;
				sqlite3_get_table(db, sqlSelect.c_str(), &results, &rows, &columns, &error);
				if (rc)
				{
					cerr << "Ошибка запроса к базе данных: " << sqlite3_errmsg(db) << endl << endl;
					sqlite3_free(error);
				}
				else
				{
					for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
					{
						for (int colCtr = 0; colCtr < columns; ++colCtr)
						{
							int cellPosition = (rowCtr * columns) + colCtr;
							cout.width(12);
							cout.setf(ios::left);
							cout << results[cellPosition] << " ";
						}
						cout << endl;
						if (0 == rowCtr)
						{
							for (int colCtr = 0; colCtr < columns; ++colCtr)
							{
								cout.width(30);
								cout.setf(ios::left);
								cout << "";
							}
							cout << endl;
						}
					}
				}
				sqlite3_free_table(results);
				sqlite3_close(db);
			}
		}
	}
}
/////////////////////////////////////////////////////////
		// РАБОТА С SQLITE 

void addTable(string nameTable) // 
{
	string nameDatabase; // строковый тип данных имя nameDataBase
	ifstream in("config.txt");  //   на чтение  ввод в программу из файла
	if (in.is_open()) //  открытие файла с условием 
	{
		while (getline(in, nameDatabase)) // цикл + ввод  данных из таблицы / Функция getline предназначена для ввода данных из потока, например, для ввода данных из консольного окна.
		{
			ifstream file; //  на чтение  ввод в программу из файла
			file.open(nameDatabase); // открытие файла 
			file.close(); // закрытие файла 
			if (file)
			{
				int rc;
				char* error; // символьный тип char одномерный массив переменная error
				sqlite3* db;
				rc = sqlite3_open(nameDatabase.c_str(), &db);
				if (rc)
				{
					cerr << "Ошибка подключения к базе данных: " << sqlite3_errmsg(db) << endl << endl;
					sqlite3_close(db);
					system("pause");
					exit(0);
				}
				string sqlInsert;
				if (nameTable == "Clients") {
					string Lastname;
					cout << "Введите фамилию: ";
					cin >> Lastname;
					cerr << "" << endl;
					string Name;
					cout << "Введите имя: ";
					cin >> Name;
					cerr << "" << endl;
					string Patronymic;
					cout << "Введите отчество: ";
					cin >> Patronymic;
					cerr << "" << endl;
					string Gosnomer;
					cout << "Введите госномер автомобиля: ";
					cin >> Gosnomer;
					cerr << "" << endl;
					string Marka;
					cout << "Введите марку автомобиля: ";
					cin.ignore();
					getline(cin, Marka); // Функция getline предназначена для ввода данных из потока, например, для ввода данных из консольного окна.
					cerr << "" << endl;

					sqlInsert = "INSERT INTO clients VALUES(NULL, '" + Lastname + "', '" + Name + "', '" + Patronymic + "', '" + Gosnomer + "', '" + Marka + "');"; // добавление в  таблицу 
				}
				if (nameTable == "Services") {
					string nameService;
					cout << "Введите наименование услуги: ";
					cin.ignore();
					getline(cin, nameService);
					cerr << "" << endl;
					string cost;
					cout << "Введите стоимость услуги: ";
					cin >> cost;
					cerr << "" << endl;

					sqlInsert = "INSERT INTO services VALUES(NULL, '" + nameService + "', " + cost + ");";
				}
				if (nameTable == "Orders") {
					viewTable("Clients", "0");
					cerr << "" << endl;
					string idClient;
					cout << "Введите ID клиента: ";
					cin.ignore();
					getline(cin, idClient);
					cerr << "" << endl;
					viewTable("Services", "0");
					cerr << "" << endl;
					string idServices;
					cout << "Введите ID услуги: ";
					cin >> idServices;
					cerr << "" << endl;

					sqlInsert = "INSERT INTO orders VALUES(NULL, " + idClient + ", " + idServices + ", date('now'))";
				}
				cout << "Новая запись добавлена!";
				cerr << "" << endl;
				rc = sqlite3_exec(db, sqlInsert.c_str(), NULL, NULL, &error);
				if (rc)
				{
					cerr << "Ошибка запроса к базе данных: " << sqlite3_errmsg(db) << endl << endl;
					sqlite3_free(error);
				}
				sqlite3_close(db); //  закрытие БД
				return;
			}
		}
	}
}

void updateTable(string nameTable)
{
	string nameDatabase;
	ifstream in("config.txt");
	if (in.is_open())
	{
		while (getline(in, nameDatabase))
		{
			ifstream file;
			file.open(nameDatabase);
			file.close();
			if (file)
			{
				int rc;
				char* error;
				sqlite3* db;
				rc = sqlite3_open(nameDatabase.c_str(), &db);
				if (rc)
				{
					cerr << "Ошибка подключения к базе данных: " << sqlite3_errmsg(db) << endl << endl;
					sqlite3_close(db);
					system("pause");
					exit(0);
				}
				string id;
				cout << "Введите ID записи, которую хотите изменить: ";
				cin >> id;
				cerr << "" << endl;
				string sqlUpdate;
				if (nameTable == "Clients") {
					viewTable("Clients", id);
					cerr << "" << endl;
					string lastname;
					cout << "Введите новую фамилию: ";
					cin >> lastname;
					cerr << "" << endl;
					string name;
					cout << "Введите новое имя: ";
					cin >> name;
					cerr << "" << endl;
					string patronymic;
					cout << "Введите новое отчество: ";
					cin >> patronymic;
					cerr << "" << endl;
					string gosnomer;
					cout << "Введите новый госномер автомобиля: ";
					cin.ignore();
					getline(cin, gosnomer);
					cerr << "" << endl;
					string marka;
					cout << "Введите новую марку автомобиля: ";
					cin.ignore();
					getline(cin, marka);
					cerr << "" << endl;

					sqlUpdate = "UPDATE clients SET lastname = '" + lastname + "', name = '" + name + "', patronymic = '" + patronymic + "', gosnomer = '" + gosnomer + "', marka = '" + marka + "' WHERE id = " + id;
				}
				if (nameTable == "Services") {
					viewTable("Services", id);
					cerr << "" << endl;
					string nameService;
					cout << "Введите новое наименование услуги: ";
					cin.ignore();
					getline(cin, nameService);
					cerr << "" << endl;
					string cost;
					cout << "Введите новую стоимость услуги: ";
					cin >> cost;
					cerr << "" << endl;

					sqlUpdate = "UPDATE services SET name = '" + nameService + "', cost = " + cost + " WHERE id = " + id;
				}
				if (nameTable == "Orders") {
					viewTable("Orders", id);
					cerr << "" << endl;
					string idClients;
					cout << "Введите новый ID клиента: ";
					cin.ignore();
					getline(cin, idClients);
					cerr << "" << endl;
					string idServices;
					cout << "Введите новый ID услуги: ";
					cin >> idServices;
					cerr << "" << endl;
					string day;
					cout << "Введите новую дату в формате YYYY-MM-DD: ";
					cin >> day;
					cerr << "" << endl;

					sqlUpdate = "UPDATE orders SET id_client = " + idClients + ", id_service = " + idServices + ", day = '" + day + "' WHERE id = " + id;
				}
				cout << "Запись обновлена!";
				cerr << "" << endl;
				rc = sqlite3_exec(db, sqlUpdate.c_str(), NULL, NULL, &error);
				if (rc)
				{
					cerr << "Ошибка запроса к базе данных: " << sqlite3_errmsg(db) << endl << endl;
					sqlite3_free(error);
				}
				sqlite3_close(db);
				return;
			}
		}
	}
}

void deleteTable(string nameTable)
{
	string nameDatabase;
	ifstream in("config.txt");
	if (in.is_open())
	{
		while (getline(in, nameDatabase))
		{
			ifstream file;
			file.open(nameDatabase);
			file.close();
			if (file)
			{
				int rc;
				char* error;
				sqlite3* db;
				rc = sqlite3_open(nameDatabase.c_str(), &db);
				if (rc)
				{
					cerr << "Ошибка подключения к базе данных: " << sqlite3_errmsg(db) << endl << endl;
					sqlite3_close(db);
					system("pause");
					exit(0);
				}
				string sqlDelete;
				string id;
				cout << "Введите ID записи, которую хотите удалить: ";
				cin >> id;
				cerr << "" << endl;

				sqlDelete = "DELETE FROM " + nameTable + " WHERE id = " + id;
				cout << "Запись удалена!";
				cerr << "" << endl;
				rc = sqlite3_exec(db, sqlDelete.c_str(), NULL, NULL, &error);
				if (rc)
				{
					cerr << "Ошибка запроса к базе данных: " << sqlite3_errmsg(db) << endl << endl;
					sqlite3_free(error);
				}
				sqlite3_close(db);
				return;
			}
		}
	}
}

void clients() // клиенты 
{
	int comanda = 0;
	while (comanda != 5) {
		cout << "" << endl;
		cout << "Управление клиентами:" << endl;
		cout << "1. Просмотр клиентов" << endl;
		cout << "2. Добавить клиента" << endl;
		cout << "3. Редактировать клиента" << endl;
		cout << "4. Удалить клиента" << endl;
		cout << "5. Назад в Меню" << endl;
		cout << "" << endl;
		cout << "Команда: " << endl;

		cin >> comanda;

		if ((comanda != 1) && (comanda != 2) && (comanda != 3) && (comanda != 4) && (comanda != 5))  // && - логическое значение И.  Если функция НЕ выполняется или если результат неправильный, то условие срабатывает
			cout << "Введите правильную команду!!!\n";

		if (comanda == 1) viewTable("Clients", "0");
		if (comanda == 2) addTable("Clients");
		if (comanda == 3) updateTable("Clients");
		if (comanda == 4) deleteTable("Clients");
	}
}

void services() // услуги 
{
	int comanda = 0;
	while (comanda != 5) {
		cout << "" << endl;
		cout << "Управление услугами:" << endl;
		cout << "1. Просмотр услуг" << endl;
		cout << "2. Добавить услугу" << endl;
		cout << "3. Редактировать услугу" << endl;
		cout << "4. Удалить услугу" << endl;
		cout << "5. Назад в Меню" << endl;
		cout << "" << endl;
		cout << "Команда: " << endl;

		cin >> comanda;

		if ((comanda != 1) && (comanda != 2) && (comanda != 3) && (comanda != 4) && (comanda != 5)) // && - логическое значение И.  Если функция НЕ выполняется или если результат неправильный, то условие срабатывает
			cout << "Введите правильную команду!" << endl;

		if (comanda == 1) viewTable("Services", "0");
		if (comanda == 2) addTable("Services");
		if (comanda == 3) updateTable("Services");
		if (comanda == 4) deleteTable("Services");
	}
}

void orders() // заказы 
{
	int comanda = 0;
	while (comanda != 5) {
		cout << "" << endl;
		cout << "Управление заказами:" << endl;
		cout << "1. Просмотр заказов" << endl;
		cout << "2. Добавить заказ" << endl;
		cout << "3. Редактировать заказ" << endl;
		cout << "4. Удалить заказ" << endl;
		cout << "5. Назад в Меню" << endl;
		cout << "" << endl;
		cout << "Команда: ";

		cin >> comanda;

		if ((comanda != 1) && (comanda != 2) && (comanda != 3) && (comanda != 4) && (comanda != 5))// && - логическое значение И.  Если функция НЕ выполняется или если результат неправильный, то условие срабатывает
			cout << "Введите правильную команду!!!\n";

		if (comanda == 1) viewTable("Orders", "0");
		if (comanda == 2) addTable("Orders");
		if (comanda == 3) updateTable("Orders");
		if (comanda == 4) deleteTable("Orders");
	}
}

void reports() // подсчет выручки за день, месяц, год
{
	int comanda = 0;
	while (comanda != 7) {
		cout << "" << endl;
		cout << "Управление отчетами:" << endl;
		cout << "1. Подсчет выручки за текущий день" << endl;
		cout << "2. Подсчет выручки за текущий месяц" << endl;
		cout << "3. Подсчет выручки за текущий год" << endl;
		cout << "4. Подсчет количества посетителей за текущий день" << endl;
		cout << "5. Подсчет количества посетителей за текущий месяц" << endl;
		cout << "6. Подсчет количества посетителей за текущий год" << endl;
		cout << "7. Назад в Меню" << endl;
		cout << "" << endl;
		cout << "Команда: " << endl;

		cin >> comanda;

		if ((comanda != 1) && (comanda != 2) && (comanda != 3) && (comanda != 4) && (comanda != 5) && (comanda != 6) && (comanda != 7))  // && - логическое значение И.  Если функция НЕ выполняется или если результат неправильный, то условие срабатывает
			cout << "Введите правильную команду!!!" << endl;

		if (comanda == 1) viewTable("Reports", "1");
		if (comanda == 2) viewTable("Reports", "2");
		if (comanda == 3) viewTable("Reports", "3");
		if (comanda == 4) viewTable("Reports", "4");
		if (comanda == 5) viewTable("Reports", "5");
		if (comanda == 6) viewTable("Reports", "6");

	}
}

void firstOpenDB() // создание БД в проекте 
{
	string nameDatabase;
	ifstream in("config.txt");
	if (in.is_open())
	{
		while (getline(in, nameDatabase))
		{
			ifstream file;
			file.open(nameDatabase);
			file.close();
			if (file)
			{
				int rc;
				sqlite3* db;
				rc = sqlite3_open(nameDatabase.c_str(), &db);
				if (rc)
				{
					cerr << "Ошибка подключения к базе данных: " << sqlite3_errmsg(db) << endl << endl;
					sqlite3_close(db);
					system("pause");
					exit(0);
				}
				sqlite3_close(db);
			}
			else {
				cout << "Создать базу данных с названием " << nameDatabase << " ?" << endl;
				cout << "1 - Да" << endl;
				cout << "2 - Нет" << endl;
				cout << "" << endl;
				cout << "Команда: " << endl;
				int comanda;
				cin >> comanda;
				if (comanda == 1) {
					int rc;
					char* error;
					sqlite3* db;
					rc = sqlite3_open(nameDatabase.c_str(), &db);
					if (rc)
					{
						cerr << "Ошибка открытия базы данных: " << sqlite3_errmsg(db) << endl << endl;
						sqlite3_close(db);
						system("pause");
						exit(0);
					}
					const char* sqlCreateTable = "CREATE TABLE clients(id INTEGER PRIMARY KEY, lastname STRING, name STRING, patronymic STRING, gosnomer STRING, marka STRING); CREATE TABLE services(id INTEGER PRIMARY KEY, name STRING, cost INTEGER); CREATE TABLE orders(id INTEGER PRIMARY KEY, id_client INTEGER NOT NULL, id_service INTEGER NOT NULL, day INTEGER, month INTEGER, year INTEGER, FOREIGN KEY (id_client) REFERENCES clients(id), FOREIGN KEY (id_service) REFERENCES services(id));";
					rc = sqlite3_exec(db, sqlCreateTable, NULL, NULL, &error);
					if (rc)
					{
						cerr << "Ошибка запроса к базе данных: " << sqlite3_errmsg(db) << endl << endl;
						sqlite3_free(error);
					}
					cout << "База данных " << nameDatabase << " создана!!!" << endl << endl;
					sqlite3_close(db);
				}
				else if (comanda == 2) {
					cout << "В файле config.txt задайте свое название базы данных (например: carwash.db) и перезапустите программу!!!" << endl;
					system("pause");
					exit(0);
				}
				else {
					cout << "Перезапустите программу и выберите правильную команду!!!" << endl;
					system("pause");
					exit(0);
				}

			}
		}
		in.close(); // закрытие 
	}
	else {
		cout << "Отсутствует файл config.txt!!! В файле config.txt задайте свое название базы данных (например: carwash.db) и перезапустите программу!!!" << endl;
		system("pause");
		exit(0);
	}
}

int main() // меню 
{
	SetConsoleOutputCP(1251);   //  задаем кодировку для ввода символов с клавиатуры в консоль

	SetConsoleCP(1251); // задаем кодировку для вывода символов на экран

	int comanda = 0;

	firstOpenDB();
	while (comanda != 5) {   // Происходит  Логическое отрицание, НЕ
		cout << "" << endl;
		cout << "Меню:" << endl;
		cout << "1. Управление клиентами" << endl;
		cout << "2. Управление услугами" << endl;
		cout << "3. Управление заказами" << endl;
		cout << "4. Управление отчетами" << endl;
		cout << "5. Выход" << endl;
		cout << "" << endl;
		cout << "Команда: " << endl;

		cin >> comanda;

		if ((comanda != 1) && (comanda != 2) && (comanda != 3) && (comanda != 4) && (comanda != 5)) // Логическое отрицание, НЕ     // Логическое умножение, И
			cout << "Введите правильную команду!!!" << endl;

		if (comanda == 1) clients();
		if (comanda == 2) services();
		if (comanda == 3) orders();
		if (comanda == 4) reports();
	}
}