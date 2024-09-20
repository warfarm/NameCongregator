#include <iostream>
#include <fstream>
#include <filesystem>

#include <string>
#include <string_view>
#include <sstream>

#include <cassert>
#include <vector>
#include <algorithm>

#include <set>

#include "NCong.h"

bool compareSubstring(const std::string& a, const std::string& b) {
	// Extract substrings up to the first comma
	/*std::istringstream issA(a);
	std::istringstream issB(b);
	std::string substrA, substrB;
	std::getline(issA, substrA, ',');
	std::getline(issB, substrB, ',');*/
	// Compare the extracted substrings

	//std::cout << "Comparing: " << a << ' ' << b << '\n';

	return a < b;
}

std::set<std::string> readFile(const std::string& filename) {

	std::cout << "Extracting File Contents..." << '\n';

	// Open File
	std::ifstream file(filename);
	
	// Check File Validity
	if (!file) {
		throw std::runtime_error("Failed to open file");
	}

	std::vector<char> characters{};
	char c;

	std::vector<std::string> buffer;
	std::string line;
	while (std::getline(file, line)) {

		while (file >> c) {
			if (c == ',')
				break;
			characters.push_back(c);
		}

		buffer.emplace_back(characters.begin(), characters.end());
		characters.clear();
	}

	std::cout << "Finished Extraction..." << '\n';
	std::cout << "Commencing Alphabeticalization..." << '\n';

	// Sort the vector using the custom comparison function
	std::sort(buffer.begin(), buffer.end(), compareSubstring);

	std::cout << "Alphabeticalized..." << '\n';

	return std::set<std::string>{ buffer.begin(), buffer.end() };
}

bool concatFile(const std::string_view& fFile, const std::string_view& sFile)
{
	// Check File Validity
	assert(fileExists(fFile) && "File Does not Exist");
	if (!fileExists(fFile)) { return false; }

	std::ofstream	ofile(static_cast<std::string>(sFile), std::ios_base::binary | std::ios_base::app);
	std::ifstream	ifile(static_cast<std::string>(fFile), std::ios_base::binary);

	ofile.seekp(0, std::ios_base::end);
	ofile << ifile.rdbuf() << '\n';

	ofile.close();

	return true;
}

bool writeFile(const std::set<std::string> fileData, const std::string& fileName)
{
	std::cout << "Updating File..." << '\n';

	std::ofstream file(fileName, std::ofstream::out | std::ofstream::trunc);

	// Check File Validity
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file for writing");
	}

	for (const auto& str : fileData) {
		//std::cout << "Sorting " << str << '\n';
		file << str << '\n';
	}

	file.close(); // It's good practice to close the file explicitly

	std::cout << "File Updated..." << '\n';

	return true;
}

bool alphabeticalize(const std::string& fileName)
{
	std::cout << "Alphabeticalizing..." << '\n';
	return writeFile(readFile(fileName), fileName);
}

bool fileExists(const std::string_view& fileName)
{
	return std::filesystem::exists(fileName);
}

int main()
{
	// Declare Start and End File Parameters
	constexpr int startYear	{ 1880 };
	constexpr int endYear	{ 2022 };

	// Create File Prefix
	constexpr std::string_view filePrefix{ "Names/yob" };
	const std::string outFile{ "Names/nameList.txt" };

	//Create Modifiable String outside loop to avoid a creation of a string every iteration
	std::string fName{};

	for (int year{ startYear }; year <= endYear; ++year)
	{
		//Modify String
		fName = static_cast<std::string>(filePrefix) + (std::to_string(year) + ".txt");

		//Check if the File was fully processed
		if (!concatFile(fName, outFile)) {
			std::cout << "Failed File Concatenation yob" << year << '\n';
			return 1;
		}
		std::cout << "Concatenated File yob" << year << '\n';
	}

	//Check if alphabeticalization successful
	if (!alphabeticalize(outFile))
		return 1;

	std::cout << "Operation Finished" << '\n';

	return 0;

}