#ifndef _DLG_H_INC_
#define _DLG_H_INC_

#include "ui_designer.h"

class Dlg : public QDialog , private Ui::Dialog
{									
	Q_OBJECT

	public:
		Dlg( QWidget *parent = 0 ) // construtor
		: QDialog (parent )
		{
			setupUi(this); // usa configura��es de Ui::Dialog
		}

	// agora posso acrescentar m�todos espec�ficos;
	// e posso acrescentar tamb�m novos signals e slots
	// .....................

};

#endif // ( _DLG_H_INC_ )

