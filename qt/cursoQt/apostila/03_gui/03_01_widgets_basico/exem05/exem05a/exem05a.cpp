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
	// cria outro objeto de "layout" para organizar o segundo bot�o e o segundo label HORIZONTALMENTE:
	QHBoxLayout * hlayQuit = new QHBoxLayout;

	// adicionar objetos aos 2 layouts:
	
/*	// Primeira alternativa (simplesmente adicionamos os Widgets, sem cuidar de posicionamento):
	hlayAboutQt->addWidget( btAboutQt);
	hlayAboutQt->addWidget( lbAboutQt );
	hlayQuit->addWidget( btQuit );
	hlayQuit->addWidget( lbQuit);
	// e o resultado � que a posi��o e dimens�o dos objetos est� fora de controle
*/

/*	// Segunda alternativa (definimos o alinhamento):
	hlayAboutQt->addWidget( btAboutQt, Qt::AlignLeft  );
	hlayAboutQt->addWidget( lbAboutQt,  Qt::AlignRight );
	hlayQuit->addWidget( btQuit, Qt::AlignLeft );
	hlayQuit->addWidget( lbQuit,  Qt::AlignRight );
	// o aspecto � melhor, mais ainda fora de controle 
*/

	// Terceira alternativa (definimos um fator de "stretch" para divis�o proporcional dos espa�os):
	hlayAboutQt->addWidget( btAboutQt,  1  ); // o bot�o ocupa menos espa�o horizontal...
	hlayAboutQt->addWidget( lbAboutQt,  3 ); // o label o triplo da largura do bot�o

	hlayQuit->addWidget   ( btQuit   ,  1 );  // o bot�o ocupa menos espa�o horizontal...
	hlayQuit->addWidget   ( lbQuit   ,  3 );  // o label o triplo da largura do bot�o
	
	// e esta �ltima � a melhor dessas 3 alternativas para obter o efeito desejado neste exemplo

	// finalmente, cria um objeto de "layout" que ir� organizar os dois layouts anteriores VERTICALMENTE:
	QVBoxLayout * vlayMain = new QVBoxLayout;
	vlayMain->addLayout( hlayAboutQt  );
	vlayMain->addLayout( hlayQuit );


	// associa o layout vertical � janela principal:
	wgMain->setLayout ( vlayMain );

	// exibe a janela:
	wgMain->show();

	// entra no "event loop"  (laco de mensagens)
	app.exec();

	return 0;
}
