#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	// Acrescentar as "actions" do menu de contexto da MainWindow
	// no menu "Exibir" da menuBar, e, se for o caso, tamb�m na toolbar:

	// Observar que essas "actions" do menu de contexto
	// s�o as "toggleViewAction" de cada "dockWidget".
	// Elas permitem ocultar/exibir cada "dockWidget" (toggle):

/*
	// == realizando o trabalho topicamente:

	// adiciona ao menu "Exibir"
	ui->menuView->addAction( ui->dockWidget_1->toggleViewAction() );
	ui->menuView->addAction( ui->dockWidget_2->toggleViewAction() );
	ui->menuView->addAction( ui->dockWidget_3->toggleViewAction() );
	ui->menuView->addAction( ui->dockWidget_4->toggleViewAction() );

	// adiciona � toolbar:
	ui->mainToolBar->addAction( ui->dockWidget_1->toggleViewAction() );
	ui->mainToolBar->addAction( ui->dockWidget_2->toggleViewAction() );
	ui->mainToolBar->addAction( ui->dockWidget_3->toggleViewAction() );
	ui->mainToolBar->addAction( ui->dockWidget_4->toggleViewAction() );

	// acrescenta todos os "dockWidgets" na lista de "dockWidgets"
	// para facilitar acessos futuros:
	m_dockWidgets << ui->dockWidget_1 << ui->dockWidget_2
					<< ui->dockWidget_3 << ui->dockWidget_4;
*/
	// ==== OU realizando esse mesmo trabalho de modo gen�rico:

/*
	//A)  pega uma refer�ncia para a lista ligada de filhos deste objeto "MainWindow":
	const QObjectList & objectList = children();
	int n = 0;
	// percorre a lista de filhos:
	foreach( QObject * object, objectList )
	{
		// se a classe desse objeto filho for "QDockWidget"
		// ent�o temos o que fazer:
		if ( object->metaObject()->className() == QString("QDockWidget") )
		{
			// tipifica o endereco desse objeto como um "QDockWidget *":
			QDockWidget * dockWidget = static_cast<QDockWidget*>(object);
			// n�o � necess�rio um "dynamic_cast" ou um "qobject_cast"
			// pois, no "if" acima, j� garantimos que a classe deste objeto � "QDockWidget"

			// acrescenta a "toggleViewaction" deste "dockWidget"
			// no menu "Exibir" da menuBar:
			ui->menuView->addAction( dockWidget->toggleViewAction() );
			// e tamb�m na toolbar:
			ui->mainToolBar->addAction( dockWidget->toggleViewAction() );

			// finalmente, insere este objeto "dockWidget" na lista de "dockWidgets"
			// para simplificar acessos futuros:
			m_dockWidgets.append( dockWidget );

			// Adicionalmente, podemos mudar as PROPRIEDADES DA ACTION "toggleViewAction"
			// de cada "dockWidget" (elas s�o exibidas por default no menu de contexto da "MainWindow")
			// Cada uma dessas "actions" permite fechar/exibir o respectivo "dockWidget".
			// Poder�amos mudar suas propriedades mesmo que n�o estiv�ssemos
			// inserindo as "actions" no menu "Exibir" e na "toolbar".
			// De qualquer modo, a altera��o das propriedades ir� modificar a exibi��o/comportamento
			// das "actions" no pr�prio menu de contexto da "MainWindow".

			++n; // simples numerador para as strings que ser�o usadas abaixo;

			// definindo um �cone para a "action":
			dockWidget->toggleViewAction()->setIcon( QIcon( QString(":/images/dock_%1.png").arg(n)) );
			// definindo o "iconText" (que ser� exibido na "toolBar"):
			dockWidget->toggleViewAction()->setIconText(QString("D%1").arg(n));
			// inibindo a exibi��o do �cone em menus (tanto no menu de contexto quanto na "menuBar"):
			 dockWidget->toggleViewAction()->setIconVisibleInMenu(true); // se false teremos a marca "check"
		}
	}
*/
	 //B) usando findChildren<QDockWidget *>:
	 QList < QDockWidget *> dockList = findChildren<QDockWidget*>();
	 int n = 0;
	 // percorre a lista de filhos:
	 foreach( QDockWidget * dockWidget, dockList )
	 {
			 // acrescenta a "toggleViewaction" deste "dockWidget"
			 // no menu "Exibir" da menuBar:
			 ui->menuView->addAction( dockWidget->toggleViewAction() );
			 // e tamb�m na toolbar:
			 ui->mainToolBar->addAction( dockWidget->toggleViewAction() );

			 ++n; // simples numerador para as strings que ser�o usadas abaixo;

			 // definindo um �cone para a "action":
			 dockWidget->toggleViewAction()->setIcon( QIcon( QString(":/images/dock_%1.png").arg(n)) );
			 // definindo o "iconText" (que ser� exibido na "toolBar"):
			 dockWidget->toggleViewAction()->setIconText(QString("D%1").arg(n));
			 // inibindo a exibi��o do �cone em menus (tanto no menu de contexto quanto na "menuBar"):
			  dockWidget->toggleViewAction()->setIconVisibleInMenu(true); // se false teremos a marca "check"
	 }
	// acrescentando a  "toggleAction" da toolBar no menu "Exibir":
	ui->menuView->addSeparator(); // um separator
	ui->menuView->addAction( ui->mainToolBar->toggleViewAction());

	// Ser�o acrescentados � "toolbar" um "label" e uma "comboBox"
	// (n�o conseguimos fazer isso no QtDesigner):
	QLabel * labelCombo = new QLabel(" Organizar janelas: ");
	QComboBox * comboViewMode = new QComboBox;

	// adiciona itens � comboBox
	// (para sele��o do modo de organiza��o dos "dockWidgets"):
	comboViewMode->addItem("Restaurar estado original");
	comboViewMode->addItem("Agrupar em abas (acima)");
	comboViewMode->addItem("Agrupar em abas (abaixo)");
	comboViewMode->addItem("Lado a lado (horizontal)");
	comboViewMode->addItem("Lado a lado (vertical)");

	// agora, adiciona um "separator",
	// o label e a comboBox na toolBar:
	ui->mainToolBar->addSeparator();
	ui->mainToolBar->addWidget( labelCombo );
	ui->mainToolBar->addWidget(comboViewMode);

	// signal activated da comboBox inserida na toolBar:
	// ser� emitido sempre que um item da comboBox
	// � selecionado (mesmo que o �ndice n�o tenha sido alterado)
	// - ser� aqui conectado ao slot "selectViewMode"
	//   que permitir� alterar o modo de organiza��o dos "dockWidgets":
	connect( comboViewMode, SIGNAL(activated(int)),
				 this, SLOT(selectViewMode(int)) );

	// salva estado original da disposi��o dos "dockWidgets"
	// permitindo sua restaura��o futura:
	m_state = saveState(0);
}

MainWindow::~MainWindow()
{
	delete ui;
}

// slot para o signal "activated" da
// comboBox para sele��o do modo de visualiza��o:
void MainWindow::selectViewMode(int index)
{
	restoreState(m_state, 0);
	switch ( index )
	{
		case 0: // restaurar estado original
			break; // j� restaurou acima...

		case 1: // colocar todos em uma mesma "tabBar" (abas acima)
			tabify_( QTabWidget::North );
			break;

		case 2: // colocar todos em uma mesma "tabBar" (abas abaixo)
			tabify_( QTabWidget::South );
			break;

		case 3: // lado a lado em um splitter horizontal
			if ( tabifiedDockWidgets(ui->dockWidget_1).size() > 0 ) // se atualmente est�o em abas
				split_( Qt::Vertical ); // <= *** FIX-ME: isso n�o deveria ser necess�rio (mas o "splitter" horizontal �s vezes falha quando j� est�o todos em abas - verificar vers�es futuras...)

			split_( Qt::Horizontal );
			break;

		case 4: // lado a lado em um splitter vertical
			if ( tabifiedDockWidgets(ui->dockWidget_1).size() > 0 ) // se atualmente est�o em abas
				split_( Qt::Horizontal ); // <= *** FIX-ME: isso n�o deveria ser necess�rio (mas o "splitter" vertical �s vezes falha quando j� est�o todos em abas - verificar vers�es futuras...)

			split_( Qt::Vertical );
			break;
	}
}

// fun��o auxiliar que coloca todos os "dockWidgets"
// em uma mesma "tabBar", cada um deles em uma "aba"
// (as abas poder�o estar acima ou abaixo, conforme o par�metro "tabPosition"):
void MainWindow::tabify_(  QTabWidget::TabPosition tabPosition  )
{
/*
	// == realizando o trabalho topicamente:

	tabifyDockWidget( ui->dockWidget_1, ui->dockWidget_2);
	tabifyDockWidget( ui->dockWidget_2, ui->dockWidget_3);
	tabifyDockWidget( ui->dockWidget_3, ui->dockWidget_4);
*/
	// ==== OU realizando esse mesmo trabalho de modo gen�rico:

/*
  QDockWidget * previousDock = 0;
	// percorre a lista de "dockWidgets" ("m_dockWidgets") alimentada na construtora:
	foreach( QDockWidget * dockWidget, m_dockWidgets )
	{
		if ( previousDock )
			tabifyDockWidget( previousDock, dockWidget);
			// insere na mesma "tabBar" o "dockWidget" anterior e o atual

		previousDock = dockWidget;
	}
*/

	//melhor (sem a lista-membro da classe):
	QDockWidget * previousDock = 0;
	QList<QDockWidget*> dockList = findChildren<QDockWidget*>();
	// percorre a lista de "dockWidgets" ("m_dockWidgets") alimentada na construtora:
	 foreach( QDockWidget * dockWidget,  dockList )
	 {
		 if ( previousDock )
			 tabifyDockWidget( previousDock, dockWidget);
			 // insere na mesma "tabBar" o "dockWidget" anterior e o atual

		 previousDock = dockWidget;
	 }

	// define posi��o da "tabBar" (acima ou abaixo, conforme o valor de "tabPosition"):
	setTabPosition(Qt::AllDockWidgetAreas, tabPosition);
}

// fun��o auxiliar que coloca todos os "dockWidgets"
// lado a lado em um "splitter"
// (vertical ou horizontal, de acordo com o parametro "orientation"):
void MainWindow::split_ ( Qt::Orientation orientation)
{
/*
	// == realizando o trabalho topicamente:

	splitDockWidget( ui->dockWidget_1, ui->dockWidget_2, orientation);
	splitDockWidget( ui->dockWidget_2, ui->dockWidget_3, orientation);
	splitDockWidget( ui->dockWidget_3, ui->dockWidget_4, orientation);
*/
	// ==== OU realizando esse mesmo trabalho de modo gen�rico:
/*
	QDockWidget * previousDock = 0;
	// percorre a lista de "dockWidgets" ("m_dockWidgets") alimentada na construtora:
	foreach( QDockWidget * dockWidget, m_dockWidgets )
	{
		if ( previousDock )
		splitDockWidget( previousDock, dockWidget, orientation);
		// insere no mesmo "splitter" o "dockWidget" anterior e o atual
		// - sendo que a "orientation" do splitter pode ser horizontal ou vertical

		previousDock = dockWidget;
	}
	*/
	// OU:
	QDockWidget * previousDock = 0;
	QList<QDockWidget*> dockList = findChildren<QDockWidget*>();
	// percorre a lista de "dockWidgets" ("m_dockWidgets") alimentada na construtora:
	 foreach( QDockWidget * dockWidget,  dockList )
	 {
		 if ( previousDock )
			 splitDockWidget( previousDock, dockWidget, orientation);
			 // insere no mesmo "splitter" o "dockWidget" anterior e o atual
			 // - sendo que a "orientation" do splitter pode ser horizontal ou vertical

		 previousDock = dockWidget;
	 }

}

// slot para o signal "triggered" da action "actionAppQuit" (usada no "menuFile" e na "toolBar"):
void MainWindow::on_actionAppQuit_triggered()
{
	close(); // fecha a "MainWindow" e encerra a aplicac�o
}

