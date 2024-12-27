// zipcracker.h
#ifndef ZIPCRACKER_H
#define ZIPCRACKER_H

#include <string>
#include <vector>

bool crack_zip_password(const std::string& zip_filename, const std::string& password);
bool test_password(const std::string& zip_filename, const std::string& password);
bool load_password_list(const std::string& password_list_file, std::vector<std::string>& password_list);

#endif // ZIPCRACKER_H
