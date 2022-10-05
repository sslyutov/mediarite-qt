/*!\brief audio test application for Qt6
* \file main.cpp
* \author Sergey Slyutov
* \date September 01, 2022
*/

#include "videorecorder.h"
#include <QApplication>

int main(int argc, char * argv[])
{
	QApplication app(argc, argv);

	QApplication::setOrganizationName("Inetgrad");
	QApplication::setApplicationName("IG Media Capture");
	QApplication::setApplicationVersion("1.0.1.0");

	CVideoRecorderForm videorecorder;
	videorecorder.setWindowTitle("Media-rite");
	videorecorder.show();

	app.exec();

	return 0;
};