#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;
const int BITS_64 = 64; //длина 64-битного сообщения
const int BITS_56 = 56; //длина 56-битного ключа
const int BITS_48 = 48; //длина 48-битного ключа

//начальная перестановка ключа
vector<bool> getInitalKeySwap(vector<bool> key_64) {
	vector<bool> key_56; //новый 56-битный ключ

	//таблица перестановок PC-1
	const int PC1[] = {
		57, 49, 41, 33, 25, 17, 9,
		1, 58, 50, 42, 34, 26, 18,
		10, 2, 59, 51, 43, 35, 27,
		19, 11, 3, 60, 52, 44, 36,
		63, 55, 47, 39, 31, 23, 15,
		7, 62, 54, 46, 38, 30, 22,
		14, 6, 61, 53, 45, 37, 29,
		21, 13, 5, 28, 20, 12, 4 
	};

	//перестановка битов по таблице PC-1
	for (int i = 0; i < BITS_56; i++) key_56.push_back(key_64[PC1[i] - 1]);

	return key_56;
};

//перестановка битов ключа
vector<bool> rearrangeKeyBits(vector<bool> key_56, int num) {
	vector<bool> left_key; //левая половина ключа
	vector<bool> right_key; //правая половина ключа

	//делим ключ на левую и правую части
	left_key.insert(left_key.end(), key_56.begin(), key_56.end() - BITS_56 / 2);
	right_key.insert(right_key.end(), key_56.begin() + BITS_56 / 2, key_56.end());

	key_56.clear();

	//таблица сдвигов битов ключа
	const int SHIFT_TABLE[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

	//определение величины сдвига
	int shift = 0;
	for (int i = 0; i < num; i++) {
		shift += SHIFT_TABLE[i];
	}

	//цикл на величину сдвига
	for (int i = 0; i < shift; i++) {
		//храним в памяти 0-й бит
		bool temp_left = left_key[0];
		bool temp_right = right_key[0];
		//сдвиг на 1 бит влево
		for (int j = 0; j < BITS_56 / 2 - 1; j++) {
			left_key[j] = left_key[j + 1];
			right_key[j] = right_key[j + 1];
		}
		//последний бит принимает значение 0-го
		left_key[27] = temp_left;
		right_key[27] = temp_right;
	}

	//объединение частей в целый ключ
	key_56.insert(key_56.end(), left_key.begin(), left_key.end());
	key_56.insert(key_56.end(), right_key.begin(), right_key.end());

	//таблица перестановок PC-2
	const int PC2[] = {
		14, 17, 11, 24, 1, 5,
		3, 28, 15, 6, 21, 10,
		23, 19, 12, 4, 26, 8,
		16, 7, 27, 20, 13, 2,
		41, 52, 31, 37, 47, 55,
		30, 40, 51, 45, 33, 48,
		44, 49, 39, 56, 34, 53,
		46, 42, 50, 36, 29, 32
	};

	vector<bool> key_48;

	//перестановка битов по таблице PC-2
	for (int i = 0; i < BITS_48; i++) key_48.push_back(key_56[PC2[i] - 1]);

	return key_48;
}

//начальная перестановка сообщения
vector<bool> getInitalTextSwap(vector<bool> text) {
	//таблица перестановок IP
	const int IP[] = {
		58, 50, 42, 34, 26, 18, 10, 2,
		60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6,
		64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17, 9, 1,
		59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5,
		63, 55, 47, 39, 31, 23, 15, 7
	};

	vector<bool> temp_text;
	temp_text.insert(temp_text.end(), text.begin(), text.end());
	text.clear();

	//перестановка битов по таблице IP
	for (int i = 0; i < BITS_64; i++) text.push_back(temp_text[IP[i] - 1]);

	return text;
}

vector<bool> rearrangeTextBits(vector<bool> text, vector<bool> key) {
	vector<bool> left_text; //левая половина текста
	vector<bool> right_text; //правая половина текста

	//делим текст на левую и правую части
	left_text.insert(left_text.end(), text.begin(), text.end() - BITS_64 / 2);
	right_text.insert(right_text.end(), text.begin() + BITS_64 / 2, text.end());
	text.clear();

	//сохраним значение правой части, которая потом перейдет в левую
	vector<bool> old_right_text;
	old_right_text.insert(old_right_text.end(), right_text.begin(), right_text.end());

	//таблица выбора бит E
	const int E[] = {
		32, 1, 2, 3, 4, 5,
		4, 5, 6, 7, 8, 9,
		8, 9, 10, 11, 12, 13,
		12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21,
		20, 21, 22, 23, 24, 25,
		24, 25, 26, 27, 28, 29,
		28, 29, 30, 31, 32, 1
	};

	//расширяем правую часть с 32 до 48 битов с помощью таблицы E
	vector<bool> right_text_48;
	for (int i = 0; i < BITS_48; i++) right_text_48.push_back(right_text[E[i] - 1]);
	right_text.clear();

	//производим сложение XOR ключа с расширенной частью текста
	vector<bool> right_text_xor_key;
	for (int i = 0; i < BITS_48; i++) right_text_xor_key.push_back(right_text_48[i] ^ key[i]);

	//таблицы перестановок S для каждого блока правой части ключа
	const int S[8][4][16] = {
		{	{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
			{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
			{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
			{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}	},
		
		{	{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
			{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
			{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
			{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}	}, 
		
		{	{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
			{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
			{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
			{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}	}, 
		
		{	{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
			{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
			{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
			{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}	}, 
		
		{	{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
			{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
			{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
			{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}	}, 
		
		{	{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
			{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
			{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
			{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}	}, 
		
		{	{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
			{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
			{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
			{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}	}, 
		
		{	{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
			{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
			{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
			{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}	}
	};

	for (int i = 0; i < BITS_48; i = i + 6 ) {
		vector<bool> binar_row, binar_column;
		int row = 0, column = 0;
		
		binar_row.push_back(right_text_xor_key[i]); //номер строки определяем по первому
		binar_row.push_back(right_text_xor_key[i + 5]); //и последнему элементу 6-битного блока
		binar_column.insert(binar_column.end(), right_text_xor_key.begin() + i + 1, right_text_xor_key.begin() + i + 5); //номер столбца определяем по четырем средним элементам блока

		//перевод в десятичное число битов, определяющих строку
		for (int j = 0; j < 2; j++)
			if (binar_row[2 - j - 1]) row += pow(2, j);

		//перевод в десятичное число битов, определяющих столбец
		for (int j = 0; j < 4; j++) 
			if (binar_column[4 - j - 1]) column += pow(2, j);

		//выбираем таблицу S1-S8 и по ней получаем новый блок, записанный в десятичном виде
		int num = S[i / 6][row][column];

		//переводим полученное значение в двоичную запись числа, получая блок длиной 4 бита
		vector<bool> block;
		while (num >= 1) {
			block.push_back(num % 2);
			num = (num - num % 2) / 2;
		};

		//запись в блок недостающих битов
		while (block.size() < 4) block.push_back(0);
		
		//переворачиваем блок и сохраняем его в тексте
		for (int j = 3; j >= 0; j--) right_text.push_back(block[j]);
		block.clear();
	}
	
	//сохраняем значения правой части
	vector<bool> temp_right;
	for (int i = 0; i < BITS_64 / 2; i++) temp_right.push_back(right_text[i]);
	right_text.clear();

	const int P[] = {
		16, 7, 20, 21,
		29, 12, 28, 17,
		1, 15, 23, 26,
		5, 18, 31, 10,
		2, 8, 24, 14,
		32, 27, 3, 9,
		19, 13, 30, 6,
		22, 11, 4, 25,
	};

	//применяем к ней таблицу перестановок P
	for (int i = 0; i < BITS_64 / 2; i++) right_text.push_back(temp_right[P[i] - 1]);
	temp_right.clear();

	//сохраняем значения правой и левой части текста
	vector<bool> temp_left;
	temp_right.insert(temp_right.end(), right_text.begin(), right_text.end());
	temp_left.insert(temp_left.end(), left_text.begin(), left_text.end());
	right_text.clear();
	left_text.clear();

	//левый блок принимает значение старого правого
	left_text.insert(left_text.end(), old_right_text.begin(), old_right_text.end());

	//правый блок получаем путем XOR сложения предыдущей левой частью и функцией от правой
	for (int i = 0; i < BITS_64 / 2; i++) right_text.push_back(temp_right[i] ^ temp_left[i]);

	//соединяем обе части в один текст
	text.insert(text.end(), left_text.begin(), left_text.end());
	text.insert(text.end(), right_text.begin(), right_text.end());

	return text;
}

//окончательная перестановка текста
vector<bool> getFinalTextSwap(vector<bool> text) {
	vector<bool> left_text, right_text;

	//делим текст на левую и правую части
	left_text.insert(left_text.end(), text.begin(), text.end() - BITS_64 / 2);
	right_text.insert(right_text.end(), text.begin() + BITS_64 / 2, text.end());
	text.clear();

	//записываем левую и правую части в текст, меняя местами
	text.insert(text.end(), right_text.begin(), right_text.end());
	text.insert(text.end(), left_text.begin(), left_text.end());

	//таблица перестановок IP^(-1)
	const int INVERSE_IP[] = {
		40, 8, 48, 16, 56, 24, 64, 32,
		39, 7, 47, 15, 55, 23, 63, 31,
		38, 6, 46, 14, 54, 22, 62, 30,
		37, 5, 45, 13, 53, 21, 61, 29,
		36, 4, 44, 12, 52, 20, 60, 28,
		35, 3, 43, 11, 51, 19, 59, 27,
		34, 2, 42, 10, 50, 18, 58, 26,
		33, 1, 41,  9, 49, 17, 57, 25
	};

	//сохраняем значение текста
	vector<bool> temp_text;
	temp_text.insert(temp_text.end(), text.begin(), text.end());
	text.clear();

	//выполняем перестановки по таблице IP^(-1)
	for (int i = 0; i < BITS_64; i++) text.push_back(temp_text[INVERSE_IP[i] - 1]);

	return text;
}

vector<bool> desCipher(vector<bool> text, vector<bool> key_64) {
	const int ITERS = 16; //количество итераций

	//начальная перестановка ключа
	vector<bool> key_56 = getInitalKeySwap(key_64);

	//начальная перестановка текста
	text = getInitalTextSwap(text);

	int counter = 1;
	vector<bool> key_48;

	//преобразование сетью Фейстеля
	for (int i = 0; i < ITERS; i++) {
		key_48 = rearrangeKeyBits(key_56, i + 1);
		text = rearrangeTextBits(text, key_48);
	}

	//финальная перестановка текста
	text = getFinalTextSwap(text);

	return text;
}

int main() {
	setlocale(LC_ALL, "Russian");

	//изначальный 64-битный ключ в двоичной записи
	vector<bool> key_64 { 
		0, 0, 0, 1, 0, 0, 1, 1, 
		0, 0, 1, 1, 0, 1, 0, 0, 
		0, 1, 0, 1, 0, 1, 1, 1, 
		0, 1, 1, 1, 1, 0, 0, 1, 
		1, 0, 0, 1, 1, 0, 1, 1 ,
		1, 0, 1, 1, 1, 1, 0, 0, 
		1, 1, 0, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 0, 0, 0, 1 };

	//открытый текст в двоичной записи
	vector<bool> text{
		0, 0, 0, 0, 0, 0, 0, 1,
		0, 0, 1, 0, 0, 0, 1, 1,
		0, 1, 0, 0, 0, 1, 0, 1,
		0, 1 ,1, 0, 0, 1, 1, 1,
		1, 0, 0, 0, 1, 0, 0, 1,
		1, 0, 1, 0, 1, 0, 1, 1,
		1, 1, 0, 0, 1, 1, 0, 1,
		1, 1, 1, 0, 1, 1, 1, 1
	};

	//Вывод открытого текста
	cout << "Открытый текст: \n";
	for (int i = 0; i < BITS_64; i++) {
		if (i % 8 == 0 && i != 0) cout << " ";
		cout << text[i];
	}
	cout << "\n";

	//шифрование
	text = desCipher(text, key_64);

	//вывод зашифрованного текста в двоичной записи
	cout << "Зашифрованный текст:\n";
	for (int i = 0; i < 64; i++) {
		if (i % 8 == 0 && i != 0) cout << " ";
		cout << text[i];
	}
	cout << "\n";

	return 0;
}
