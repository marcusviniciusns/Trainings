#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui/QWidget>
#include <QImage>

class QPrinter;

namespace Ui
{
    class MainWidgetClass;
}

// Neste exemplo temos 2 "players" (2 imagens)
// e uma bola que se movimenta horizontalmente entre eles.

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    Ui::MainWidgetClass *ui;

	// habilita tratamento de eventos de teclado e mouse:
	void startGame();
	// desabilita o tratamento desses eventos:
	void stopGame();

	// eventos de mouse e teclado (virtuais da base):
	void keyReleaseEvent( QKeyEvent * event );
	void mouseReleaseEvent ( QMouseEvent * event );
	void mouseMoveEvent ( QMouseEvent * event );

	// movimentar a bola em fun��o dos eventos de teclado ou mouse:
	void moveBallToInitialPosition();
	void moveBall(int moveX, QEvent * event);

	// paintEvent(virtual); pintura da janela:
	void paintEvent(QPaintEvent * pEv);
	// auxiliares de "paintEvent" para desenhar cada item:
	void drawBoard(QPainter & paint);
	void drawText ( QPainter & paint);
	void drawPlayers(QPainter & paint);
	void drawBall(QPainter & paint, const QRect & rect);
	void eraseBall(QPainter & paint, const QRect & rect);

	// struct para os dois "players"
	struct Player : public QImage // o player � uma imagem
	{
		QPoint m_winPoint; // coordenadas x e y na janela
	} ;

	// struct para a "bola" (uma ellipse):
	struct Ball : public QRect // a bola � uma ellipse contida em um ret�ngulo
	{
		QBrush m_brush; // cor de fundo
		int m_xMove; // deslocamento horizontal
	};

	// struct para um texto que ser� desenhado no widget:
	struct Text
	{
		Text(); // construtor
		void adjustRect(int idealWidth, int idealHeight); // define ret�ngulo em fun��o das strings contidas na lista
		void draw(QPainter & painter, const QBrush & background); // desenha

		QStringList m_strings; // lista de strings para compor o texto
		QRect m_rect;  // ret�ngulo que servir� de moldura e fundo para o texto
		QFont m_font;  // fonte
		int m_wMargin; // margem lateral (esquerda/direita)
		int m_topMargin; // margem do topo
	};

	Player m_player1; // imagem
	Player m_player2; // imagem
	Ball m_ball;   // elipse
	Text m_text; // texto

	bool m_started; // se verdadeiro, trata eventos de teclado e mouse para movimentar a bola
					// do contr�rio ignora esses eventos.
	// cor de fundo:
	QBrush m_background;

	// enumera a��es poss�veis para o "paintEvent"
        enum DrawActions { DrawOff, DrawBoard,
                                DrawBall, EraseBall, DrawToPrinter
					};

	DrawActions m_drawAction;

	// objeto para impress�o:
	QPrinter  * m_print;

	// dimens�es iniciais do widget:
	int m_idealWidth;
	int m_idealHeight;
};

#endif // MAINWIDGET_H
