#include "my_layout.h"

MyLayout::MyLayout(int space)
{
    QLayout::setSpacing(space);
}
MyLayout::MyLayout(int space, QWidget *parent, const QMargins &margins):
    QLayout(parent)
{
    QLayout::setSpacing(space);
    QLayout::setContentsMargins(margins);
}


MyLayout::~MyLayout()
{
    QLayoutItem *l;
    while ((l = this->takeAt(0)))
    {
        delete l;
    }
}

int MyLayout::count() const
{
    // QVector::size() returns the number of QLayoutItems in m_items
    return m_list.size();
}

Qt::Orientations MyLayout::expandingDirections() const
{
    return Qt::Horizontal | Qt::Vertical;
}

QLayoutItem* MyLayout::itemAt(int idx) const
{
    ItemWrapper *wrapper = m_list.value(idx);
    return wrapper ? wrapper->s_item : nullptr; // Tra ve con Tro toi m_item trong list

    // QList::value() performs index checking, and returns nullptr if we are
    // outside the valid range

}

QLayoutItem* MyLayout::takeAt(int idx) 
{
    // CHecking idx co nam trong day idx item khong
    if (idx >= 0 && idx < m_list.size())
    {
        ItemWrapper *wrapper = m_list.takeAt(idx);
        return wrapper->s_item;             // Tra ve con tro toi m_item trong lis
    }
    return nullptr;
    
} 

void MyLayout::addWidget(QWidget *widget, Position position)
{
    add(new QWidgetItem(widget), position);
}
void MyLayout::add(QLayoutItem *item, Position position)
{
    m_list.append(new ItemWrapper(item, position));
}
void MyLayout::addItem(QLayoutItem *item)
{
    add(item, Position::West);
}

QSize MyLayout::sizeHint() const 
{
    return this->calculateSize(SizeType::e_size_hint);
}   

QSize MyLayout::minimumSize() const
{
    return this->calculateSize(SizeType::e_mini_type);
}

QSize MyLayout::calculateSize(SizeType size_type) const
{
    QSize totalSize;
    for (int i {0}; i < m_list.size(); ++ i)
    {
        ItemWrapper *wrapper = m_list.at(i);                       // Temporary pointer variable    
        Position temp_position = wrapper->s_position;
        QSize itemSize;

        if (size_type == SizeType::e_mini_type)
            itemSize = wrapper->s_item->minimumSize();
        else // (sizeType == SizeHint)
            itemSize = wrapper->s_item->sizeHint();

        if (temp_position == North || temp_position == South || temp_position == Center)
            totalSize.rheight() += itemSize.height();

        if (temp_position == West || temp_position == East || temp_position == Center)
            totalSize.rwidth() += itemSize.width();
    }

    return totalSize;
}
void MyLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    std::cout << rect.width() << std::endl;
}