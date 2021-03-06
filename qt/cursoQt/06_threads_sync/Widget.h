#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "Controller.h"

namespace Ui {
    class Widget;
}
#include <QCloseEvent>

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
void closeEvent(QCloseEvent *);

	signals:
		void startController();

	private slots:
		void on_start_stopButton_clicked();
		void onTaskDone(const QString & data);

	private:
    Ui::Widget *ui;
	 Controller m_controller;


};

#endif // WIDGET_H
