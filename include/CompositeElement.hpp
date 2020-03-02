#pragma once

#include <tesla.hpp>
#include <vector>

class CompositeElement : public tsl::elm::Element {
public:
    virtual ~CompositeElement() {
        for (auto &elem : m_elems) {
            delete elem.ptr;
        }
    }

    void addElement(tsl::elm::Element *elem, s16 offsetX = 0, s16 offsetY = 0, s16 width = 0, s16 height = 0);

    void draw(tsl::gfx::Renderer *renderer) override;
    void layout(u16 parentX, u16 parentY, u16 parentWidth, u16 parentHeight) override;
    virtual tsl::elm::Element* requestFocus(tsl::elm::Element *oldFocus, tsl::FocusDirection direction) override;

private:
    tsl::elm::Element* selectInitialElement(size_t idx);

private:
    struct ElementWrapper {
        tsl::elm::Element *ptr;
        s16 offsetX, offsetY, width, height;
    };

    std::vector<ElementWrapper> m_elems;
    size_t m_focusIdx = 0;
};
