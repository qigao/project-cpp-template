#pragma once

#include "thing.hpp"
#include <QObject>
#include <qqml.h>

class Singleton : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT
    Q_PROPERTY(Thing* thing READ getThing CONSTANT)

    inline static Thing* getThing()
    {
        static Thing thing;
        return &thing;
    }
};
