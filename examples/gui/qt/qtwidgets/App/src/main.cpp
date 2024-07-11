#include "MainWindow.h"
#include "QtDemoClass.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

static int const width = 640;
static int const height = 480;
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QWidget widget;
    widget.resize(width, height);
    widget.setWindowTitle("Hello, world!!!");

    auto* gridLayout = new QGridLayout(&widget);

    app::QtDemoClass obj("Hello");
    auto* label = new QLabel(obj.appendIt("library world!!"));

    auto* image = new QImage(":/images/example.png");
    auto* imglbl = new QLabel();
    imglbl->setPixmap(QPixmap::fromImage(*image));
    imglbl->setAlignment(Qt::AlignCenter);

    auto* btn1 = new QPushButton("Push Me");
    QObject::connect(btn1, &QPushButton::released, &widget,
                     []()
                     {
                         auto* dialog = new app::MainWindow();
                         dialog->setModal(true);
                         dialog->setWindowModality(Qt::ApplicationModal);
                         dialog->exec();
                     });

    label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    gridLayout->addWidget(imglbl);
    gridLayout->addWidget(label);
    gridLayout->addWidget(btn1);

    widget.show();

    return app.exec();
}
