
#include <QtGui>

int main(int argc , char * argv[])
{
	QApplication app(argc, argv); // objeto para iniciar a aplica��o
	
	// cria um objeto "slider", um spinbox e um "label"
	QSlider * pSlider = new QSlider(Qt::Horizontal); 
	QLabel * pLabel = new QLabel; 
	QSpinBox *pSpinBox = new QSpinBox;

	// cria um primeiro layout(horizontal) para agrupar o "spinbox" e o "label":
	QHBoxLayout * hlayBottom = new QHBoxLayout; // "H" ->horizontal

	// cria um segundo layout(vertical) para agrupar o primeiro layout e o "slider":
	QVBoxLayout * vlayMain = new QVBoxLayout; // "V"->vertical
	
	// adciona o spin e o label ao layout-horizontal
	// usando "stretch" antes e depois para que ocupem
	// apenas a largura necess�ria e fiquem centralizados:
	hlayBottom->addStretch();
	hlayBottom->addWidget( pSpinBox );
	hlayBottom->addWidget( pLabel );
	hlayBottom->addStretch();

	// adiciona o slider e o layout-horizontal ao layout-vertical:
	vlayMain->addWidget( pSlider );
	vlayMain->addLayout( hlayBottom );
	vlayMain->addStretch();

	// acrescenta bordas ao label;
	pLabel->setFrameShape ( QFrame::Box );
	pLabel->setFrameShadow( QFrame::Raised );

	// conecta o sinal "valueChanged" do "slider"
	// ao "slot" "SetNum" do "label":
	// e ao "slot" "setValue" do "spinbox":

	QObject::connect(pSlider, SIGNAL( valueChanged(int) ) ,
					 pLabel , SLOT  ( setNum(int) )  );

	QObject::connect(pSlider , SIGNAL( valueChanged(int) ) ,
					 pSpinBox, SLOT  ( setValue(int) )  );


	// conecta o sinal "valueChanged" do "spinbox"
	// ao "slot" "setValue" do "slider",
	// estabelecendo uma rela��o rec�proca entre o "slider" e o "spinbox":

	QObject::connect(pSpinBox , SIGNAL( valueChanged(int) ) ,
					 pSlider  , SLOT  ( setValue(int) )  );


	// define o range do spinbox:
	pSpinBox->setRange(0, 100); // range

	// define o range e o valor inicial do slider:
	pSlider->setRange(0, 100); // range
	pSlider->setValue( 50 ); // valor inicial

		// acima, a fun��o "setValue" j� ir� modificar o valor do "slider";
		// nesse momento, o "slider" emitir� o sinal "valueChanged" 
		// que j� ir� modificar o conte�do do label e tamb�m do spinbox;
		

	// janela principal:
	QWidget * wgMainWindow = new QWidget;

	// adiciona o layout vertical � janela principal:
	wgMainWindow->setLayout ( vlayMain );

	// exibe a janela
	wgMainWindow->show();

	// entra no eventloop (la�o de mensagens):
	app.exec();

	return 0;
}

