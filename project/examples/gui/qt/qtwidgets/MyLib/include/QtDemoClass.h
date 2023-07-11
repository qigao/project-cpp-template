#pragma once
#include <QString>
#include "QtDemoLib_export.h"

namespace app {

class QTDEMOLIB_EXPORT QtDemoClass {
  QString _text;

 public:
  explicit QtDemoClass(QString text);

  QString appendIt(const QString& extra) const;
  QString text() const;
};

}  // namespace app
