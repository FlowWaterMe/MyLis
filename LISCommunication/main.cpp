#include "LISCommunication.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <QFile>
#include <QDialog>
#include <QtGui> 
#include <QtQml/QQmlApplicationEngine>
#include <Qurl>
void SetAppStyleSheet(QApplication &App);
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("System");    //获取系统编码
    QTextCodec::setCodecForLocale(codec);
	SetAppStyleSheet(a);
    LISCommunication w;
    w.show();
    return a.exec();	
}


void SetAppStyleSheet(QApplication &App)
{
	QString qsFullName = QApplication::applicationDirPath() + QString::fromUtf8("/analyser.qss");
	QFile QssFile(qsFullName);

	bool bRtn = false;
	bRtn = QssFile.open(QFile::ReadOnly);
	if (bRtn)
	{
		App.setStyleSheet(QssFile.readAll());
		QssFile.close();
	}

	return;
}