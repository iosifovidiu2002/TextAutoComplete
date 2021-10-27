#include "TextAutoComplete.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TextAutoComplete w;
    w.show();
    return a.exec();
}
