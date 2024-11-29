#include <iostream>
#include <vector>
#include <pqxx/pqxx>
#include <string>
#include <windows.h>
#include <tuple>

class phonebook {

	pqxx::connection* con;
	std::string n;
	std::string s;
	std::string e;
	std::string t;
	std::string nt;
	std::string nn;
	std::string ns;

	void name() {
		std::cout << "Enter name\n";
		std::cin >> n;
		std::cout << "Enter surname\n";
		std::cin >> s;
	}

	void email() {
		std::cout << "Enter email\n";
		std::cin >> e;
	}

	void number() {
		std::cout << "Enter telephon number\n";
		std::cin >> t;
	}

	void new_name() {
		std::cout << "Enter new name\n";
		std::cin >> nn;
		std::cout << "Enter new surname\n";
		std::cin >> ns;
	}

	void new_number() {
		std::cout << "Enter new number\n";
		std::cin >> nt;
	}

	void clear() {
		n.clear();
		s.clear();
		e.clear();
		t.clear();
		nn.clear();
		ns.clear();
		nt.clear();
	}

public:
	phonebook() {
		try {
			con = new pqxx::connection(
				"host=localhost "
				"port=5432 "
				"dbname=postgres "
				"user=postgres "
				"password=1234"
			);
		}
		catch (const std::exception& ex) {
			std::cout << ex.what();
		}
	}

	~phonebook() {
		delete con;
	}

	void add_client() {
		pqxx::work w(*con);
		name();
		email();
		number();
		auto add_client = w.exec("INSERT INTO bd (name, surname, email) VALUES " 
			"('" + w.esc(n) + "', '" + w.esc(s) + "', '" + w.esc(e) + "');");
		auto add_phone = w.exec("INSERT INTO bd_tel (id, name, surname, num) VALUES "
		"((SELECT id FROM bd WHERE name = '" + w.esc(n) + "' AND surname = '" + w.esc(s) + "'), '" + w.esc(n) + "', '" + w.esc(s) + "', '" + w.esc(t) + "');");
		w.commit();
		clear();
	}
	void add_phone() {
		pqxx::work w(*con);
		name();
		number();
		try {
			auto add_phone = w.exec("INSERT INTO bd_tel (id, name, surname, num) VALUES "
				"((SELECT id FROM bd WHERE name = '" + w.esc(n) + "' AND surname = '" + w.esc(s) + "'), '" + w.esc(n) + "', '" + w.esc(s) + "', '" + w.esc(t) + "');");
		}
		catch (const std::exception& ex) {
			std::cout << ex.what();
		}
		w.commit();
		clear();
	}
	void change() {
		pqxx::work w(*con);
		name();
		try {
			auto find = w.query<std::string, std::string, std::string>(
				"SELECT name, surname, num FROM bd_tel WHERE name='" + w.esc(n) + "' AND surname='" + w.esc(s) + "';");
			std::cout << "Choise number. Write:\n";
			for (auto [name, surname, number] : find) {
				std::cout << name << '\t' << surname << '\t' << number << '\n';
			}
			number();
			new_name();
			email();
			new_number();

			auto change1 = w.exec("UPDATE bd_tel SET name='" + w.esc(nn) + "', surname='" + w.esc(ns) + "' WHERE name ='" + w.esc(n) + "' "
				"AND surname='" + w.esc(s) + "'; ");
			auto change11 = w.exec("UPDATE bd_tel SET num='" + w.esc(nt) + "' WHERE num='" + w.esc(t) + "'; ");
			auto change2 = w.exec("UPDATE bd SET name='" + w.esc(nn) + "', surname='" + w.esc(ns) + "', email='" + w.esc(e) + "' "
				"WHERE name = '" + w.esc(n) + "' AND surname = '" + w.esc(s) + "'; ");
		}
		catch(const std::exception& ex){
			std::cout << ex.what();
		}
		w.commit();
		clear();
	}
	void remove_phone() {
		pqxx::work w(*con);
		std::tuple<std::string, std::string, std::string> data;
		std::string num{ '0' };
		name();
		try {
			auto find = w.query<std::string, std::string, std::string>("SELECT name, surname, num FROM bd_tel WHERE name='" + w.esc(n) + "' AND surname='" + w.esc(s) + "';");
			std::cout << "Choise number for delete:\n";
			for (auto [name, surname, tel] : find) {
				std::cout << name << '\t' << surname << "\t" << tel << "\n";
			}
			std::cin >> num;
			auto remove = w.exec("DELETE FROM bd_tel WHERE num='" + w.esc(num) + "'; ");
		}
		catch (const std::exception& ex) {
			std::cout << ex.what();
		}
		w.commit();
		clear();
	}
	void remove_client() {
		pqxx::work w(*con);
		std::string id;
		name();
		try {
			auto find = w.query_value<std::string>("SELECT id FROM bd WHERE name='" + w.esc(n) + "' AND surname='" + w.esc(s) + "';");
			id = find;
			auto remove = w.exec("DELETE FROM bd_tel WHERE id=" + id + "; DELETE FROM bd WHERE id=" + id + ";");
		}
		catch (const std::exception& ex) {
			std::cout << ex.what();
		}
		w.commit();
		clear();
	}
	void create() {
		pqxx::work w{ *con };
		try {
			auto create = w.exec("create table if not exists bd ("
				"id serial primary key not null, "
				"name varchar(40) not null, "
				"surname varchar(40) not null, "
				"email varchar(40)"
				"); "
				"create table if not exists bd_tel("
				"id int not null references bd(id), "
				"name varchar(40) not null, "
				"surname varchar(40) not null, "
				"num varchar(40)"
				"); "
				"insert into bd(name, surname, email) values('Ivan', 'Petrov', 'ip@mail.com'); "
				"insert into bd(name, surname, email) values('John', 'Johnes', 'jj@mail.com'); "
				"insert into bd_tel(id, name, surname, num) values(1, 'Ivan', 'Petrov', '5737493'); "
				"insert into bd_tel(id, name, surname, num) values(2, 'John', 'Johnes', '3545444'); "
				"insert into bd_tel(id, name, surname, num) values(1, 'Ivan', 'Petrov', '3456565'); "
				"insert into bd(name, surname, email) values('Kate', 'Moss', 'km@mail.com'); "
				"insert into bd_tel(id, name, surname, num) values(3, 'Kate', 'Moss', '35333'); "
				"insert into bd_tel(id, name, surname, num) values(3, 'John', 'Johnes', '7854333'); "
			);			
		}
		catch (const std::exception& ex) {
			std::cout << ex.what();
		}
		w.commit();
	}
	void find(){
		pqxx::work w(*con);
		std::vector<std::tuple<std::string, std::string, std::string, std::string>> data;
		int ch = 0;
		std::cout << "Choise variant of find, or any number for exit\n1 - use name and surname\n2 - use email\n3 - use telephone number\n";
		std::cin >> ch;
		if (ch == 1) {
			name();
		}
		else if (ch == 2) {
			email();
		}
		else if (ch == 3) {
			number();
		}
		else {
			return;
		}
		try {
			auto find = w.query<std::string, std::string, std::string, std::string>(
				"SELECT b.name, b.surname, b.email, bt.num FROM bd b "
				"JOIN bd_tel bt ON b.id = bt.id "
				"WHERE b.name = '" + w.esc(n) + "' AND b.surname = '" + w.esc(s) + "' OR b.email = '" + w.esc(e) + "' OR bt.num = '" + w.esc(t) + "';"
			);
			std::cout << '\n';
			for (auto [name, surname, email, tel] : find) {
				std::cout << name << '\t' << surname << '\t' << email << '\t' << tel << '\n';
			}
			std::cout << '\n';
		}
		catch (const std::exception& ex) {
			std::cout << ex.what();
		}
		clear();
		w.commit();
	}

};

int main() {
	setlocale(LC_ALL, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	phonebook ph;
	int choise = 1;
	while (choise != 0) {
		std::cout << "Enter number of query type or \'0\' for end to use:\n";
		std::cout << " 1 - add client \n 2 - add telephone number\n 3 - update data of client\n 4 - delete telephone number\n 5 - delete of client\n 6 - Find client\n 7 - Create bd(with data)\n";
		std::cin >> choise;
		switch (choise) {
		case 1: ph.add_client(); break;
		case 2: ph.add_phone(); break;
		case 3: ph.change(); break;
		case 4: ph.remove_phone(); break;
		case 5: ph.remove_client(); break;
		case 6: ph.find(); break;
		case 7: ph.create(); break;
		case 0: choise = 0; break;
		default: break;
		}
	}
	return 0;
}