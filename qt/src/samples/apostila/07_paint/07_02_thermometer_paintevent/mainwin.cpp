#include "MainWin.h"

#include <QPaintEvent>
#include <QPainter>

#include <QSlider>
#include <QSpinBox>
# include <QGridLayout>

MainWin::MainWin ( QWidget * pParent )
    : QWidget(pParent )
{
	// cria um slider, define o seu range e sua largura m�nima:
	m_slider = new QSlider(Qt::Horizontal, this);
	m_slider->setMinimumWidth (200);
	m_slider->setRange(0, 100);

	// Cria uma spin-box para exibir o valor do slider,
	// podendo tamb�m alterar o slider:
	QSpinBox * spin = new QSpinBox(this);
	spin->setRange(0,100);

	// cria o term�metro e define seu tamanho m�nimo:
	m_thermometer = new Thermometer(this);
	m_thermometer->setMinimumSize(60,250);

	// cria um gridLayout, relacionando-o ao widget principal('this'):
	QGridLayout * gridLayout = new QGridLayout(this);
	// o grid ter� 4 colunas, sendo que a segunda e a terceira
	// ocupar�o um espa�o maior:
	gridLayout->setColumnStretch(0,1);
	gridLayout->setColumnStretch(1,3);
	gridLayout->setColumnStretch(2,3);
	gridLayout->setColumnStretch(3,1);
	// o term�metro ocupa a segunda e a terceira colunas (inicia na coluna 1, com colspan=2):
	gridLayout->addWidget(m_thermometer, 0, 1, 1, 2, Qt::AlignHCenter);
	// o slider ocupa as 3 primeiras colunas (inicia na coluna 0, com colspan=3):
	gridLayout->addWidget(m_slider, 1, 0, 1, 3);
	// a spin-box ocupa apenas a quarta e �ltima coluna:
	gridLayout->addWidget(spin, 1, 3);

	// Conecta o signal "valueChanged" do slider
	// ao slot "setValue" da spin-box para que exiba o valor do slider,
	// quando este for alterado:
	connect(m_slider, SIGNAL(valueChanged(int)),
					 spin, SLOT(setValue(int)));

	// Conex�o inversa: o signal da spin-box e o slot do slider:
	connect(spin, SIGNAL(valueChanged(int)),
					m_slider, SLOT(setValue(int)));

	// Conecta o signal "valueChanged" do slider
	// ao slot "update" do term�metro,
	// de modo a for�ar uma nova pintura do term�metro:
	connect(m_slider, SIGNAL(valueChanged(int)),
                         m_thermometer, SLOT( update()));
                        // =>	update(); => for�a um  novo "paintEvent"

	m_slider->setValue(0);
}

// paintEvent: aqui podemos personalizar o desenho/pintura de um Widget
// usando um objeto QPainter relacionado a esse Widget:
void Thermometer::paintEvent(QPaintEvent * pEv)
{
	MainWin * mainWin = qobject_cast<MainWin*>( parent() );
	// "qobject_cast": semelhante ao standard "dynamic_cast", mas N�O usa RTTI.
	// A exig�ncia � que o tipo envolvido tenha declarado a macro Q_OBJECT.

	if ( !mainWin)
		return; // parent n�o � do tipo "MainWin*" !!!

	// Objeto QPainter que permitir� desenhar na �rea cliente do widget "Thermometer"
	QPainter paint(this);

	// desenha um fundo para �rea total do "thermometer":
	//paint.fillRect( this->rect(), QBrush( Qt::yellow ));
	// ou, neste exemplo:
	paint.fillRect( pEv->rect() ,QBrush( Qt::yellow ));

	// �rea restante para desenho das partes brancas e vermelha do "thermometer":
	const int border =4; // borda
	const int left = this->rect().left()+border;   //  coordenada "x", esquerda, adicionando borda
	const int width = this->width()-(border<<1); // largura, subtraindo borda-esquerda e borda-direita
	const int top = this->rect().top()+border; //  coordenada "y", topo, adicionando borda
	const int height = this->height()-(border<<1); // altura, subtraindo borda-superior e borda-inferior

	// 1) a parte inferior do label "thermometer"
	//      ser� preenchida em vermelho,
	//      ocupando uma �rea proporcional ao valor atual do slider

	// 2) a parte superior do "thermometer",
	//     ser� preenchida com branco
	//     e ocupar� a �rea restante

	// Desativar a caneta para que n�o seja usada ao desenhar ret�ngulos abaixo.
	paint.setPen(Qt::NoPen);
	// Caso fosse usada uma caneta, as �reas inferior e superior teriam, al�m de bordas laterais,
	// tambem uma borda acima da �rea vermelha e abaixo da �rea branca,
	// fazendo uma divis�o entre as duas, o que n�o � desejado aqui.
	// Por isso, acima, desenhamos um fundo para a �rea total do "thermometer"
	// e estabelecemos bordas que ser�o obedecidas no desenho das 2 �reas internas.


	// 1) ==== �rea INFERIOR (vermelha, proporcional ao valor do slider):

	// 1.a) valor atual do slider:
	const int redValue = mainWin->slider()->value();
	// 1.b) altura para esta �rea  calculada proporcionalmente ao valor do slider:
	const int redHeight  = (redValue * height) / mainWin->slider()->maximum() ;
	// 1.c) coordenada "y"(topo) para a cor vermelha,
	const int redTop = top + (height-redHeight) ;  // preencher� toda a �rea inferior a partir de "redTop"

	// 1.d) desenha a parte inferior (vermelha):
	if ( redHeight > 0  )
	{
		paint.setBrush(QBrush(Qt::red)); // "pincel" vermelho para o fundo
		paint.drawRect( left, redTop, width, redHeight );
	}

	// 2) ==== �rea SUPERIOR (branca, ocupando a �rea restante):

	// 2.a) altura para esta �rea:
	const int whiteHeight= height-redHeight;
	// 2.b) coordenada "y"(topo) para cor branca:
	const int & whiteTop = top;

	// 2.c) desenha a parte superior (branca)
	if ( whiteHeight > 0  )
	{
		paint.setBrush(QBrush(Qt::white)); // "pincel" branco para o fundo
		paint.drawRect( left, whiteTop, width, whiteHeight );
	}
}
