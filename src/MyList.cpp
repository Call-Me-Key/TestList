#include "MyList.h"
#include <vector>

MyList::iterator::iterator(ListNode * node)
{
	ptr = node;
}

MyList::iterator MyList::iterator::operator++()
{
	ptr = ptr->next;
	return *this;
}

MyList::iterator MyList::iterator::operator++(int)
{
	iterator tmp = *this;
	operator++();
	return tmp;
}

MyList::iterator MyList::iterator::operator--()
{
	ptr = ptr->prev;
	return *this;
}

MyList::iterator MyList::iterator::operator--(int)
{
	iterator tmp = *this;
	operator--();
	return tmp;
}

bool MyList::iterator::operator==(const iterator & other) const
{
	return ptr == other.ptr;
}

bool MyList::iterator::operator!=(const iterator & other) const
{
	return !(*this==other);
}

ListNode MyList::iterator::operator*()
{
	return *ptr;
}

ListNode * MyList::iterator::operator->()
{
	return ptr;
}

MyList::iterator MyList::begin()
{
	return iterator(m_head);
}

MyList::iterator MyList::end()
{
	return iterator(nullptr);
}

MyList::~MyList()
{
	clear();
}

void MyList::push_front(ListNode* new_node)
{
	new_node->prev = nullptr;
	if (m_count == 0)
	{
		m_head = new_node;
		m_tail = new_node;
	}
	else
	{
		new_node->next = m_head;
		m_head->prev = new_node;
		m_head = new_node;
	}
	m_count++;
}

void MyList::push_back(ListNode* new_node)
{
	new_node->next = nullptr;
	if (m_count == 0)
	{
		m_head = new_node;
		m_tail = new_node;
	}
	else
	{
		new_node->prev = m_tail;
		m_tail->next = new_node;
		m_tail = new_node;
	}
	m_count++;
}

int MyList::size() const
{
	return m_count;
}

void MyList::clear()
{
	if (m_count == 0)
		return;

	auto it = std::begin(*this);
	while (it != std::end(*this))
	{
		auto ptr = it.ptr;
		it++;
		delete ptr;
	}
	m_head = nullptr;
	m_tail = nullptr;
	m_count = 0;
}

void MyList::Serialize(FILE * file)
{
	fwrite(&m_count, sizeof(m_count), 1, file);
	if (m_count == 0)
		return;
	//записать data и переписать random как ID элементов
	int id = 0;
	for (auto it = std::begin(*this); it != std::end(*this); it++)
	{
		*reinterpret_cast<int*>(&(it->prev)) = id; //переписать поинтер как ID
		id++;
		
		size_t str_size = it->data.size(); 
		fwrite(&str_size, sizeof(str_size), 1, file); //длина данных
		fwrite(it->data.data(), sizeof(char), str_size, file); //данные
	}
	//записать в файл поля random
	for (auto it = std::begin(*this); it != std::end(*this); it++)
	{
		int id = 0;
		if (it->rand == nullptr)
			id = -1;
		else 
			id = *reinterpret_cast<int*>(&(it->rand->prev));
		fwrite(&id, sizeof(id), 1, file); //rand
	}
	//восстановить лист
	ListNode* prev = nullptr;
	for (auto it = std::begin(*this); it != std::end(*this); it++)
	{
		it->prev = prev;
		prev = it.ptr;
	}
}
void MyList::Deserialize(FILE * file)
{
	clear();

	int length = 0;
	fread(&length, sizeof(length), 1, file); //десерриализация размера листа
	if (length <= 0)
		return;

	std::vector<ListNode*> pointers; //временное хранилище указателей
	pointers.reserve(length);

	//десериализация data
	for (size_t i = 0; i < length; i++) 
	{
		auto node = new ListNode();

		pointers.push_back(node);
		push_back(node);

		std::string data;
		size_t str_size = 0;
		fread(&str_size, sizeof(str_size), 1, file);
		data.resize(str_size);
		fread(data.data(), sizeof(char), str_size, file);

		node->data = data;
	}
	//десериализация rand
	for (auto it = std::begin(*this); it != std::end(*this); it++)
	{
		int id = 0;
		fread(&id, sizeof(id), 1, file);

		if (id < 0)
			it->rand = nullptr;
		else
			it->rand = pointers[id];
	}
}