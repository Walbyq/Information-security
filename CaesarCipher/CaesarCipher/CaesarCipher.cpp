#include <iostream>
#include <string>
using namespace std;

string cipher(string str, int key) {
	string result = "";

	for (int i = 0; i < str.length(); i++) {
		// проверка, является ли символ буквой
		if (isalpha(str[i])) {
			// если да, то проверяем регистр и записываем букву со сдвигом в новую строку
				char ch = islower(str[i]) ? 'a' : 'A';
				result += static_cast<char>(((str[i] + key - ch) % 26 + 26) % 26 + ch);
		}
		else
			// если нет, то записываем без изменений
			result += str[i];
	}
	return result;
}

string cracker(string str, char ch2) {
	const int MAX_SIZE = 26;
	unsigned int counter[MAX_SIZE];
	int key, max = 0;
	char maxch = 0, ch = 'a';

	// записываем в массив количество повторений каждого символа
	for (int i = 0; i < MAX_SIZE; i++) {
		counter[i] = 0;
		for (int j = 0; j < str.length(); j++) {
			if (str[j] == ch || str[j] == static_cast<char>(ch - 32))
				counter[i]++;
		}
		// находим символ, повторяющийся чаще остальных
		if (counter[i] > max) {
			max = counter[i];
			maxch = ch;
		}
		// переход к следующему символу
		ch = static_cast<char>(ch + 1);
	}
	// вычисляем ключ
	key = static_cast<int>(ch2 - maxch);

	return cipher(str, key);
}

int main() {
	setlocale(LC_ALL, "Russian");

	int i = 0;
	char choise, ch = 'a';
	char popularLet[] = {
		'e', 't', 'a', 'o', 'i', 'n', 's', 'h', 'r', 'd', 
		'l', 'c', 'u', 'm', 'w', 'f', 'g', 'y', 'p', 'b', 
		'v', 'k', 'j', 'x', 'q', 'z'};
	string str = 
		"\nThe Caesar cipher is named for Julius Caesar, who used an alphabet where decrypting would shift three letters to the left.";
	int key = 3;

	// зашифрованная строка
	string encryptedString = cipher(str, key);
	cout << "Encrypted text: " << encryptedString << endl;

	// расшифрованная строка
	string decryptedString = cipher(encryptedString, -key);
	cout << "\nDecrypted text: " << decryptedString << endl;

	// взломанная строка
	do {
		cout << "\nCracked text: " << cracker(encryptedString, popularLet[i]) << endl;
		cout << "\nContinue? (y/n): ";
		cin >> choise;
		i++;
	} while (choise != 'n' && i < 26);

	return 0;
}