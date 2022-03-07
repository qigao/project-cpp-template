#pragma once
#include "MyLib_export.h"
#include <QString>
namespace app
{

class MYLIB_EXPORT QtDemoClass
{
    QString _text;

public:
    explicit QtDemoClass(QString  text);

    QString appendIt(const QString& extra) const;
    QString text() const;
};

}
