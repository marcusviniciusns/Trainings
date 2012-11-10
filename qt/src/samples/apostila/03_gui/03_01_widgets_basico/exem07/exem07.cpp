#include <QtGui>

int main(int argc , char ** argv)
{
	QApplication app(argc, argv); // objeto que inicia a aplicacao

	// cria um "Widget" (abreviatura de "Windows Gadget) que e a base de todas as janelas;
	// essa ser� a janela principal:

	QWidget * wgMain = new QWidget(0);

	// Cria dois "Widgets" (no caso dois bot�es);
	// - observar que os bot�es s�o criados aqui sem uma janela "parent":
	QPushButton * btAboutQt	= new QPushButton("About Qt", 0);
	QPushButton * btQuit	= new QPushButton("Quit", 0);

	// Cria mais dois "Widgets" com textos explicativos sobre os bot�es;
	// no caso, dois objetos da classe "QLabel":
	// (o '\n' no meio dos textos permitir� a quebra dos mesmos em 2 linhas):
	QLabel * lbAboutQt = new QLabel("O bot�o ao lado exibir� um di�logo\ncom informa��es sobre Qt.", 0);
	QLabel * lbQuit    = new QLabel("O bot�o ao lado\nencerrar� esta aplica��o.", 0);
	
	// == Conectando eventos a uma acao:

	// o primeiro bot�o estara conectado ao "slot" "aboutQt" 
	// do objeto de QApplication:
	QObject::connect( btAboutQt, SIGNAL( clicked() ) , 
					  &app	   , SLOT  ( aboutQt() )  );

	// o segundo, ao "slot" "quit":
	QObject::connect( btQuit , SIGNAL( clicked() ) , 
					  &app	 , SLOT  ( quit() )  );

	// cria um objeto de "GridLayout" para organizar os Widgets:
	QGridLayout * glayMain = new QGridLayout;
	glayMain->addWidget( btAboutQt, 0,0 ); // linha, coluna
	glayMain->addWidget( lbAboutQt, 0,1 ); // idem

	glayMain->addWidget( btQuit, 1,0 ); // linha, coluna
	glayMain->addWidget( lbQuit, 1,1 ); // idem
	
	glayMain->setColumnStretch(0,1);
	glayMain->setColumnStretch(1,3);

	// cria 5 Widgets com a data de hoje e que ser�o acrescentados ao final da janela
	QDateTimeEdit *pDateEdit1 = new QDateTimeEdit(QDate::currentDate());
	QDateTimeEdit *pDateEdit2 = new QDateTimeEdit(QDate::currentDate());
	QDateTimeEdit *pDateEdit3 = new QDateTimeEdit(QDate::currentDate());
	QDateTimeEdit *pDateEdit4 = new QDateTimeEdit(QDate::currentDate());
	QDateTimeEdit *pDateEdit5 = new QDateTimeEdit(QDate::currentDate());
	// a data de hoje ser� apenas exibida mas n�o poder� ser alterada:
	pDateEdit1->setReadOnly( true ); 
	pDateEdit2->setReadOnly( true ); 
	pDateEdit3->setReadOnly( true ); 
	pDateEdit4->setReadOnly( true ); 

	// abaixo, vamos usar o argumento "columnSpan" 
	// para que um Widget ocupe a linha toda:
	glayMain->addWidget( pDateEdit1, 2,0 ,1, 2); // linha, coluna, rowSpan, columnSpan

	// al�m disso, ele pode ser posicionado atrav�s do alinhamento:
	glayMain->addWidget( pDateEdit2, 3,0 ,1, 2, Qt::AlignHCenter ); // linha, coluna, rowSpan, columnSpan, alignment
	glayMain->addWidget( pDateEdit3, 4,0 ,1, 2, Qt::AlignRight );   // idem
	glayMain->addWidget( pDateEdit4, 5,0 ,1, 2, Qt::AlignLeft ); // idem

	// podemos obter o mesmo efeito do �ltimo exemplo("AlignLeft") 
	// simplesmente inserindo o "DateEdit" na coluna zero da sua linha:
	glayMain->addWidget( pDateEdit5, 6,0 ); // linha, coluna
	
	// associa o GridLayout � janela principal:
	wgMain->setLayout ( glayMain );

	// exibe a janela:
	wgMain->show();

	// entra no "event loop"  (laco de mensagens)
	app.exec();

	return 0;
}
