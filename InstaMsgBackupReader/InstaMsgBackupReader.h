#pragma once

#include <QtWidgets/QMainWindow>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qjsondocument.h>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qtextstream.h>
#include <qdebug.h>
#include "ui_InstaMsgBackupReader.h"
#include <vector>

class InstaMsgBackupReader : public QMainWindow
{
	Q_OBJECT

public:
	InstaMsgBackupReader(QWidget *parent = Q_NULLPTR);


public slots:
	void openBackupFile();
	void onConversationSelect(QListWidgetItem* item);

private:
	Ui::InstaMsgBackupReaderClass ui;
	QString msgFile;
	QJsonDocument messagesJson;
	std::vector<QJsonArray> Conversations;
};
