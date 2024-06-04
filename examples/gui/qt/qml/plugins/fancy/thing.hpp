#pragma once

#include <QObject>
#include <qqml.h>

class Thing : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString description READ getDescription CONSTANT)
    QML_ELEMENT
public:
    explicit Thing(QObject* parent = nullptr) {}
    QString getDescription() const;
};
