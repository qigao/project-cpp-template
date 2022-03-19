import QtQuick.Controls 2.15
import com.mycompany.test 1.0

ApplicationWindow {
  id: root

  visible: true
  width: 480
  height: 320
  title: thing.description

  Thing {
    id: thing
  }

  FancyComponent {}
}
