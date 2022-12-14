/*!\brief audio test application for Qt6
* \file main.cpp
* \author Sergey Slyutov
* \date September 01, 2022
*/

#include "videorecorder.h"

#include <QApplication>

#include <QIcon>

#include <QSharedMemory>

#include <QMessageBox>

int main(int argc, char * argv[])
{
	QApplication app(argc, argv);

	// prevent multiple running instance of the application
	QSharedMemory singleapp("d842f963-e5b8-4197-9b35-c02d8848f45d");
	if (!singleapp.create(QString("d842f963-e5b8-4197-9b35-c02d8848f45d").length())) {
		singleapp.errorString();
		QMessageBox::information(NULL, qAppName(),
			QString("The %1 application cannot be started due to %2").arg(qAppName()).arg("another instance of the application is alraedy running."), QMessageBox::Ok);
		return 0;
	}

	QApplication::setOrganizationName("Inetgrad");

	QApplication::setApplicationName("IG Media Capture");
	
	QApplication::setApplicationVersion("1.0.1.0");

	QApplication::setWindowIcon(QIcon(":/resources/sss_b2.ico"));

	CVideoRecorderForm videorecorder;
	
	videorecorder.setWindowTitle(qAppName());
	
	videorecorder.show();

	app.exec();

	return 0;
};