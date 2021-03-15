#ifndef MY_LAYOUT_H
#define MY_LAYOUT_H

#include <QtWidgets>
#include <QLayout>
#include <QRect>
#include <iostream>

class MyLayout  : public QLayout
{
public:
    enum Position
    {
        West, North, South, East, Center
    };

    MyLayout(int space);
    MyLayout(int space, QWidget *parent, const QMargins &margins = QMargins());
    ~MyLayout();

    void add(QLayoutItem *item, Position position) ;
    void addWidget(QWidget *widget, Position position);
    void addItem(QLayoutItem *item);

    Qt::Orientations expandingDirections() const override;

    QSize sizeHint() const override;        // QObj
    QSize minimumSize() const override;     // QObj
    QLayoutItem *itemAt(int) const override;    // not QObj
    QLayoutItem *takeAt(int) override;          // not QObj
    void setGeometry(const QRect &rect) override;
    int count() const;

private:
    struct ItemWrapper
    {
        QLayoutItem *s_item;
        Position s_position;
        ItemWrapper (QLayoutItem *item, Position position)
        {
            s_item = item;
            s_position = position;
        }
    };
    
    enum SizeType
    {
        e_mini_type,
        e_size_hint,
    };

    QSize calculateSize(SizeType size_type) const;
    QList <ItemWrapper*> m_list;
    
};

















// Xac dinh moi quan he parent - child. Khi xoa parent thi child cung phai deconstructor.
/*
class Base
{

public:
    Base *m_child;
    Base(std::string string) 
    {
        std::cout <<string;
    }

    Base(Base *parent = nullptr)
    {
        std::cout <<"Construct Base!\n";
        parent->m_child = this;
    }
    
    ~Base()
    {
        if (m_child != nullptr)
        {
            std::cout <<"DeConstruct Child!\n";
            delete m_child;
        }
        std::cout <<"DeConstruct Base!\n";
    }
    void print()
    {
        std::cout <<"Something !\n";
    }

    // int main
    {
        Base *base_parent = new Base{"Construct Base!\n"};
        Base *base_child = new Base(base_parent);
        base_child->print();
        delete base_parent;
    }
};
*/
#endif