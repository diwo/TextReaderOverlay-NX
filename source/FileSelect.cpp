#include <FileSelect.hpp>

#include <filesystem>

#include <FileSelectEntry.hpp>
#include <CompositeElement.hpp>
#include <Config.hpp>
#include <Utils.hpp>

namespace fs = std::filesystem;

FileSelect::FileSelect(std::string const &path)
    : m_path(path),
      m_failed(false)
{
    auto favorites = Config::read()["favorites"];
    if (!favorites.is_array()) {
        favorites = {};
    }
    auto isFavorite = [favorites](std::string const &path) {
        auto found = std::find(favorites.begin(), favorites.end(), path);
        return found != favorites.end();
    };

    for (auto &entry : fs::directory_iterator(path)) {
        std::string absolutePath = entry.path();
        if (entry.is_directory()) {
            absolutePath.append("/");
            m_entries.push_back(new FileSelectDirEntry(absolutePath, isFavorite(absolutePath)));
        }
        else if (entry.is_regular_file()) {
            if (utils::toLowerCase(entry.path().extension()) == ".txt")
                m_entries.push_back(new FileSelectFileEntry(absolutePath, isFavorite(absolutePath)));
        }
    }

    std::sort(
        m_entries.begin(), m_entries.end(),
        [](FileSelectEntry *a, FileSelectEntry *b) { return *a < *b; });
}

FileSelect::~FileSelect() {
    for (auto it = m_entries.begin(); it != m_entries.end(); ++it) {
        delete *it;
    }
}

tsl::elm::Element* FileSelect::createUI() {
    auto frame = new FileSelectFrame();

    auto composite = new CompositeElement();

    composite->addElement(new tsl::elm::CustomDrawer([this](tsl::gfx::Renderer* renderer, u16 x, u16 y, u16 w, u16 h) {
        renderer->drawString(m_path.c_str(), false, 20, 80, 12, a(0xFFFF));
    }));

    auto list = new tsl::elm::List(7);

    for (auto it = m_entries.begin(); it != m_entries.end(); ++it) {
        auto item = new tsl::elm::ListItem((*it)->label());
        item->setClickListener([it, item](u64 keys) {
            if (keys & KEY_A) {
                (*it)->select();
                return true;
            }
            else if (keys & KEY_Y) {
                (*it)->toggleFavorite();
                item->setText((*it)->label());
                return true;
            }
            return false;
        });
        list->addItem(item);
    }

    composite->addElement(list, 40, 110, tsl::cfg::FramebufferWidth - 80);

    frame->setContent(composite);

    return frame;
}

void FileSelectFrame::draw(tsl::gfx::Renderer *renderer) {
    renderer->fillScreen(a({ 0x0, 0x0, 0x0, 0xD }));
    renderer->drawString("Select File...", false, 20, 50, 30, a(0xFFFF));
    renderer->drawRect(15, 720 - 73, tsl::cfg::FramebufferWidth - 30, 1, a(0xFFFF));
    renderer->drawString("\uE0E3  Favorite", false, 30, 693, 23, a(0xFFFF));

    if (this->m_contentElement != nullptr)
        this->m_contentElement->frame(renderer);
}
