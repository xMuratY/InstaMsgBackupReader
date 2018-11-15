#include "InstaMsgBackupReader.h"

InstaMsgBackupReader::InstaMsgBackupReader(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.btn_loadbackup, SIGNAL(clicked()), this, SLOT(openBackupFile()));
	connect(ui.lw_friends, SIGNAL(itemClicked(QListWidgetItem*)),
		this, SLOT(onConversationSelect(QListWidgetItem*)));
}

void InstaMsgBackupReader::openBackupFile()
{
	msgFile = QFileDialog::getOpenFileName(this,
		tr("Open Message Backup File"), "", tr("Message Backup File (*.json)"));
	if (msgFile.isNull())
	{
		QMessageBox *msg = new QMessageBox(this);
		msg->setText("Please select a backup file!");
		msg->setWindowTitle("Error!");
		msg->setIcon(QMessageBox::Icon::Critical);
		msg->show();
	}
	else
	{
		printf("Loading backup file...!\n");
		
		ui.le_backupfilelocation->setText(msgFile);
		
		QFile dataFile(msgFile);
		if (!dataFile.open(QIODevice::ReadOnly))
		{
			printf("Failed to load backup file!\n");
			return;
		}
		
		QTextStream qts_datastream(&dataFile);
		QByteArray qba_data = qts_datastream.readAll().toLocal8Bit();
		
		dataFile.close();
		
		QJsonParseError error;
		messagesJson = QJsonDocument::fromJson(qba_data, &error);
		
		if (error.error != QJsonParseError::NoError) {
			printf("Error: %s\n", error.errorString().toStdString().c_str());
			return;
		}

		printf("Backup file loaded!\n");
						
		if (messagesJson.isArray())
		{
			printf("Backup loaded!\n");
			for (auto axChatSession : messagesJson.array())
			{
				auto ChatSession = axChatSession.toObject();
				auto Participants = ChatSession["participants"].toArray();
				QString ax;
				for (auto ParticiPant : Participants)
				{
					ax += ParticiPant.toString();
					if (ParticiPant != Participants.last())
						ax += " -> ";
				}
				ui.lw_friends->addItem(ax);
				Conversations.push_back(ChatSession["conversation"].toArray());
			}
		}
	}
}

void InstaMsgBackupReader::onConversationSelect(QListWidgetItem * item)
{
	ui.txt_messages->clear();
	auto ConversationMessages = Conversations.at(ui.lw_friends->currentRow());
	int senderA = 0, senderB = 0;
	QString senderAs = "", senderBs = "";
	for (auto i = ConversationMessages.size() - 1; i > -1; i--)
	{
		auto message = ConversationMessages.at(i);
		auto MessageObj = message.toObject();
		auto sender = MessageObj["sender"].toString();
		auto isMediaShare = !MessageObj["media_owner"].isNull();
		auto isText = !MessageObj["text"].isNull();
		auto isStoryShare = !MessageObj["story_share"].isNull();
		auto isMedia = !MessageObj["media"].isNull();
		auto createTime = MessageObj["created_at"].toString();
		auto isHeart = !MessageObj["heart"].isNull();
		
		if (senderAs.isEmpty())
			senderAs = sender;
		if (sender != senderAs && senderBs.isEmpty())
			senderBs = sender;

		if (sender == senderAs)
			senderA++;
		if (sender == senderBs)
			senderB++;

		auto datex = createTime.split('T')[0].split('-');
		QString date = datex[2] + "/" + datex[1] + "/" + datex[0];

		auto timex = createTime.split('T')[1].split('.');
		QString time = timex[0];

		QString msgLine = "[" + date + " - " + time + "] "+ sender + " : ";

		if (isHeart)
		{
			msgLine += MessageObj["heart"].toString();
		}
		if (isMediaShare)
		{
			msgLine += "[Media by : " + MessageObj["media_owner"].toString() + "] [Media Desc : " + MessageObj["media_share_caption"].toString() + "] [Url : " + MessageObj["media_share_url"].toString() + "]";
		}
		if(isText)
		{
			if (isStoryShare)
			{
				msgLine += "[Story by : " + MessageObj["story_share"].toString() + "] ";
			}
			msgLine += MessageObj["text"].toString();
		}
		ui.txt_messages->append(msgLine);
	}
	ui.label_3->setText("Messages (Total: " + QString::fromStdString(std::to_string(ConversationMessages.size())) + ") (By " + senderAs + ": " + QString::fromStdString(std::to_string(senderA)) + ") (By " + senderBs + ": " + QString::fromStdString(std::to_string(senderB)) + ")");
}
