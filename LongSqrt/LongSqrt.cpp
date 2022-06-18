﻿#include <iostream>
#include <string>
#include <fstream>
using namespace std;

void del_zero(string &S) // функция для удаления незначащих нулей из исходного числа
{
	if (S.length() > 1 && S[0] == '0')
	{
		while (S[0] == '0')
			S.erase(0, 1);
	}
}

string oborona(string S)
{
	string P = "";
	if (S == "")
	{
		P = "Файл пуст!";
	}
	if (S[0] == '0' && S.length() == 1)
		P = "Число не должно являться нулем!";
	if (S.length() <= 3001)
	{
		for (int i = 0; i < S.length(); i++)
		{
			if (S[i] > '9' || S[i] < '0')
			{
				P = "В файле не должно быть лишних символов, только натуральное число!";
			}
		}
	}
	if (S.length() == 3001)
	{
		if (S[0] > '1' && S[0] <= '9')
			P = "Число не должно превышать 10^3000!";
		else if (S[0] == '1')
		{
			for (int i = 1; i < 3001; i++)
			{
				if (S[i] > '0' && S[i] <= '9')
					P = "Число не должно превышать 10^3000!";
			}
		}
	}
	if (S.length() > 3001)
	{
		P = "Превышено допустимое количество символов. Число не должно превышать 10^3000!";
	}
	return P;
}

int sravnenie(string Sub, string V) // функция для сравнения
{
	if (Sub.length() > V.length())
	{
		while (Sub.length() != V.length())
		{
			V = '0' + V; // дополняем вычитаемое незначащими нулями, если в нем меньше разрядов, чем в "подчисле"
		}
	}
	if (Sub.length() == V.length())
	{
		for (int i = 0; i < Sub.length(); i++) // поразрядное сравнение
		{
			if (Sub[0] <= V[0])
			{
				if (Sub[i] > V[i])
				{
					return 0;
				}
				if (Sub[i] < V[i])
				{
					return -1;
				}
			}
		}
	}
	return 0;
}

string minus_Sub(string Sub, string V) // функция для вычитания
{

	int n1 = Sub.length(), n2 = V.length();

	string RaznS = "";

	reverse(Sub.begin(), Sub.end()); // переворачиваем "подчисло"
	reverse(V.begin(), V.end()); // переворачиваем вычитаемое

	for (int i = n2; i < n1; i++) // дополняем вычитаемое незначащими нулями, если в нем меньше разрядов, чем в "подчисле"
		V.push_back('0');

	int mns = 0; // остаток от деления на 10 этой переменной будет результатом разницы разрядов
	for (int i = 0; i < n1; i++)
	{
		mns = mns + (Sub[i] - '0') - (V[i] - '0') + 10;
		RaznS.push_back(mns % 10 + '0');
		if (mns <= 9)
			mns = -1; // если разряд "подчисла" меньше разряда вычитаемого, то занимаем единицу у старшего разряда "подчисла"
		else
			mns = 0;
	}

	reverse(RaznS.begin(), RaznS.end()); // не забываем перевернуть строку, чтобы получить нужное число
	return RaznS;
}

string plus_Two(string V) // функция для прибавления двойки
{
	int n1 = V.length();

	string SumV = "";

	reverse(V.begin(), V.end()); // переворачиваем число для сложения столбиком

	int perenos = 0; // переменная, куда будет записываться остаток от сложения
	int sum = (V[0] - '0') + 2 + perenos;
	SumV.push_back(sum % 10 + '0'); // от получившейся суммы берем остаток от деления на 10 и добавляем в конец строки
	perenos = sum / 10; // делим сумму на 10 нацело - получаем остаток для переноса в следующий разряд
	for (int i = 1; i < n1; i++) // в этом цикле, если остаток равен нулю, цифры в разрядах числа остаются без изменений
	{
		int sum = ((V[i] - '0') + perenos);
		SumV.push_back(sum % 10 + '0');
		perenos = sum / 10;
	}

	if (perenos != 0) SumV.push_back(perenos + '0'); // если остаток в итоге не равен нулю, то тоже добавляем его в конец строки
	reverse(SumV.begin(), SumV.end()); // не забываем перевернуть строку, чтобы получить нужное число
	return SumV;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	ifstream fin("input.txt");
	ofstream fout("output.txt");
	string S;
	fin >> S; // считываем число из файла в виде строки
	del_zero(S); // удаляем незначащие нули, если такие имеются
	if (oborona(S) != "")
	{
		cout << oborona(S) << endl;
		fout << oborona(S);
	}
	else
	{
		cout << "Число, из которого извлекается корень:" << endl;
		cout << S << endl;

		int N = S.length(); // количество цифр в числе
		cout << "Количество цифр в числе = " << N << endl;
		string Sub;
		if (N % 2 == 0) // если количество цифр чётное, берём за "подчисло" два старших разряда, если нет - один
		{
			Sub.push_back(S[0]);
			Sub.push_back(S[1]);
		}
		else
		{
			Sub.push_back(S[0]);
		}
		string V = "1"; // первое вычитаемое число
		string One = "1"; // понадобится после каждого цикла вычитаний

		string kor;
		int k = 0; // счетчик количества вычитаний

		while (V.length() <= Sub.length() && sravnenie(Sub, V) != -1) // в этом цикле вычитаем V из "подчисла" до тех пор, пока V не станет больше "подчисла"
		{
			Sub = minus_Sub(Sub, V); // вычитаем из "подчисла" нечетное число
			V = plus_Two(V); // увеличиваем это нечетное число на 2
			k++;
		}
		kor.push_back(k + '0'); // Получившееся число вычитаний - первая цифра корня, добавляем её в строку
		k = 0;

		V = minus_Sub(V, One); // Так как вычитаемое на последнем шаге лишний раз увеличивается на 2, а нам надо лишь на 1, вычитаем 1
		V.push_back('1'); // умножаем вычитаемое на 10 и прибавляем 1 (по сути, просто дописываем в конец единицу)

		int i = 0;
		if (N > 2)
		{
			if (N % 2 != 0) // определяем, от какой по счёту цифры шагаем после первого цикла вычитаний
				i = 1;
			else
				i = 2;
			for (i; i < N; i += 2)
			{
				Sub.push_back(S[i]);
				Sub.push_back(S[i + 1]); // после каждого цикла вычитаний добавляем к "подчислу" по две цифры от исходного числа

				while (V.length() <= Sub.length() && sravnenie(Sub, V) != -1)
				{
					Sub = minus_Sub(Sub, V);
					V = plus_Two(V);
					k++;
				}
				kor.push_back(k + '0'); // получившееся число вычитаний - это следующая цифра корня, добавляем её в конец строки
				k = 0;

				V = minus_Sub(V, One); // Так как вычитаемое на последнем шаге лишний раз увеличивается на 2, а нам надо лишь на 1, вычитаем 1
				V.push_back('1'); // умножаем вычитаемое на 10 и прибавляем 1 (по сути, просто дописываем 1 в конец)
			}
		}
		cout << "Корень равен:" << endl << kor << endl;
		fout << kor;
	}
}