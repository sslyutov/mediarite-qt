/*!\brief timer settings widget provide ui for setup of a timer
* \file timersettingswidget.h
* \date November 02, 2022
* \author Sergey Slyutov
*/

#ifndef _TIMERSSETTINGWIDEGT_H

#define _TIMERSSETTINGWIDEGT_H

#include <QDialog>

#include <QList>

#include "ui_timersettingswidget.h"

class CTimerSettingsWidget: 

	public QDialog, 

	public Ui::TimerSettingsForm
{
	Q_OBJECT

public:

	CTimerSettingsWidget(int timervalue);

	virtual ~CTimerSettingsWidget();

	int timervalue(void) { return m_timervalue; };

private:

	int m_timervalue;

};

#endif // _TIMERSSETTINGWIDEGT_H
