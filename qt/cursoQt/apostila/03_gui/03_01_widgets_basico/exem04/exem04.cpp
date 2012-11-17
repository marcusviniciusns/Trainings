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

	// == Conectando eventos a uma acao:

	// o primeiro bot�o estara conectado ao "slot" "aboutQt" 
	// do objeto de QApplication:
	QObject::connect( btAboutQt, SIGNAL( clicked() ) , 
					  &app	   , SLOT  ( aboutQt() )  );

	// o segundo, ao "slot" "quit":
	QObject::connect( btQuit , SIGNAL( clicked() ) , 
					  &app	 , SLOT  ( quit() )  );


	// cria um objeto de "layout" para organizar os bot�es dentro da janela:
	QVBoxLayout * layMain = new QVBoxLayout;
	// adiciona os bot�es ao layout:
	layMain->addWidget( btAboutQt );
	layMain->addWidget( btQuit );

	// associa o layout � janela principal:
	wgMain->setLayout ( layMain );

	// exibe a janela:
	wgMain->show();

	// entra no "event loop"  (laco de mensagens)
	app.exec();

	return 0;
}
