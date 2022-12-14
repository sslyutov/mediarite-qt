/*!\brief playthrough audion widget
* \file playthrough.h
* \author Sergey Slyutov
* \date October 20 ,2022
*/

#ifndef _PLAYTHROUGH_H

#define _PLAYTHROUGH_H

#include <QWidget>

#include <QAudioSource>

#include <QAudioSink>

#include <QIODevice>

#include <QBuffer>

#include <QSettings>

class CPlaythrough : public QWidget
{

	public:

		CPlaythrough();

		virtual ~CPlaythrough();

		void init(QString audiosrcname = QString(), QString audiosinkname = QString());

		void reset();

		void volume(int vol) { 
			if(m_audiosink)
				m_audiosink->setVolume((float)vol / 100.0);
			
		};
		int volume(void) { 
			if (m_audiosink) {
				return (int)(m_audiosink->volume() * 100.0);
			}
			return 0;
		};
		

	protected:

	private:

		QAudioSource*	m_audiosource;

		QAudioSink*		m_audiosink;

		QIODevice*		m_iosource;

		QBuffer			m_buffer;

		const char* playthrough_volume_flag = "playthrough_volume";

};

#endif //_PLAYTHROUGH_H

