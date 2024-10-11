#include <iostream>
#include <string>

using namespace std;

const string ABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string LOWER_ABC = "abcdefghijklmnopqrstuvwxyz";

string edit(string str) {
	str.erase(remove(str.begin(), str.end(), ' '), str.end());

	for (int i = 0; i < str.length(); i++)
		if (islower(str[i])) str[i] = ABC[LOWER_ABC.find(str[i])];
	return str;
}

string convertKey(string str, string k) {
	int key_length = k.length();
	for (int i = k.length(); i < str.length(); i++)
		k += k[i - key_length];
	return k;
}

string encryption(string str, string k) {
	string encrypted_str = "";
	int index;
	for (int i = 0; i < str.length(); i++) {
		index = (ABC.find(str[i]) + ABC.find(k[i])) % ABC.length();
		encrypted_str += ABC[index];
	}
	return encrypted_str;
}

string decryption(string encrypted_str, string k) {
	string decrypted_str = "";
	int index;
	for (int i = 0; i < encrypted_str.length(); i++) {
		index = (ABC.length() + ABC.find(encrypted_str[i]) - ABC.find(k[i])) % ABC.length();
		decrypted_str += ABC[index];
	}
	return decrypted_str;
}

int main() {
	setlocale(LC_ALL, "Russian");
	string text, word_key, key, encrypted_text, decrypted_text;

	cout << "Введите сообщение (EN) : ";
	getline(cin, text);
	text = edit(text);

	cout << "Введите ключевое слово (EN) : ";
	getline(cin, word_key);
	word_key = edit(word_key);
	
	key = convertKey(text, word_key);

	system("cls");

	cout << "Сообщение: " << text << endl
		<< "Ключевое слово: " << word_key << endl
		<< "Ключ: " << key << endl;

	encrypted_text = encryption(text, key);
	decrypted_text = decryption(encrypted_text, key);
	cout << "Зашифрованное сообщение: " << encrypted_text << endl
		<< "Расшифрованное сообщение: " << decrypted_text << endl;

	return 0;
}
