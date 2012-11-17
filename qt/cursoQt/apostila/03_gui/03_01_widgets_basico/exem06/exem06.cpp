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


	// cria um objeto de "layout" para organizar o primeiro bot�o e o primeiro label HORIZONTALMENTE:
	QHBoxLayout * hlayAboutQt = new QHBoxLayout;
	// adiciona os objetos a esse layout:
	hlayAboutQt->addWidget( btAboutQt,1 ); // o bot�o ocupa menos espa�o horizontal...
	hlayAboutQt->addWidget( lbAboutQt,3 ); // o label o triplo da largura do bot�o

	// cria outro objeto de "layout" para organizar o segundo bot�o e o segundo label HORIZONTALMENTE:
	QHBoxLayout * hlayQuit = new QHBoxLayout;
	// adiciona os objetos a esse layout:
	hlayQuit->addWidget( btQuit, 1 ); // o bot�o ocupa menos espa�o horizontal...
	hlayQuit->addWidget( lbQuit, 3);  // o label o triplo da largura do bot�o
	 
	// cria um objeto de "layout" que ir� organizar os dois layouts anteriores VERTICALMENTE:
	QVBoxLayout * vlayMain = new QVBoxLayout;
	vlayMain->addLayout( hlayAboutQt  );
	vlayMain->addLayout( hlayQuit);

	// cria 4 Widgets com a data de hoje e que ser�o acrescentados ao final da janela
	QDateTimeEdit *pDateEdit1 = new QDateTimeEdit(QDate::currentDate());
	QDateTimeEdit *pDateEdit2 = new QDateTimeEdit(QDate::currentDate());
	QDateTimeEdit *pDateEdit3 = new QDateTimeEdit(QDate::currentDate());
	QDateTimeEdit *pDateEdit4 = new QDateTimeEdit(QDate::currentDate());
	// a data de hoje ser� apenas exibida mas n�o poder� ser alterada:
	pDateEdit1->setReadOnly( true ); 
	pDateEdit2->setReadOnly( true ); 
	pDateEdit3->setReadOnly( true ); 
	pDateEdit4->setReadOnly( true ); 

	// podemos adicionar o Widget diretamente no layout principal:
	vlayMain->addWidget( pDateEdit1); // adiciona widget...
	
	// mas se queremos manipular o posicionamento do Widget
	// ent�o a melhor maneira � criar um novo layOut Horizontal
	// que conter� o widget; 

	// centralizando o DateEdit:
	QHBoxLayout * hlayDate2 = new QHBoxLayout;
	hlayDate2->addStretch( );         // espa�o proporcional
	hlayDate2->addWidget( pDateEdit2);
	hlayDate2->addStretch( ); // espa�o proporcional

	// alinhando � direita:
	QHBoxLayout * hlayDate3 = new QHBoxLayout;
	hlayDate3->addStretch( );         // espa�o proporcional
	hlayDate3->addWidget( pDateEdit3);
	
	// alinhando � esquerda:
	QHBoxLayout * hlayDate4 = new QHBoxLayout;
	hlayDate4->addWidget( pDateEdit4);
	hlayDate4->addStretch( );         // espa�o proporcional

	// finalmente esses novos layouts Horizontais s�o adicionados ao layout principal
	vlayMain->addLayout( hlayDate2 );
	vlayMain->addLayout( hlayDate3 );
	vlayMain->addLayout( hlayDate4 );
	
	// associa o layout vertical � janela principal:
	wgMain->setLayout ( vlayMain );

	// exibe a janela:
	wgMain->show();

	// entra no "event loop"  (laco de mensagens)
	app.exec();

	return 0;
}


