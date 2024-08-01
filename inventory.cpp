#include "inventory.h"

Inventory::Inventory(QObject *parent)
    : QObject{parent}
{
    load();
}

void Inventory::add(QString name, int qty)
{
    if(m_items.contains(name)){
        m_items[name] += qty;
    }
    else{
        m_items.insert(name,qty);
        qInfo() << "Item added";
    }
    emit save();
}

void Inventory::remove(QString name, int qty){
    if(m_items.contains(name)){
        m_items[name] -= qty;
        if(m_items.value(name) < 0) {
            m_items.remove(name);
            qInfo() << "Item deleted";
        }
    }else{
        qWarning() << "No item with that name exists!";
    }
}

void Inventory::list(){
    qInfo() << "Items:" << m_items.size();
    foreach(QString name ,m_items.keys()){
        qInfo() <<  name<<  " = " << m_items.value(name);
    };
}

void Inventory::save()
{
}

void Inventory::load() {
}



