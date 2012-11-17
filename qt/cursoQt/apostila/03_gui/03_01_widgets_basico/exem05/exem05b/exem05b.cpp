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


	// cria um objeto de "layout" para organizar os dois bot�es VERTICALMENTE:
	QVBoxLayout * vlayButtons = new QVBoxLayout;
	// cria outro objeto de "layout" para organizar os dois labels VERTICALMENTE:
	QVBoxLayout * vlayLabels = new QVBoxLayout;

	// adicionar objetos aos 2 layouts:

	// bot�es:
	vlayButtons->addWidget( btAboutQt); 
	vlayButtons->addWidget( btQuit   ); 
	// labels:
	vlayLabels->addWidget ( lbAboutQt );
	vlayLabels->addWidget ( lbQuit  );  
	
	// finalmente, cria um objeto de "layout" que ir� organizar os dois "layouts" anteriores HORIZONTALMENTE:
	QHBoxLayout * hlayMain = new QHBoxLayout;
	hlayMain->addLayout( vlayButtons  );
	hlayMain->addLayout( vlayLabels );


	// associa o layout horizontal � janela principal:
	wgMain->setLayout ( hlayMain );

	// exibe a janela:
	wgMain->show();

	// entra no "event loop"  (laco de mensagens)
	app.exec();

	return 0;
}
