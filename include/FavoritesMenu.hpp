#pragma once

#include <tesla.hpp>
#include <list>

#include <FileSelectEntry.hpp>

class FavoritesMenu : public tsl::Gui {
public:
    FavoritesMenu();
    ~FavoritesMenu();

    tsl::Element* createUI();
    void preDraw(tsl::Screen *screen) override;

private:
    std::list<FileSelectEntry*> m_entries;
};
