#include "widget.h"
#include "ui_widget.h"

#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

	// Cria um menu que depois ser� usado como um "contextMenu".
	// O "contextMenu" ser� exibido a partir do clique do
	// bot�o direito do mouse ou outra a��o padr�o determinada pela plataforma.
	// Nesse momento, sera chamada a virtual "contextMenuEvent", redefinida abaixo,
	// a qual ir� exibir o "contextMenu".
	m_contextMenu = new QMenu(this); // cria objeto menu

	// Cria as "actions" que ser�o executadas
	// a partir do "contextMenu:
	m_action1 = new QAction("1) - Action-1", this);
	m_action2 = new QAction("2) - Action-2", this);

	// Conecta o "signal triggered" de todas as "actions"
	// aos respectivos "slots" fornecidos por esta classe(classe "Widget")
	connect(m_action1, SIGNAL(triggered()),
					this, SLOT( onAction1() ) );
	connect(m_action2, SIGNAL(triggered()),
					this, SLOT( onAction2() ) );


	// Adiciona os objetos "QAction" ao menu:
	m_contextMenu->addAction(m_action1);
	m_contextMenu->addAction(m_action2);

	// OPCIONALMENTE, tamb�m quero que o contextMenu
	// seja exibido quando este bot�o for pressionado:
	ui->pushButton->setMenu( m_contextMenu );

	// Devido ao uso da fun��o "setMenu",
	// o "pushButton" ir� exibir uma pequena seta
	// indicando que ao ser pressionado ele abrir� alguma coisa.
}

Widget::~Widget()
{
	delete ui;
}

// Fun��o virtual "contextMenu":
// chamada quando "contextMenu" deve ser exibido;
// e o seu argumento "event" permite capturar facilmente
// a posi��o onde o menu de contexto deve ser exibido:
void Widget::contextMenuEvent(QContextMenuEvent *event)
{
	// executa o menu na posi��o fornecida pelo parametro "event",
	// ponteiro para um objeto "QContextMenuEvent":
	m_contextMenu->exec(event->globalPos());
	
	// globalPos() -> posi��o ideal para exibi��o do menu
}

// "slots" para as duas "actions" executadas
// a partir do "contextMenu":
void Widget::onAction1()
{
	QMessageBox::information(this, "ACTION-1", "1) - testando a ACTION-1");
}
void Widget::onAction2()
{
	QMessageBox::information(this, "ACTION-2", "2) testando a ACTION-2");
}

