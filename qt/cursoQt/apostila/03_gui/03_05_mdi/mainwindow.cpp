#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#include <QMdiSubWindow>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>

#include "form1.h"
#include "form2.h"
#include "webbrowser.h"
#include "aboutdlg.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	// inicializa com zero os ponteiros para as duas "subwindows"
	// que s� devem ter uma inst�ncia em execu��o:
	, m_subwinForm1(0)
	, m_subwinForm2(0)
{
	ui->setupUi(this);

	QDesktopWidget * desk = qApp->desktop();

	int w = desk->width(); // largura dispon�vel no desktop
	int h = desk->height(); // altura, idem

	int mw_w = int ( w *0.8) ; // largura: 80% do desktop
	int mw_h  = int ( h * 0.7); // altura: 70%

	// centralizar horizontalmente:
	int mw_x = int ( w * 0.1 );  // x: 10% do desktop
				// logo teremos 10% � esquerda, 80% do widget
				// sobrando 10% � direita: centralizou.

	// verticalmente: ao inv�s de centralizar,
	// vou me aproximar do topo:
	int mw_y = int ( h * 0.05 );

	// define posi��o "x,y"(canto esquerdo superior)
	// e dimens�es(altura, largura) iniciais:
	setGeometry(mw_x, mw_y, mw_w, mw_h );
	// define tamanho m�nimo: 60% do inicial:
	setMinimumSize( int(mw_w*0.6), int(mw_h*0.6) );

	// Ir� acrescentar aqui um "label" e uma "comboBox" � "toolbar"
	// (n�o conseguimos fazer isso no QtDesigner):
	QLabel * labelCombo = new QLabel(" Visualiza��o: ");
	QComboBox * comboViewMode = new QComboBox;

	// adiciona itens � comboBox:
	comboViewMode->addItem("Abas (triangulares)");
	comboViewMode->addItem("Abas (retangulares)");
	comboViewMode->addItem("Cascata");
	comboViewMode->addItem("Telhas");

	// agora, adiciona um "separator",
	// o label e a comboBox na toolBar:
	ui->mainToolBar->addSeparator();
	ui->mainToolBar->addWidget( labelCombo );
	ui->mainToolBar->addWidget(comboViewMode);

	// signal "activated" da comboBox:
	// ao contr�rio do signal "currentIndexChanged",
	// este signal � emitido sempre que um item da comboBox
	// � selecionado (mesmo que o �ndice n�o tenha sido alterado):
	connect( comboViewMode, SIGNAL(activated(int)),
				 this, SLOT(selectViewMode(int)) );
	// e o slot "selectViewMode" ser� chamado sempre que uma linha da comboBox
	// for pressionada, mesmo que j� seja a linha atual.

	// for�a sele��o inicial do modo de visualizac�o
	// usando a primeira linha da comboBox:
	selectViewMode(0);
}

MainWindow::~MainWindow()
{
	delete ui;
}

// slot conectado ao signal "triggered" da action "actionForm1"
// - ir� criar nova inst�ncia do widget "Form1" e sua respectiva "subWindow":
void MainWindow::on_actionForm1_triggered()
{
	// Para "Form1" admitimos apenas uma inst�ncia em execu��o.
	// Ent�o, sua "subWindow" ser� criada apenas se n�o houver uma inst�ncia j� em execu��o:

	if ( !m_subwinForm1 ) // ... se n�o est� instanciada...
	{
		// Cria nova inst�ncia de "Form1" ("new Form1")
		// que sera o "internal widget" da nova instancia de "m_subWinForm1":
		m_subwinForm1 = createSubWindow_( new Form1 );

		// Conecta o signal "destroyed" desta subWindow(chamado pela destrutora da base QObject)
		// ao slot "subWinForm1Destroyed" da "mainWindow";
		// com isso saberemos que a inst�ncia atual de "Form1" ser� destru�da logo em seguida:
		connect(m_subwinForm1, SIGNAL(destroyed()),
					this, SLOT(subWinForm1Destroyed()) );

		// Ir� conectar um signal de "Form1" a um slot de "Form2":
		connectForm1ToForm2_();
	}

	// Exibir essa subWindow, conforme a necessidade:
	showSubWindow_(m_subwinForm1);

	// Coloca essa "subWindow" no primeiro plano da mdiArea:
	ui->mdiArea->setActiveSubWindow(m_subwinForm1);
}

// slot conectado ao signal "triggered" da action "actionForm2"
// - ir� criar nova inst�ncia do widget "Form2" e sua respectiva "subWindow":
void MainWindow::on_actionForm2_triggered()
{
	// Para "Form2" admitimos apenas uma inst�ncia em execu��o.
	// Ent�o, sua "subWindow" ser� criada apenas se n�o houver uma inst�ncia j� em execu��o:

	if ( !m_subwinForm2 ) // ... se n�o est� instanciada ...
	{
		// Cria nova inst�ncia de "Form2" ("new Form2")
		// que sera o "internal widget" da nova instancia de "m_subWinForm2":
		m_subwinForm2 = createSubWindow_( new Form2 );

		// Conecta o signal "destroyed" desta subWindow(chamado pela destrutora da base QObject)
		// ao slot "subWinForm2Destroyed" da "mainWindow";
		// com isso saberemos que a inst�ncia atual de "Form2" ser� destru�da logo em seguida:
		connect(m_subwinForm2, SIGNAL(destroyed()),
					this, SLOT(subWinForm2Destroyed()) );

		// Ir� conectar um signal de "Form1" a um slot de "Form2":
		connectForm1ToForm2_();
	}

	// Exibir essa "subWindow", conforme a necessidade:
	showSubWindow_(m_subwinForm2);

	// Coloca essa "subWindow" no primeiro plano da mdiArea:
	ui->mdiArea->setActiveSubWindow(m_subwinForm2);
}

// slot conectado ao signal "triggered" da action "actionBrowser"
// - ir� criar nova inst�ncia do widget "WebBrowser" e sua respectiva "subWindow":
void MainWindow::on_actionBrowser_triggered()
{
	// Para "WebBrowser" admitimos m�ltiplas inst�ncias em execu��o.
	// Ent�o, aqui sempre ser� criada uma nova inst�ncia de uma "subWindow"
	// (mesmo que j� existam outras inst�ncias em execu��o).
	// Cada inst�ncia dessa subWindow receber� como seu "internal widget"
	// uma nova instancia do widget "WebBrowser" ("new WebBrowser"):
	QMdiSubWindow * browser = createSubWindow_( new WebBrowser);

	// Exibir essa "subWindow", conforme a necessidade:
	showSubWindow_(browser);

	// Coloca essa "subWindow" no primeiro plano da mdiArea:
	ui->mdiArea->setActiveSubWindow(browser);
}

// fun��o auxiliar que cria nova inst�ncia de uma "subWindow",
// j� definindo qual ser� o "internal widget" dessa "subWindow":
QMdiSubWindow * MainWindow::createSubWindow_( QWidget * internalWidget )
{
	//Uma nova inst�ncia de determinada "subWindow" � criada
	// e adicionada ao objeto "mdiArea".
	// E um determinado "widget" � passado aqui para que se torne
	// o "internal Widget" dessa "subWindow":
	QMdiSubWindow * subWin = ui->mdiArea->addSubWindow( internalWidget );

	// Atributo "DeleteOnClose": no evento "close" dessa "subWindow",
	// a inst�ncia do objeto "QMdiSubWindow" ser� liberada/destru�da:
	subWin->setAttribute(Qt::WA_DeleteOnClose);

	return subWin; // retorna endere�o da nova "subWindow"
}

// fun��o auxiliar que exibe cada uma das subWindows:
void MainWindow::showSubWindow_( QMdiSubWindow * subWin )
{
	// checa se a "subWindow" est� minimizada
	// ou se ainda n�o foi exibida:

	if ( ui->mdiArea->viewMode() == QMdiArea::SubWindowView && subWin->isMinimized())
		subWin->showNormal(); // restaura se estiver minimizada
	else if ( !subWin->isVisible() )
		subWin->show(); // exibe (se n�o est� vis�vel � porque foi criada agora);
}

// fun��o auxiliar que ir� conectar um signal de "Form1" a um slot de "Form2":
void MainWindow::connectForm1ToForm2_()
{
	// Checa se as duas "subWindows" desses dois "forms" j� foram instanciadas:
	if ( m_subwinForm1 && m_subwinForm2 )
	{
		// Recupera os objetos "Form"
		// que s�o os "internal widget" de cada subWindow:
		Form1 * form1 = static_cast<Form1*>(m_subwinForm1->widget() );
		Form2 * form2 = static_cast<Form2*>(m_subwinForm2->widget() );

		// Conecta o signal "editTextChanged" do "form1"
		// ao slot "externText" do "form2"
		// (ver form1.h/form1.cpp e form2.h/form2.cpp).
		// Nesse signal, o "form1" envia o texto do seu "lineEdit"
		// e o "form2", no slot, adiciona esse texto em sua "comboBox":
		connect( form1, SIGNAL(editTextChanged(QString)),
					   form2, SLOT(externText(QString)) );
	}
}

// slot conectado ao signal "activated" da comboBox
// - permite a sele��o do modo de visualiza��o:
void MainWindow::selectViewMode(int index)
{
	if ( index > 1 ) // ir� usar subWindows (e n�o abas)
	{
		// ent�o, antes de modificar modo de exibi��o,
		// checa se h� alguma "subWindow" minimizada:
		QList<QMdiSubWindow*> subWindows = ui->mdiArea->subWindowList();
		foreach ( QMdiSubWindow* subWin, subWindows )
			showSubWindow_(subWin); // se estiver minimizada, ser� restaurada
	}

	switch ( index )
	{
		case 0: // abas triangulares
			ui->mdiArea->setViewMode(QMdiArea::TabbedView);
			ui->mdiArea->setTabShape(QTabWidget::Triangular);
			break;

		case 1: // abas retangulares com cantos arredondados
			ui->mdiArea->setViewMode(QMdiArea::TabbedView);
			ui->mdiArea->setTabShape(QTabWidget::Rounded);
			break;

		case 2: // cascata
			ui->mdiArea->setViewMode(QMdiArea::SubWindowView);
			ui->mdiArea->cascadeSubWindows();
			break;

		case 3: // telhas
			ui->mdiArea->setViewMode(QMdiArea::SubWindowView);
			ui->mdiArea->tileSubWindows();
			break;
	}

	// Se tiver mudado o "viewMode" de "subWindows" para "abas",
	// a janela ativa ir� apresentar a action "restore" no menu de contexto.
	// Pois, como j� est� ativa, o slot "subWindowActivated" n�o ser� chamado;
	// ent�o, for�amos aqui que o "restore" seja desabilitado
	// (ATEN��O: ver, abaixo, coment�rio sobre a "Bug Tracker" de Qt)
	on_mdiArea_subWindowActivated( ui->mdiArea->activeSubWindow()); // desabilitar a action "restore"
}

// Abaixo:
// slot para o signal "subWindowActivated";
// quando uma subWindow � ativada, caso o "viewMode" seja "abas",
// podemos desabilitar a op��o "restore" do menu de contexto.
// ==========================================================================
// ATEN��O: PROVAVELMENTE isto ser� desnecess�rio em pr�ximas vers�es de Qt.
// Ver a entrada sobre esse t�pico na "Bug Tracker" de Qt em:
// http://qt.nokia.com/developer/task-tracker/index_html?method=entry&id=211669
// Entrada: 211669 - Restoring a QMdiSubWindow when QMdiArea viewMode is QMdiArea::TabbedView
// Status atual:
// 2008-05-15 19:50 - Entry created: Task status changed to 'Open', Resolution set to 'Pending', Version found set to '4.4.0', Version for fix set to 'Not scheduled', Priority changed to 'No Priority'
// 2008-05-19 10:24 - Version for fix set to 'Some future release'
// 2008-05-19 10:24 - Priority changed to '3'
// ==========================================================================


void MainWindow::on_mdiArea_subWindowActivated(QMdiSubWindow* subWin)
{
	// se est� usando abas...
	if ( subWin && ui->mdiArea->viewMode() == QMdiArea::TabbedView )
	{
		// com abas, desabilitar a action "restore"

		// 1) Primeira alternativa (a mais simples):
		//    confiando no tradi��o, desabilitamos a primeira action da lista de actions
		//    (que *costuma ser* "Restore").
		//    *Hoje* isso funciona(sempre foi assim).

		if ( subWin->actions().size()> 0)
			subWin->actions().first()->setEnabled(false);

		// A primeira action *deve ser*(!) o restore...
		// Exceto se algo mudar no futuro,
		// OU SE modificarmos os "windowFlags" das subWindows
		// (dependendo da mudan�a feita, o menu de sistema pode ser alterado por decorr�ncia)

		// 2) segunda alternativa: localizar a "action restore" pelo seu texto;
		//    Problema: a string "restore" deve ser obrigatoriamente traduzida
		//    caso se esteja usando dicion�rios (internacionaliza��o).
		//    E a tradu��o de "restore" deve estar presente no(s) dicion�rio(s) para Qt, j� que e uma string de Qt.
		//    e tamb�m no(s) dicion�rio(s) da aplicac�o de forma id�ntica:
		/*
			QList<QAction*> swActions = subWin->actions();
			foreach( QAction * swAction, swActions )
			{
				// usar "Rest" (ao inv�s de "Restore"): assim, esse m�todo
				// pode funcionar para alguns idiomas sem tradu��o
				// - ingl�s, portugu�s, italiano, espanhol, franc�s, alem�o...
				if ( swAction->text().indexOf( tr("Rest") ) >= 0 )
				{
					swAction->setEnabled(false);
					break;
				}
			}
		*/

		// 3) Terceira alternativa (mais elegante, MAIS SEGURA e... mais dispendiosa):
		//    percorrer a lista de actions como feito na segunda alternativa
		//    para descobrir qual aquela cujo *�cone* corresponde � action "restore":
		/*
			QList<QAction*> swActions = subWin->actions();
			foreach( QAction * swAction, swActions )
			{
				// podemos comparar o �cone da action com o �cone padr�o "QStyle::SP_TitleBarNormalButton"
				// que � aquele usado na action "restore
				if ( swAction->icon().pixmap(32).toImage() ==
					 qApp->style()->standardIcon(QStyle::SP_TitleBarNormalButton).pixmap(32).toImage() )
				{
					swAction->setEnabled(false);
					break;
				}
				// *** o custo extra da alternativa acima est� na convers�o de QPixMap para QImage
			}
		*/

		// 4) Uma quarta alternativa: mais radical e que resolve o problema na raiz,
		//    ainda que se seja a mais onerosa de todas.
		//    Neste caso, alteramos o "systemMenu" de cada "subWindow" (com "setSystemMenu").
		//    Assim, teremos um "systemMenu" espec�fico para os dois diferentes "viewModes"
		//	  Mas isso seria muito trabalhoso.

		// ====================================================================
		// OBS: Como a solu��o do problema em Qt J� EST� AGENDADA
		//      (ainda que para 'Some future release')
		//      preferimos usar aqui a solu��o mais simples (primeira alternativa)
		//      (isto �, menos trabalhosa e tamb�m menos onerosa).
		// ====================================================================

	}
}


// slot para o signal "triggered" da action "actionAbout"
// - abre o di�logo modal "About"(Sobre):
void MainWindow::on_actionAbout_triggered()
{
	AboutDlg dlg(this);
	dlg.exec(); // o di�logo modal � aberto com "exec"
				// e, assim, enquanto n�o for fechado,
				// n�o poderemos dar foco em qualquer outra janela
				// da mesma aplicac�o.
}

// slot conectado ao signal "triggered" da action "actionQuit"
// - ir� chamar "close", gerando uma chamada a "closeEvent";
//   e, em "closeEvent", a MainWindow poder� ser fechada, caso o evento seja aceito.
//   Por default o evento � sempre aceito.
//   Mas, neste exemplo, redefinimos a virtual "closeEvent",
//   podendo aceit�-lo ou n�o (ver "closeEvent" mais abaixo).
void MainWindow::on_actionQuit_triggered()
{
	close();
}

//  closeEvent: quando a mainWindow recebe o evento "close"
//  podemos aceit�-lo ou n�o (por default ele � aceito):
void MainWindow::closeEvent(QCloseEvent * event)
{
	// Checa se h� alguma "subWindow" aberta:
	if ( ui->mdiArea->subWindowList().size() > 0 )
	{
		QMessageBox::StandardButton result
				= QMessageBox::question(this, "Encerrar a aplica��o",
					"Existem janelas abertas.\nEncerrar mesmo assim?",
					QMessageBox::Yes | QMessageBox::No );

		if ( result == QMessageBox::Yes )
			ui->mdiArea->closeAllSubWindows(); // Fecha todas as SubWindows. Em seguida, a MainWindow ser� fechada e a aplica��o ser� encerrada.
		else
			event->ignore(); // Este evento deve ser IGNORADO: a MainWindow n�o ser� fechada.
	}
}
