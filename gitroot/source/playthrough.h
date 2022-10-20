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

class CPlaythrough : public QWidget
{

	public:

		CPlaythrough();

		virtual ~CPlaythrough();

		void init(QString audiosrcname = QString(), QString audiosinkname = QString());

		void reset();

	protected:

	private:


		/*!\brief output to audio device */
		class CAudiosink : public QAudioSink // output to audio device
		{
			public:
				CAudiosink(): QAudioSink() {};

		};

		QAudioSource*	m_audiosource;

		QAudioSink*		m_audiosink;

		QIODevice*		m_iosource;

};

#endif //_PLAYTHROUGH_H

