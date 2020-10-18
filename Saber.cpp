// Saber.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <random>

#include "MyList.h"

template<typename T>
void ToBinary(T value); //печатает бинарное представление объекта на экран
void RemoveDups(char* str); //удаляет повторяющиеся символы
void PrintNode(const ListNode& node); //печатает информацию о ноде на экран

int main()
{
//=========1=========
	uint32_t num1 = 0xAAAAAAAA; //2863311530
	std::cout << num1 << " (uint32_t) to binary: " << std::endl;
	ToBinary(num1);

	uint16_t num2 = 1;
	std::cout << num2 << " (uint16_t) to binary: " << std::endl;
	ToBinary(num2);

	std::cout << std::endl;

//=========2=========
	char to_remove[] = "ttttthhis sstring iiissss a lliittttllleee ssttrraannnngeeeeeeee";
	std::cout << to_remove << std::endl << "remove dups: " << std::endl;
	RemoveDups(to_remove);
	std::cout << to_remove << std::endl;

	std::cout << std::endl;

//=========3=========
	//Создаем ноды
	const int nodes_count = 15;
	std::vector<ListNode*> nodes;

	std::random_device r;
	std::default_random_engine generator(r());
	std::uniform_int_distribution<int> range(0, nodes_count / 2);

	for (size_t i = 0; i <= nodes_count; i++)
	{
		auto ptr = new ListNode();
		ptr->data = "Num " + std::to_string(i);
		if (i > nodes_count / 2)
		{
			ptr->rand = nodes[range(generator)];
		}
		nodes.push_back(ptr);
	}

	//создаем лист, заполняем нодами
	MyList list;
	for (size_t i = 0; i <= nodes_count; i++)
	{
		list.push_back(nodes[i]);
	}
	std::cout << "List" << std::endl;
	for (auto it = std::begin(list); it != std::end(list); it++)
		PrintNode(*it);
	std::cout << std::endl;

	const std::string filename("list.bin");

	//Открываем файл, серриализуем данные листа
	auto file = fopen(filename.c_str(), "wb");
	if (file == nullptr) 
		std::cout << "Open file issue" << std::endl;

	list.Serialize(file);
	fclose(file);

	//Открываем файл, дессериализуем данные листа
	file = fopen(filename.c_str(), "rb");
	if (file == nullptr)
		std::cout << "Open file issue" << std::endl;

	list.Deserialize(file);
	fclose(file);

	//Выводим на экран
	std::cout << "List after serialization/deserialization" << std::endl;
	for (auto it = std::begin(list); it != std::end(list); it++)
		PrintNode(*it);

	return 0;
}

template<typename T>
inline void ToBinary(T value)
{
	const uint32_t bits_count = sizeof(T) * 8;

	T acc(1);
	acc = acc << (bits_count - 1); //передвигаем бит на самую последнюю позицию

	printf("Bits:");
	for (int i = 0; i < bits_count; i++)
	{
		if (value & acc) //выясняем бит
			printf("%d", 1);
		else
			printf("%d", 0);
		value <<= 1;
	}
	printf("\n");
}

void RemoveDups(char* str)
{
	int write_it = 0; //итератор записи
	int it = 1; //итератор проверки
	char symbol = str[0]; //последний встреченный уникальный символ

	while (str[it] != 0)
	{
		if (str[it] != symbol)
		{
			write_it++;
			symbol = str[it];
			str[write_it] = symbol;
		}
		it++;
	}
	str[write_it + 1] = 0; //установить конец строки
}

void PrintNode(const ListNode & node)
{
	std::string prev_data = "Null";
	std::string next_data = "Null";
	std::string rand_data = "Null";

	if (node.prev) prev_data = node.prev->data;
	if (node.next) next_data = node.next->data;
	if (node.rand) rand_data = node.rand->data;

	std::cout << "Prev: " << prev_data <<
		"\tMy: " << node.data <<
		"\tNext: " << next_data <<
		"\tRand: " << rand_data <<
		std::endl;
}