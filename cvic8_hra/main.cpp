#include "cvic8_hra.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    cvic8_hra window;
    window.show();
    return app.exec();
}
