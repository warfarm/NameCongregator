#ifndef NAMECONG_H
#define NAMECONG_H

std::set<std::string> readFile(const std::string& fileName);
bool compareSubstring(const std::string& a, const std::string& b);

bool concatFile(const std::string_view& fFile, const std::string_view& sFile);
bool writeFile(const std::set<std::string> fileData, const std::string& fileName);
bool alphabeticalize(const std::string& fileName);

inline bool fileExists(const std::string_view& fileName);

#endif