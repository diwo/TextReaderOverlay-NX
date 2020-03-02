#pragma once

#include <tesla.hpp>
#include <string>
#include <vector>

class FileSelectEntry;

class FileSelect : public tsl::Gui {
public:
    FileSelect(std::string const &path);
    ~FileSelect();

    tsl::elm::Element* createUI() override;

private:
    std::string m_path;
    std::vector<FileSelectEntry*> m_entries;
    bool m_failed;
};

class FileSelectFrame : public tsl::elm::OverlayFrame {
public:
    FileSelectFrame() : OverlayFrame("", "") {}

    void draw(tsl::gfx::Renderer *renderer) override;
};
