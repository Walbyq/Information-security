#include <iostream>
#include <cmath>
#include <cstring>
#include <ctime>
#include <cstdlib>
using namespace std;

int text[100]; //открытый текст
long long cipherText[100]; //зашифрованный текст
int n, e = 0, d;

//двоичное преобразование
int bianaryTransform(int num, int bin_num[]) {
	int i = 0, mod = 0;

	//преобразуется в двоичный, обратный временно сохраняется в массиве temp []
	while (num != 0) {
		mod = num % 2;
		bin_num[i] = mod;
		num = num / 2;
		i++;
	}

	//возвращает количество цифр в двоичных числах
	return i;
}

//повторное возведение в квадрат в степень
long long modular_Exonentiation(long long a, int b, int n) {
	int c = 0, bin_num[1000];
	long long d = 1;
	int k = bianaryTransform(b, bin_num) - 1;

	for (int i = k; i >= 0; i--) {
		c = 2 * c;
		d = (d * d) % n;
		if (bin_num[i] == 1) {
			c = c + 1;
			d = (d * a) % n;
		}
	}
	return d;
}

//генерация простых чисел в пределах 1000
int producePrimeNumber(int prime[]) {
	int c = 0, vis[1001];
	memset(vis, 0, sizeof(vis));
	for (int i = 2; i <= 1000; i++)if (!vis[i]) {
		prime[c++] = i;
		for (int j = i * i; j <= 1000; j += i)
			vis[j] = 1;
	}

	return c;
}

//расширенный алгоритм Евклида
int exgcd(int m, int n, int& x) {
	int x1, y1, x0, y0, y;
	x0 = 1; y0 = 0;
	x1 = 0; y1 = 1;
	x = 0; y = 1;
	int r = m % n;
	int q = (m - r) / n;
	while (r) {
		x = x0 - q * x1; y = y0 - q * y1;
		x0 = x1; y0 = y1;
		x1 = x; y1 = y;
		m = n; n = r; r = m % n;
		q = (m - r) / n;
	}
	return n;
}

//инициализация RSA
void RSA_Initialize() {
	//вынимаем простые числа в пределах 1000 и сохраняем их в массиве prime []
	int prime[5000];
	int count_Prime = producePrimeNumber(prime);

	//случайно возьмем два простых числа p, q
	srand((unsigned)time(NULL));
	int ranNum1 = rand() % count_Prime;
	int ranNum2 = rand() % count_Prime;
	int p = prime[ranNum1], q = prime[ranNum2];
	n = p * q;
	int On = (p - 1) * (q - 1);

	//используем расширенный алгоритм Евклида, чтобы найти e, d
	for (int j = 3; j < On; j += 1331) {
		int gcd = exgcd(j, On, d);
		if (gcd == 1 && d > 0) {
			e = j;
			break;
		}
	}
}

//шифрование
void RSA_Encrypt() {
	cout << "Открытый ключ (e, n) : e = " << e << " n = " << n << '\n';
	cout << "Секретный ключ (d, n) : d = " << d << " n = " << n << '\n' << '\n';

	int i = 0;
	for (i = 0; i < 100; i++)
		cipherText[i] = modular_Exonentiation(text[i], e, n);

	cout << "Зашифрованное сообщение:" << '\n';
	for (i = 0; i < 100; i++)
		cout << cipherText[i] << " ";
	cout << '\n' << '\n';
}

//расшифровка
void RSA_Decrypt() {
	int i = 0;
	for (i = 0; i < 100; i++)
		cipherText[i] = modular_Exonentiation(cipherText[i], d, n);

	cout << "Расшифрованное сообщение:" << '\n';
	for (i = 0; i < 100; i++)
		cout << cipherText[i] << " ";
	cout << '\n' << '\n';
}

//инициализация алгоритма
void initialize() {
	int i;
	srand((unsigned)time(NULL));
	for (i = 0; i < 100; i++)
		text[i] = rand() % 1000;

	cout << "Открытый текст:" << '\n';
	for (i = 0; i < 100; i++)
		cout << text[i] << " ";
	cout << '\n' << '\n';
}

int main() {
	setlocale(LC_ALL, "Russian");
	initialize();
	while (!e)
		RSA_Initialize();
	RSA_Encrypt();
	RSA_Decrypt();

	return 0;
}
