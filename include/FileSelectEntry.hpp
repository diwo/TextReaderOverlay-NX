#pragma once

#include <string>

namespace fsel
{
    bool compareIgnoreCase(std::string s1, std::string s2);
    std::string toLowerCase(std::string str);

    class FileSelectEntry {
    public:
        FileSelectEntry(std::string path, bool favorite);
        virtual ~FileSelectEntry();

        std::string getPath() { return m_path; }
        std::string getName();
        std::string label() { return (isFavorite() ? "\u2605 " : "") + getName(); }
        void favorite();
        bool isFavorite() { return m_favorite; }
        virtual void select() = 0;

        bool operator <(FileSelectEntry &other) {
            if (ordering() < other.ordering())
                return true;
            else if (other.ordering() < ordering())
                return false;
            else
                return compareIgnoreCase(m_path, other.m_path);
        }

    protected:
        virtual int ordering() = 0;

    private:
        std::string m_path;
        bool m_favorite;
    };

    class FileSelectDirEntry : public FileSelectEntry {
    public:
        FileSelectDirEntry(std::string path, bool favorite);
        ~FileSelectDirEntry();

        void select() override;

    protected:
        int ordering() override { return 1; }
    };

    class FileSelectFileEntry : public FileSelectEntry {
    public:
        FileSelectFileEntry(std::string path, bool favorite);
        ~FileSelectFileEntry();

        void select() override;

    protected:
        int ordering() override { return 2; }
    };
}

using fsel::FileSelectEntry;
using fsel::FileSelectDirEntry;
using fsel::FileSelectFileEntry;
