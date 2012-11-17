#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>

namespace Ui
{
    class Widget;
}

class QMenu;
class QAction;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

	// contextMenu:
	QMenu * m_contextMenu;
	// actions para o contextMenu:
	QAction * m_action1;
	QAction * m_action2;

private slots:
	// slots para as duas actions executadas
	// a partir do contextMenu:
	void onAction1();
	void onAction2();

protected:
	// redefine a virtual contextMenuEvent
	// � aqui que o menu ser� executado
	// quando o bot�o direito do mouse for acionado
	// (ou outra a��o padr�o prevista pela plataforma)
	void contextMenuEvent(QContextMenuEvent *event);
};

#endif // WIDGET_H
