#pragma once

#include <tesla.hpp>
#include <string>
#include <filesystem>

#include <FileSelect.hpp>
#include <TextReader.hpp>
#include <Utils.hpp>

namespace fs = std::filesystem;

class FileSelectEntry {
public:
    FileSelectEntry(std::string const &path, bool favorite)
        : m_path(path), m_favorite(favorite) {}

    virtual ~FileSelectEntry() {}

    std::string getPath() const { return getPathInternal(); }

    std::string label() const {
        return (isFavorite() ? "\u2605 " : "") + getName();
    }

    void toggleFavorite();
    bool isFavorite() const { return m_favorite; }

    bool operator<(FileSelectEntry const &other) const;

    virtual std::string getName() const = 0;
    virtual void select() = 0;

protected:
    fs::path getPathInternal() const { return m_path; }
    virtual int ordering() const = 0;

private:
    fs::path m_path;
    bool m_favorite;
};

class FileSelectDirEntry : public FileSelectEntry {
public:
    FileSelectDirEntry(std::string const &path, bool favorite)
        : FileSelectEntry(path, favorite) {}

    std::string getName() const {
        return getPathInternal().parent_path().filename().string() + "/";
    }

    void select() override {
        tsl::changeTo<FileSelect>(getPath());
    }

protected:
    int ordering() const override { return 1; }
};

class FileSelectFileEntry : public FileSelectEntry {
public:
    FileSelectFileEntry(std::string const &path, bool favorite)
        : FileSelectEntry(path, favorite) {}

    std::string getName() const {
        return getPathInternal().filename();
    }

    void select() override {
        tsl::changeTo<TextReader>(getPath());
    }

protected:
    int ordering() const override { return 2; }
};
