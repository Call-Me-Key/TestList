#include <string>

struct ListNode 
{
	ListNode *      prev;
	ListNode *      next;
	ListNode *      rand; // ��������� �� ������������ ������� ������� ������, ���� NULL
	std::string     data;
};

class MyList
{
public:
	class iterator
	{
	public:
		iterator(ListNode* node);

		iterator operator++();
		iterator operator++(int);
		iterator operator--();
		iterator operator--(int);
		bool operator==(const iterator& other) const;
		bool operator!=(const iterator& other) const;
		ListNode operator*();
		ListNode * operator->();

		ListNode* ptr = nullptr;
	};

	MyList() = default;
	~MyList();

	iterator begin();
	iterator end();

	void push_front(ListNode* new_node);
	void push_back(ListNode* new_node);
	int size() const;
	void clear();

	void Serialize(FILE * file);  // ���������� � ���� (���� ������ � ������� fopen(path, "wb"))
	void Deserialize(FILE * file);  // �������� �� ����� (���� ������ � ������� fopen(path, "rb"))

private:
	ListNode* m_head = nullptr;
	ListNode* m_tail = nullptr;
	int m_count = 0;
};