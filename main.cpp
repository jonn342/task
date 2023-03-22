#include <fstream>
#include <map>
#include <string>
#include <iostream>
#include <algorithm>


int main()
{
	//proc/meminfo
	std::ifstream fin("/proc/meminfo", std::ios::in);
	std::multimap<unsigned long, std::string> info_from_file;

	try
	{
		if (!fin.is_open())
			throw std::runtime_error("File not found.");

		while (!fin.eof())
		{
			std::string cur_str;
			std::getline(fin, cur_str);

			if (cur_str.empty()) continue;

			cur_str = cur_str.erase(cur_str.find_first_of(':'), 1);

			size_t pos = cur_str.find_first_of(' ');

			auto iter = std::find_if(cur_str.begin() + pos, cur_str.end(),
				[](char current) { return isdigit(current); });

			unsigned long key = 0;
			if (*iter != '0')
			{
				while (*iter != ' ')
				{
					key += (*iter - '0');
					if (*++iter != ' ')
						key *= 10;
				}
			}
			else
				*iter = '-';

			info_from_file.insert({ key, cur_str });
		}
		fin.close();

		std::ofstream fout("/proc/meminfo_new", std::ios::out);
		if (!fout.is_open())
			throw std::runtime_error("Cant open or create file.");

		for (const auto& el : info_from_file)
			fout << el.second << '\n';

		fout.close();
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what();
		return -1;
	}

	return 0;
}