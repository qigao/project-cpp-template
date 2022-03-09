#include "include/QtDemoClass.h"

#include <utility>

namespace app {

QtDemoClass::QtDemoClass(QString text) : _text{ std::move(text) } {}

QString QtDemoClass::appendIt(const QString &extra) const { return _text + " " + extra; }

QString QtDemoClass::text() const
{
  // this block is purposely left untested to make sure
  // coverage never reports 100%
  return _text;
}

}// namespace app
