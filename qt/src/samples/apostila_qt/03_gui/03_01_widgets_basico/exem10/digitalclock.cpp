#include "DigitalClock.h"
#include <QTime>
#include <QTimer>

DigitalClock::DigitalClock(QWidget *parent)
	: QLCDNumber(parent)
{
	// define estilo e cor do texto do LCD:
	setSegmentStyle(Filled );
	setForegroundRole ( QPalette::Midlight);
	// quantidade de d�gitos a exibir (hh:mm:ss):
	setNumDigits(8);

	// cria um objeto QTimer (que ir� emitir o signal "timeout" 
	// no intervalo de tempo a ser especificado):
	QTimer *timer = new QTimer(this);

	// especifica o intervalo de tempo (1 segundo)
	// para emiss�o do sinal "timeout":
	timer->start(1000);
	
	// conecta o signal "timeout" do objeto timer 
	// (ser� emitido sempre que o intervalo de tempo for atingido)
	// ao slot "showTime" desta classe:
	connect(timer, SIGNAL(timeout()), 
			 this, SLOT(showTime()) );

	// define o t�tulo da janela:
	setWindowTitle("Digital Clock");
	
	// for�a a defini��o da largura e altura da janela:
	resize(300, 120);

	// for�a a exibi��o inicial da hora no LCD:
	showTime();
}

void DigitalClock::showTime()
{
	// pega a hora atual da m�quina:
	QTime time = QTime::currentTime();

	// converte o objeto QTime para string, 
	// com o formato especificado no argumento:
	QString text = time.toString("hh:mm:ss");
	
	display(text); // exibe o texto contendo a hora atual no QLCDNumber
}

