#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QByteArray>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

	// fun��o auxiliar que coloca todos os "dockWidgets"
	// em uma mesma "tabBar", cada um deles em uma "aba"
	// (as abas poder�o estar acima ou abaixo, conforme o par�metro "tabPosition"):
	void tabify_(  QTabWidget::TabPosition tabPosition  );

	// fun��o auxiliar que coloca todos os "dockWidgets"
	// lado a lado em um "splitter"
	// (vertical ou horizontal, de acordo com o parametro "orientation"):
	void split_ ( Qt::Orientation orientation);

	// armazenar� o estado inicial da disposi��o dos "dockWidgets"
	// (permitindo restaurar esse estado posteriormente):
	QByteArray m_state;

	// lista dos "dockWidgets" criados
	// (permitir� acessar todos eles em la�os):
	QList < QDockWidget *> m_dockWidgets;
	// mas ser� que essa lista � mesmo necess�ria ???

private slots:

	// slot para o signal "triggered" da action "actionAppQuit":
	void on_actionAppQuit_triggered(); // fecha a "MainWindow" e encerra a aplicac�o

	// slot para o signal "activated" da comboBox que ser� inserida na toolBar
	// - permitir� a sele��o do modo de visualiza��o:
	void selectViewMode(int index);
};

#endif // MAINWINDOW_H

