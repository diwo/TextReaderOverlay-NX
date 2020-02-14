#pragma once

#include <tesla.hpp>
#include <string>
#include <list>

#include <FileSelectEntry.hpp>

namespace fsel
{
    bool endsWith(std::string str, std::string suffix);

    class FileSelect : public tsl::Gui {
    public:
        FileSelect(std::string path);
        ~FileSelect();

        tsl::Element* createUI() override;
        void preDraw(tsl::Screen *screen) override;

    private:
        std::string m_path;
        std::list<FileSelectEntry*> m_entries;
        bool m_failed;
    };
}

using fsel::FileSelect;
