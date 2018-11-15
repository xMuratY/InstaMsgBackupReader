#include "InstaMsgBackupReader.h"
#include <QtWidgets/QApplication>
#include <Windows.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	InstaMsgBackupReader w;
	w.show();
	return a.exec();
}