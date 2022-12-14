/*!\brief timer settings user interface
* \file timersettingswidget.cpp
* \date November 02, 2022
* \author Sergey Slyutov
*/

#include "timersettingswidget.h"

#include <QIntValidator>

CTimerSettingsWidget::CTimerSettingsWidget(int timervalue)
	: QDialog()
	, Ui::TimerSettingsForm()
	, m_timervalue(timervalue)
{
	setupUi(this);

	connect(pushCancel, &QPushButton::released, [=]() {

		rejected();

		close();

	});
	
	connect(pushOk, &QPushButton::released, [=]() {

		m_timervalue = (sbHours->text().toInt() * 3600) + (sbMinutes->text().toInt() * 60) + sbSeconds->text().toInt();

		accept();

		close();

	});
	
	int hours = m_timervalue / 3600;
	sbHours->setValue(hours);

	int minutes = (m_timervalue - (hours * 3600)) / 60;
	sbMinutes->setValue(minutes);

	int seconds = m_timervalue - (hours * 3600) - (minutes * 60);
	sbSeconds->setValue(seconds);
};

CTimerSettingsWidget::~CTimerSettingsWidget()
{};

