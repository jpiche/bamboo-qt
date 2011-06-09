#include <QApplication>

#include "bamboo_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    BambooWindow w;
    w.show();
    return app.exec();
}
