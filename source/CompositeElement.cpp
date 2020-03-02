#include <CompositeElement.hpp>

void CompositeElement::addElement(tsl::elm::Element *elem, s16 offsetX, s16 offsetY, s16 width, s16 height) {
    elem->setParent(this);
    m_elems.push_back({ elem, offsetX, offsetY, width, height });
    this->invalidate();
}

void CompositeElement::draw(tsl::gfx::Renderer *renderer) {
    for (auto &elem : m_elems) {
        elem.ptr->draw(renderer);
    }
}

void CompositeElement::layout(u16 parentX, u16 parentY, u16 parentWidth, u16 parentHeight) {
    this->setBoundaries(parentX, parentY, parentWidth, parentHeight);
    for (auto &elem : m_elems) {
        elem.ptr->setBoundaries(
            std::max(0, this->getX() + elem.offsetX),
            std::max(0, this->getY() + elem.offsetY),
            std::max(0, elem.width ? elem.width : parentWidth - elem.offsetX),
            std::max(0, elem.height ? elem.height : parentHeight - elem.offsetY));
        elem.ptr->invalidate();
    }
}

tsl::elm::Element* CompositeElement::requestFocus(tsl::elm::Element *oldFocus, tsl::FocusDirection direction) {
    if (m_elems.empty())
        return nullptr;

    // initialize
    if (direction == tsl::FocusDirection::None) {
        for (size_t i = 0; i < m_elems.size(); ++i) {
            auto innerElem = selectInitialElement(i);
            if (innerElem != nullptr)
                return innerElem;
        }
        return nullptr;
    }

    // focus within current outer element
    auto newElem = m_elems[m_focusIdx].ptr->requestFocus(oldFocus, direction);
    if (newElem != oldFocus) {
        return newElem;
    }

    // change outer element
    if (direction == tsl::FocusDirection::Up) {
        for (size_t i = m_focusIdx - 1; i >= 0; --i) {
            auto innerElem = selectInitialElement(i);
            if (innerElem != nullptr)
                return innerElem;
        }
    }
    else if (direction == tsl::FocusDirection::Down) {
        for (size_t i = m_focusIdx + 1; i < m_elems.size(); ++i) {
            auto innerElem = selectInitialElement(i);
            if (innerElem != nullptr)
                return innerElem;
        }
    }

    return nullptr;
}

tsl::elm::Element* CompositeElement::selectInitialElement(size_t idx) {
    auto initElem = m_elems[idx].ptr->requestFocus(nullptr, tsl::FocusDirection::None);
    if (initElem != nullptr) {
        m_focusIdx = idx;
        return initElem;
    }
    return nullptr;
}
