#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QBitmap>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QPrinter>
#include <QDebug>

// Neste exemplo temos 2 "players" (2 imagens)
// e uma bola que se movimenta horizontalmente entre eles.

// Ao inv�s de usar o paintEvent diretamente,
// ele ser� usado indiretamente
// atrav�s das classes QGraphicsView, QGraphicsScene e QGraphicsItem

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWidgetClass)
	, m_started(false)
{
    ui->setupUi(this);

	// dimens�es do widget principal
	const int w = 500;
	const int h =  int(w*0.75);
	const int margin = int(w*0.025);
	resize(w,h);
	setMinimumSize( w, h );

	// cria a Graphics View:
	m_gView = new GView(this, m_started);

	// layout para o widget principal para que a view
	// acompanhe as suas dimens�es:
	QVBoxLayout * lay = new QVBoxLayout(this);
	lay->setMargin(0);
	lay->setSpacing(0);
	lay->addWidget(m_gView); // adiciona a view

	// cria a "scene" e a relaciona � view:
	// a "scene" � o objeto respons�vel pela cole��o de itens a serem desenhados
	m_gScene = new QGraphicsScene(0, 0, w, h);
	m_gScene->setBackgroundBrush(Qt::black);
	m_gView->setScene(m_gScene);

	adjustSize();
	const QPointF center = m_gScene->sceneRect().center();

	// ret�ngulo que ir� servir de fundo da �rea escal�vel da scene
	// usando bordas(caneta) al�m da cor de fundo(pincel):
	QBrush background = QBrush ( QColor(48,48,0) );
	QColor rectBorder = Qt::darkGray;
	QGraphicsRectItem * backgroundRect =
				m_gScene->addRect(0,0,w,h,
                                        QPen(rectBorder,2), background);
	// enviar o backGroundRect de fato para o fundo
	// (do contr�rio poderia encobrir os outros itens):
	backgroundRect->setZValue(-1); // 'ZValue' define se um item est� a frente ou se pode ser encoberto
							// o default � zero; assim, o valor -1 faz com que ele seja desenhado primeiro
							// e os demais ser�o desenhados sobre ele

	// texto:
	m_text = new Text;
	m_text->setMainAttributes(w, h, background.color(), rectBorder);
	m_text->adjustRect();
	m_gScene->addItem( m_text );

	// cria o primeiro player defindo seu pixmap e posicionamento:
	m_player1 = new Player;
	m_player1->setPixmap( QPixmap(":/images/next.png"));
	m_player1->setPos( QPointF(margin ,
								center.y() - (m_player1->pixmap().height()>>1) ));
	m_gScene->addItem(m_player1);

	// cria o segundo player defindo seu pixmap e posicionamento:
	m_player2 = new Player;
	m_player2->setPixmap(QPixmap(":/images/back.png"));
	m_player2->pixmap().setMask(m_player2->pixmap().createHeuristicMask());
	m_player2->setPos(QPointF(w-margin-m_player2->pixmap().width() ,
							   center.y() - (m_player2->pixmap().height()>>1) ));
	m_gScene->addItem(m_player2);

	// cria a bola, definindo cor de fundo, caneta e dimens�es
	// e a adiciona � scene:
	const int diameter = int ( w*0.025 )+4;
	m_ballXMove  = diameter-1;
	m_ball = new Ball;
	m_ball->setBrush( background.color().lighter(500)); // fundo
	m_ball->setPen(Qt::NoPen); // caneta
	m_ball->setRect(0,0, diameter, diameter); // dimens�es
	m_gScene->addItem(m_ball); // adiciona
	moveBallToInitialPosition(); // posicionamento inicial

	// signals da view para slots de MainWidget:
	connect( m_gView, SIGNAL (stateChanged()) ,
			 this , SLOT(changeState()));

	connect( m_gView, SIGNAL (moveBall(qreal,QEvent*)) ,
			 this , SLOT(moveBall(qreal,QEvent*)));

	// coloca o "jogo" em posi��o inativa:
	stopGame();

	// exibe:
	show();
}

MainWidget::~MainWidget()
{
    delete ui;
}

// "startGame": posiciona "m_started" como "true"
// habilitando o tratamento dos eventos de teclado e mouse para movimento da bola,
// muda t�tulo da janela e move bola para posi��o inicial.
void MainWidget::startGame()
{
	m_started = true; // estado atual
	QString strState = "--- INICIADO - tecle <enter> para parar";
	setWindowTitle(strState);
	m_text->setFirstString(strState);
	moveBallToInitialPosition();
}

// "stopGame": posiciona "m_started" como "false"
// desabilitando o tratamento dos eventos de teclado e mouse para movimento da bola,
// e muda t�tulo da janela
void MainWidget::stopGame()
{
	m_started = false;
	QString strState = "+++ PARADO - tecle <enter> para iniciar";
	setWindowTitle(strState);
	m_text->setFirstString(strState);
}

// move bola para posi��o inicial:
void MainWidget::moveBallToInitialPosition()
{
	const qreal ballX = m_player1->x()+m_player1->pixmap().width()-3;
	const qreal ballY = m_player1->y() + (m_player1->pixmap().height()>>1)-(int(m_ball->rect().height())>>1);
	m_ball->setPos( ballX, ballY);
}

// "moveBall": ir� mover a bola;
// ela � chamada a partir dos eventos de teclado e mouse;
// se "m_started" for "false", retornar� sem fazer nada
// e com isso esses eventos ser�o ignorados:
void MainWidget::moveBall(qreal xMove, QEvent * event)
{
	if ( !m_started )
	{
		event->ignore(); // se n�o iniciado, ignora o evento de teclado ou mouse
		return;
	}

	// limites para movimento da bola:
	const qreal leftLimit = qreal(m_player1->x()+m_player1->pixmap().width()-3);
	const qreal rightLimit = qreal(m_player2->x()-m_ball->rect().width()+3);

	qreal ballLeft;
	// definir movimento, sabendo-se que MouseMove envia um valor absoluto
	// e os demais eventos enviam um valor relativo:
	if ( event->type() == QEvent::MouseMove )
	{
		// neste caso xMove � a coordenada X do mouse;
		// ent�o � preciso mape�-la para as coordenadas da scene:
		QPointF pt =  m_gView->mapToScene( int(xMove), 0);
		ballLeft = pt.x() - m_ball->rect().width()/2;
	}
	else
		ballLeft = m_ball->pos().x() + (xMove * m_ballXMove);

	ballLeft = qMax ( ballLeft, leftLimit);
	ballLeft = qMin ( ballLeft,  rightLimit);

	// move:
	m_ball->setPos( ballLeft, m_ball->pos().y());
}

// ===========================================================================
// ========================= MainWidget::Text
// ===========================================================================

// Text construtor:
MainWidget::Text::Text()
{
	m_font.setFamily("Verdana");
	m_font.setPixelSize(12); // <- *** ao inv�s de setPointSize(8)
						// pois a impressora est� sendo usada em ALTA resolu��o
						// e o tamanho da fonte deve ser escal�vel do mesmo modo
						// que os demais itens de desenho.

	m_font.setBold(true);
	m_wMargin = 4;
	m_topMargin = 10;

	// preenche o texto a ser desenhado no tabuleiro
	// deixando a primeira linha em branco
	// (ela sera acrescentada posteriormente em fun��o do estado do "jogo"):
	m_strings	<< "" // primeira linha com conte�do vari�vel
				<< "" // linha em branco
				<< "<Enter> ou o bot�o esquerdo do mouse param/iniciam o \"jogo\"..."
				<< "Setas movimentam a bola uma posi��o � esquerda ou � direita."
				<< "Movimento do mouse move bola at� posi��o horizontal do mouse."
				<< "Bot�o direito do mouse permite imprimir a tela em um arquivo PDF.";
}
void MainWidget::Text::setMainAttributes(qreal mainWidth, qreal mainHeight, const QColor & mainBackColor, QColor & rectPenColor)
{
	m_mainWidth = mainWidth;
	m_mainHeight = mainHeight;
	m_mainBackColor = mainBackColor;
	m_rectPenColor = rectPenColor;
}

void MainWidget::Text::setFirstString(const QString & firstString)
{
	prepareGeometryChange();
	m_strings.front() = firstString; // muda primeira linha da stringlist do texto
	adjustRect();
	update();
}

// define ret�ngulo que servir� de moldura e fundo para o texto
// tendo como coordenadas a posi��o do retangulo do texto dentro da scene:
void MainWidget::Text::adjustRect()
{
	// FontMetrics permite apurar altura e largura de linhas
	// em fun��o da fonte em uso:
	QFontMetrics fm(m_font);
	int interLines = qMax(1, fm.leading()); // espa�o entre linhas
	int h = (4*interLines) // espa�os para margem superior e inferior do texto dentro do ret�ngulo (2 acima e 2 abaixo)
			+( (fm.lineSpacing()+interLines) * m_strings.size() );// altura de uma linha * quantidade de linhas

	// encontrar a linha de maior largura:
	int w = 0;
	foreach ( const QString & line, m_strings )
		w = qMax ( w, fm.width(line) ); // pega a maior largura at� aqui

	// adiciona a largura da margem lateral � esquerda e � direita
	w+=(m_wMargin<<1);
	// define coordenada x:
	int x = int(m_mainWidth/2)-(w>>1);
	// coordenada y:

	int y = m_topMargin + int(m_mainHeight*0.025);
	m_rectWidth = w;
	m_rectHeight = h;
	setPos(x,y);
}

// retorna ret�ngulo em coordenadas locais
QRectF MainWidget::Text::boundingRect() const
{
	return QRectF( 0,0, m_rectWidth, m_rectHeight);
}

// desenha:
void MainWidget::Text::paint ( QPainter * paint, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
	// desenha moldura do texto:
	paint->setPen( m_rectPenColor );
	paint->setBrush( m_mainBackColor.lighter(1000) );
	QRectF rect = boundingRect();
	paint->drawRect(rect);
	// fonte:
	paint->setFont( m_font );
	// coordenada x para o texto dentro do retangulo
	qreal xLine = rect.x()+m_wMargin;
	// largura do texto; desconta as margens esquerda e direita:
	qreal wLine = rect.width()-(m_wMargin<<1);
	// coordenada y para o texto dentro do ret�ngulo e altura de uma linha
	// em fun��o da m�trica do texto(dependente de sua fonte)
	QFontMetrics fm(m_font);
	int interLines = qMax(1, fm.leading()); // espa�o entre linhas
	qreal yLine = rect.y()+ (interLines<<1);
	qreal hLine = fm.height();

	// cor do texto:
	paint->setPen( QPen ( m_mainBackColor ) );
	foreach( const QString & line, m_strings)
	{
		// desenha texto:
		paint->drawText( QRectF(xLine , yLine,  wLine, hLine),
							Qt::AlignHCenter, line );
		// coordenada y da pr�xima linha:
		yLine+= fm.lineSpacing()+interLines;
	}
}


// =========================================================================
// ===================  class GView
// =========================================================================

// construtora
GView::GView(QWidget * parent, const bool & started)
	: QGraphicsView(parent)
	, m_started ( started )
{
	// desabilita barras de scroll:
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	// suaviza��o de bordas e pixmaps:
	setRenderHints(QPainter::Antialiasing
						| QPainter::TextAntialiasing
					| QPainter::SmoothPixmapTransform);

	setMouseTracking(true);
}

// evento resize:
void GView::resizeEvent(QResizeEvent *event)
{
	QGraphicsView::resizeEvent(event);

	// Neste exemplo, o �nico interesse neste evento �
	// fazer com que a scene acompanhe os redimensionamentos da view
	// (a qual ser� redimensionada pelo Layout do widget principal).
	// Isso � feito pela fun��o "fitInView":
        fitInView(scene()->sceneRect(),	Qt::KeepAspectRatio);

	// "Qt::KeepAspectRatio": mantem as propor��es nas dimens�es dos itens;
	// com isso, a scene poder� n�o ocupar toda a �rea dispon�vel
	// quando as diferen�as entre largura e altura n�o permitirem
	// um redimensionamento dos itens respeitando suas propor��es originais.
	// OBS: Caso us�ssemos "Qt::IgnoreAspectRatio", a scene sempre ocuparia
	// TODA a �rea da view; mas, ao usar inteiramente a nova dimens�o da view,
	// poderia ocorrer uma distor��o no aspecto dos itens (largura X altura).
}

// evento "release" do teclado:
void GView::keyReleaseEvent( QKeyEvent * event )
{
	// analisa a �ltima tecla pressionada/liberada:
	switch( event->key() )
	{
		case Qt::Key_Enter:
		case Qt::Key_Return:
			emit stateChanged();
			return;

		case Qt::Key_Left:
		case Qt::Key_Backtab:
		case Qt::Key_Up:
			emit moveBall( -1, event); // move bola para a esquerda
			return;

		case Qt::Key_Tab:
		case Qt::Key_Right:
		case Qt::Key_Down:
			emit moveBall( 1, event);  // move bola para a direita
			return;

		default:
			event->ignore(); // ignora outros bot�es
	}
}
// evento "Release" do mouse:
void GView::mouseReleaseEvent ( QMouseEvent * event )
{
	// analisa qual o bot�o do mouse que foi pressionado:
	switch( event->button() )
	{
		case Qt::LeftButton :
			emit stateChanged(); // inicia/finaliza movimento da bola
			return;

		case Qt::RightButton:
			print();
			return;

		default:
			event->ignore(); // ignora outros bot�es
	}
}
// evento "move" do mouse:
void GView::mouseMoveEvent ( QMouseEvent * event )
{
		emit moveBall( event->x(), event );
}

// imprime em um pdf:
void GView::print()
{
	// solicita um nome para o arquivo pdf a gravar:
	QString fileName = QFileDialog::getSaveFileName(this,
				"Informe o nome do arquivo para gravar o pdf",
				".", "Pdf files (*.pdf)");

	if (fileName.isEmpty())
		return;

	if ( fileName.right(4).toLower() != ".pdf" )
		fileName+=".pdf";

	QPrinter printer(QPrinter::HighResolution);
	// define formato e arquivo pdf
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(fileName);

	// cria um objeto painter relacionado ao objeto printer(que ser� o seu "device")
	QPainter paint(&printer); // objeto QPainter direcionado para "printer"

	// copia da view para "printer", escalando para as dimens�es de "printer"
	render(&paint);

	// PRINT: abrir J� o pdf:
	QDesktopServices::openUrl( QUrl("file:///" + fileName) );
}
