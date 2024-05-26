#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <string_view>
#include <typeindex>
#include <sstream>

struct Entry
{
	std::string Data{};
	std::string status{ "never used" };
};										// Entry structured data type for table, needs to be defined outside class if you intend to use the heap



class HashTable							// table to organise all the data
{
public:
	static constexpr int Size{ 26 };

public:
	HashTable() = default;

	~HashTable()
	{
		delete[] m_Entries;				// prevent memory leak
	}

	void Add(std::string_view entryName)
	{
		bool exists{ Find(entryName) };
		if (!exists)
		{
			int insertIndex{ GetInsertIndex(entryName) };

			m_Entries[insertIndex].Data = entryName;	// remove first character of "A" from the str
			m_Entries[insertIndex].status = "occupied";
		}
	}
	void Delete(std::string_view deleteName)
	{				
		bool exists{ Find(deleteName) };

		if (exists)
		{
			int index{ GetIndex(deleteName) };
			int repeat_search{ 27 };
			while (true && repeat_search--)
			{
				if (m_Entries[index].Data == deleteName)
				{
					m_Entries[index].Data = "";
					m_Entries[index].status = "tombstone";
					break;
				}

				index = (index + 1) % Size;				// allows wrape  around
			}
		}
	}


	void Print() const
	{

		for (int idx{ 0 }; idx < this->Size; ++idx)
		{
			if (m_Entries[idx].status != "never used")
				std::cout << ((m_Entries[idx].Data == "") ? "NULL" : m_Entries[idx].Data) << "(" << m_Entries[idx].status << ") ";	// print all data, even data marked for deletion
		}
	}

	friend std::ostream& operator<<(std::ostream& out, const HashTable& table);
private:
	Entry* m_Entries{ new Entry[26] };							// allocate memory on the heap


	int GetIndex(std::string_view entryName) const
	{
		return static_cast<int>(entryName.back() - 'a');		// get starting index for hash value
	}

	int GetInsertIndex(std::string_view entryName) const
	{
		int index{ GetIndex(entryName) };

		int repeat_search{ 27 };								// put in fail safe for infinite loop

		while (true && repeat_search--)
		{
			if (m_Entries[index].status == "never used" || m_Entries[index].status == "tombstone") return index;

			index = (index + 1) % Size;							// prevents overflow, allowing index to wrap around
		}

		return -1;
	}

	bool Find(std::string_view entryName) const
	{
		int index{ GetIndex(entryName) };
		int repeat_search{ 27 };
		

		while (true && repeat_search--)
		{
			if (m_Entries[index].Data == entryName) return true;
			if (m_Entries[index].status == "never used") return false;

			index = (index + 1) % Size;		// prevents overflow, allowing index to wrap around
		}

		return false;
	}
};

std::ostream& operator<<(std::ostream& out, const HashTable& table)				// overload "<<" operator for hastable.
{
	for (int idx{ 0 }; idx < table.Size; ++idx)
	{
		if (table.m_Entries[idx].status == "occupied")
		out << table.m_Entries[idx].Data << " ";
	}
	std::cout << std::endl;
	return out;
}

int main()
{
	// input	: Aapple Aorange Dapple Astrawberry
	// output	:	orange strawberry

	HashTable hashTable{};

	std::cout << "Enter an action (\"A\" for add, \"D\" delete) directly followed by a word to add it to a hash table\nExample: \"Aapple\" will add apple to the table, \"Dorange\" will delete orange if it is in the table.\nEnter data: ";

	std::string str_get{};
	std::getline(std::cin, str_get);
	std::stringstream ss{ str_get };
	
	std::string token{};
	while (ss.good())
	{
		ss >> token;												// get data

		if (token[0] == 'A')
			hashTable.Add(token.substr(1));							// subbstring is added to remove 'A'
		else if (token[0] == 'D')
			hashTable.Delete(token.substr(1));
	}
	hashTable.Print();												// Print the entirety of the table, including data marked for deletion.

	std::cout << '\n' << '\n' << hashTable << '\n';					// Print relevant data.

    return 0;
}

