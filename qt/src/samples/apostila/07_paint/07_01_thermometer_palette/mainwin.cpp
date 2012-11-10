#include "MainWin.h"

#include <QApplication>
#include <QWidget>
#include <QLabel>

#include <QSlider>
#include <QSpinBox>
# include <QGridLayout>
#include <QPalette>

MainWin::MainWin ( QWidget * pParent )
    : QWidget(pParent )
{
	// cria um slider, define o seu range e sua largura m�nima:
	m_slider = new QSlider(Qt::Horizontal, this);
	m_slider->setMinimumWidth (200);
	m_slider->setRange(0, 100);

	// Cria uma spin-box para exibir o valor do slider,
	// podendo tamb�m alterar o slider:
	QSpinBox * sliderSpin = new QSpinBox(this);
	sliderSpin->setRange(0,100);

	// cria o term�metro e define seu tamanho m�nimo:
	m_thermometer = new QWidget(this);
	m_thermometer->setMinimumSize(60,250);

	// cria dois labels que ocupar�o a �rea superior e inferior do "thermometer"
	m_whiteLabel = new QLabel(m_thermometer );
	m_redLabel = new QLabel(m_thermometer );

	m_thermometerMargin = 4;

	// define uma pallete para o "thermometer"
	QPalette pal= m_thermometer->palette();
	pal.setColor( QPalette::Window, QColor( Qt::yellow) );
	m_thermometer->setPalette(pal);
	m_thermometer->setAutoFillBackground(true); // preenchimento autom�tico, sem necessidade de "paintEvent"

	// define uma palette para o "whiteLabel"
	pal= m_whiteLabel->palette();
	pal.setColor( QPalette::Window, QColor( Qt::white) );
	m_whiteLabel->setPalette(pal);
	m_whiteLabel->setAutoFillBackground(true); // preenchimento autom�tico, sem necessidade de "paintEvent"

	// define uma palette para o "redLabel"
	pal= m_redLabel->palette();
	pal.setColor( QPalette::Window, QColor( Qt::red) );
	m_redLabel->setPalette(pal);
	m_redLabel->setAutoFillBackground(true); // preenchimento autom�tico, sem necessidade de "paintEvent"

	// cria um GridLayout, relacionando-o ao widget principal('this'):
	QGridLayout * mainGLayout = new QGridLayout(this);
	// o grid ter� 4 colunas, sendo que a segunda e a terceira
	// ocupar�o um espa�o maior:
	mainGLayout->setColumnStretch(0,1);
	mainGLayout->setColumnStretch(1,3);
	mainGLayout->setColumnStretch(2,3);
	mainGLayout->setColumnStretch(3,1);

	// o term�metro ocupa a segunda e a terceira colunas (inicia na coluna 1, com colspan=2):
	mainGLayout->addWidget( m_thermometer, 0, 1, 1, 2, Qt::AlignHCenter);
	// o slider ocupa as 3 primeiras colunas (inicia na coluna 0, com colspan=3):
	mainGLayout->addWidget(m_slider, 1, 0, 1, 3);
	// a spin-box ocupa apenas a quarta e �ltima coluna:
	mainGLayout->addWidget(sliderSpin, 1, 3);

	// Conecta o signal "valueChanged" do slider
	// ao slot "setValue" da spin-box para que exiba o valor do slider,
	// quando este for alterado:
	connect(m_slider, SIGNAL(valueChanged(int)),
                sliderSpin, SLOT(setValue(int)));

	// Conex�o inversa: o signal da spin-box e o slot do slider:
	connect(sliderSpin, SIGNAL(valueChanged(int)),
                        m_slider, SLOT(setValue(int)));

	// Conecta o signal "valueChanged" do slider
	// ao slot "updateThermometer" deste objeto,
	// de modo a for�ar um redimensionamento do "redLabel" e do "whiteLabel":
	connect(m_slider, SIGNAL(valueChanged(int)),
                 this, SLOT( updateThermometer(int)));


	m_slider->setValue(0);
	sliderSpin->setValue(0);
	show();
	updateThermometer (0);
}

void MainWin::updateThermometer ( int sliderValue )
{
	// �rea a usar para desenho das partes brancas e vermelha do "thermometer":
	const int left =m_thermometer->rect().left()+m_thermometerMargin; // borda
	const int width = m_thermometer->rect().width()-(m_thermometerMargin<<1); // largura, subtraindo borda-esquerda e borda-direita
	const int top = m_thermometer->rect().top()+m_thermometerMargin; //  coordenada "y", topo, adicionando borda
	const int height = m_thermometer->rect().height()-(m_thermometerMargin<<1); // altura, subtraindo borda-superior e borda-inferior

	// 1) a parte inferior do widget "thermometer"
	//      ser� preenchida em vermelho,
	//      ocupando uma �rea proporcional ao valor atual do slider

	// 2) a parte superior do "thermometer",
	//     ser� preenchida com branco
	//     e ocupar� a �rea restante


	// altura para a �rea vermelha, calculada proporcionalmente ao valor do slider:
	const int redHeight  = (sliderValue * height) / m_slider->maximum() ;
	// altura para a �rea branca
	const int whiteHeight= height-redHeight;

	// 1) ==== redimension o label da �rea branca:
	if ( whiteHeight > 0 )
	{
		m_whiteLabel->setGeometry(left, top, width, whiteHeight);
		if ( !m_whiteLabel->isVisible() )
			m_whiteLabel->show();
	}
	else
		m_whiteLabel->hide();

	// 2) ==== redimension o label da �rea vermelha:
	if ( redHeight > 0 )
	{
		const int redTop = top + whiteHeight ;  // preencher� toda a �rea inferior a partir de "redTop"
		m_redLabel->setGeometry(left, redTop, width, redHeight);
		if ( !m_redLabel->isVisible() )
			m_redLabel->show();
	}
	else
		m_redLabel->hide();
}

