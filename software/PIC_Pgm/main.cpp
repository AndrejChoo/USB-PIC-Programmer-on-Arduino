#include "picpgm.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PicPgm w;
    w.show();
    return a.exec();
}
