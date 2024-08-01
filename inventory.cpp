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
    QFile file("inventory.txt");

    if(!file.open(QIODevice::WriteOnly)){
        qCritical() << "Could not save file" << file.errorString();
        return;
    }

    QDataStream stream(&file);

    //explicity define what version of encoded file to work with.
    stream.setVersion(QDataStream::Qt_6_0);

    int len = m_items.size(); //no. of items in the map

    stream  << len;

    foreach(QString name, m_items.keys()){
        qInfo() << "Saving: " << name;
        stream << name;
        stream << m_items.value(name);
    }
    /*
     Order of items:
    No of items.
    Key
    Value
    */

    file.close();

    qInfo() << "File saved";

}


void Inventory::load() {
    QFile file("inventory.txt");

    //QFile goes out of scope it closes automatically
    if(!file.exists()){
        qWarning() << "File does not exist";
        return;
    }

    if(!file.open(QIODevice::ReadOnly)){
        qCritical() << "Could not load file" << file.errorString();
        return;
    };

    QDataStream stream(&file);

    if(stream.version() != QDataStream::Qt_6_0){
        qCritical() << "Wrong data stream version!";
        file.close();
        return;
    }
    m_items.clear(); //remove everything from the map

    int len;
    stream >> len;

    qInfo() << "Number of items to load: "<< len ;

    for(int i = 0 ; i < len ; i++){
        QString name;
        int qty;
        stream >> name;
        stream >> qty;
        m_items.insert(name,qty);
    }

    file.close();

    qInfo() << "File loaded.";
}



