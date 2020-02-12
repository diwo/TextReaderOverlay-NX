#pragma once

#include <tesla.hpp>
#include <string>
#include <list>

class FileSelect : public tsl::Gui {
public:
    FileSelect(std::string path);
    ~FileSelect();

    tsl::Element* createUI() override;

private:
    static bool endsWith(std::string str, std::string suffix);
    static bool compareIgnoreCase(std::string s1, std::string s2);
    static std::string toLowerCase(std::string str);

private:
    std::string m_path;
    std::list<std::string> m_dirs;
    std::list<std::string> m_textFiles;
    bool m_failed;
};
