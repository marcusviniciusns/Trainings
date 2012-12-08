/*
Esta classe trata a janela principal da aplica��o.
A janela ter� um ""DigitalClock" � esquerda e dois labels � direita:
- O primeiro label dever� imprimir o texto(a hora) que � passado por "DigitalClock" sempre que este emite o signal "timeChanged";
- O segundo label tamb�m ir� imprimir esse mesmo texto, mas de modo diferenciado, com atributos de fonte e cor, usando HTML.
*/

#include "MainWidget.h"

MainWidget::MainWidget(QWidget * pParent )
	: QWidget( pParent )
{	
	DigitalClock * pClock = new DigitalClock;
	QLabel * pTopLabel = new QLabel;

	// a vari�vel abaixo, "_m_pBottomLabel", � um membro da classe
	// pois ser� usada em outra fun��o e n�o apenas na construtora:
	_m_pBottomLabel = new QLabel;

	// acrescenta bordas aos labels;
	pTopLabel->setFrameShape ( QFrame::Box );
	pTopLabel->setFrameShadow( QFrame::Raised ); // Raised
	_m_pBottomLabel->setFrameShape ( QFrame::Box );
	_m_pBottomLabel->setFrameShadow( QFrame::Sunken );// Sunken

	// layout para agrupar, verticalmente, os labels � direita da janela:
	QVBoxLayout * vlay = new QVBoxLayout;
	vlay->addWidget(pTopLabel);
	vlay->addWidget(_m_pBottomLabel);
	
	// layout para agrupar, horizontalmente, 
	// o widget "DigitalClock" � esquerda e o layout vertical � direita:
	QHBoxLayout * hlay = new QHBoxLayout;
	hlay->addWidget(pClock,2); // se poss�vel, o "DigitalClock" dever� ocupar o dobro da largura 
	hlay->addLayout(vlay,1);   // com rela��o ao layout vertical que ficar� � direita

	// Conectar signals a slots:

	// 1- o signal "timeChanged" emitido pelo "DigitalClock"
	//		pode ser conectado diretamente ao slot "setText" de "pTopLabel",
	//		pois o texto passado como argumento ser� impresso 
	//		do mesmo modo que � enviado pelo signal, sem altera��es:
	connect( pClock,   SIGNAL (timeChanged( const QString &)) ,
			 pTopLabel,  SLOT ( setText (const QString &)) );

	// 2- contudo esse mesmo sinal n�o pode ser conectado diretamente ao slot "setText" de "_m_pBottomLabel",
	//		pois queremos imprimir o texto de modo diferenciado;
	//		ent�o esta classe ("MainWidget") precisa ter um slot pr�prio ("setTime", no caso)
	//		que fa�a o tratamento do texto antes de pass�-lo para "_m_pBottomLabel": 
	connect( pClock, SIGNAL (timeChanged( const QString &)) ,
			 this,     SLOT ( setTime (const QString &)) );

	// associa o layout a este objeto  "MainWidget":
	setLayout( hlay );

	// define o t�tulo desta janela:
	setWindowTitle("Digital Clock / Label Clock");

	// for�a a defini��o da largura e altura da janela:
	resize(400, 120);

	// for�a a exibi��o inicial da hora no LCD:
	// (consequentemente ocorrer� tamb�m a atualiza��o dos labels
	//  por for�a do mecanismo signal/slot):
	pClock->showTime();
}

// slot setTime: ser� acionado pelo signal "timeChanged" de "DigitalClock",
// recebendo como argumento um texto contendo a hora atual; 
// ir� tratar esse texto para imprimi-lo em "_m_pBottomLabel":
void MainWidget::setTime( const QString & qstTime )
{
	// pega os segundos da hora atual:
	int nSecs = qstTime.right(2).toInt();
	
	QString temp;

	// monta o texto para o "_m_pBottomLabel" usando tags HTML,
	// sendo que a cada segundo troca a fonte e a cor do texto:
	if ( nSecs & 1 )
		temp = "<font face='Verdana, Arial, Helvetica, sans-serif' size='5' color='#ff0000'>";
	else
		temp = "<font face='Times New Roman, Times, serif' size='6' color='#0000ff'>";
	
	// completa as tags:
	temp += "<b>";
	temp += qstTime;
	temp += "</b></font>";

	// passa o texto para "_m_pBottomLabel":
	_m_pBottomLabel->setText( temp);
}


