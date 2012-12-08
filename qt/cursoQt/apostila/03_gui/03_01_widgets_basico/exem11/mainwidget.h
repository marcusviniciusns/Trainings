#ifndef _MAIN_WIDGET_H_INC_
#define _MAIN_WIDGET_H_INC_

#include "DigitalClock.h"

class MainWidget : public QWidget
{
	Q_OBJECT

	private:
		// a vari�vel abaixo, "_m_pBottomLabel", 
		// � um membro da classe
		// pois ser� usada em mais que uma fun��o desta classe:
		QLabel * _m_pBottomLabel;

	public:
		// construtora:
		MainWidget(QWidget * pParent = 0 );

	private slots:
		// slot setTime: ser� acionado pelo signal "timeChanged" de "DigitalClock",
		// recebendo como argumento um texto contendo a hora atual; 
		// ir� tratar esse texto para imprimi-lo em "_m_pBottomLabel":

		void setTime( const QString & qstTime );

};

#endif // ( _MAIN_WIDGET_H_INC_ )


