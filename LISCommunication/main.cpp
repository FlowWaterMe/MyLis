#include "LISCommunication.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("System");    //获取系统编码
    QTextCodec::setCodecForLocale(codec);
    LISCommunication w;
    w.show();
    return a.exec();	
}
