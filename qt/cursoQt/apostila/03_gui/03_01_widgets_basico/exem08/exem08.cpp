
#include <QtGui>

int main(int argc , char * argv[])
{
	QApplication app(argc, argv); // objeto para iniciar a aplica��o
	
	// cria um objeto "slider" e um "label"
	QSlider * pSlider = new QSlider(Qt::Horizontal); 
	QLabel * pLabel = new QLabel;

	// cria um layout:
	QHBoxLayout * layMain = new QHBoxLayout;

	// adciona os widgets ao layout:
	layMain->addWidget( pSlider );
	layMain->addWidget( pLabel );

	// acrescenta bordas ao label;
	pLabel->setFrameShape ( QFrame::Box );
	pLabel->setFrameShadow( QFrame::Raised );

	// conecta o sinal "valueChanged" do "slider"
	// ao "slot" "SetNum" do "label":
	QObject::connect(pSlider, SIGNAL( valueChanged(int) ),
					 pLabel , SLOT  ( setNum(int) ) );

	// define o range e o valor inicial do slider:
	pSlider->setRange(0, 100); // range
	pSlider->setValue( 20 ); // valor inicial

		// acima, a fun��o "setValue" j� ir� modificar o valor do "slider";
		// nesse momento, o "slider" emitir� o sinal "valueChanged" que j� ir� modificar o conte�do do label;
		// e sempre que o "slider" for movimentado, o sinal ser� emitido e o label ser� modificado.

	// janela principal:
	QWidget * wgMainWindow = new QWidget;

	// adiciona o layou � janela principal:
	wgMainWindow->setLayout ( layMain );

	// exibe a janela
	wgMainWindow->show();

	// entra no eventloop (la�o de mensagens):
	app.exec();

	return 0;
}

