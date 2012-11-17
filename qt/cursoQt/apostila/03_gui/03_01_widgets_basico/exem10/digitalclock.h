	
	/* ==== ENTENDENDO O MOC (meta-object compiler)

		- ele � respons�vel, entre outras coisas, por gerar o c�digo
			para a emiss�o de sinais, invocando os slots que estejam conectados a esses signals.

		- sempre que uma classe tem signals e/ou slots pr�prios
			ela precisa do suporte do MOC;

		- para isso � preciso adicionar a macro Q_OBJECT � classe

			The moc tool reads a C++ header file. 
			If it finds one or more class declarations that contain the Q_OBJECT macro, 
			it produces a C++ source file containing the meta-object code for those classes. 
			Among other things, meta-object code is required for the signals and slots mechanism, 
			the run-time type information, and the dynamic property system.

	*/

#ifndef _DIGITAL_CLOCK_H_INC_
#define _DIGITAL_CLOCK_H_INC_

#include <QLCDNumber>

// class DigitalClock: derivada de QLCDNumber (um display "LCD"),
// acrescentando um slot:
class DigitalClock : public QLCDNumber
{
	// esta classe tem slot pr�prio;
	// ent�o � preciso adicionar a macro "Q_OBJECT"
	// para que seja invocado o "moc" (meta-object compiler)
	Q_OBJECT

	public:
		// construtora:
		DigitalClock(QWidget *parent = 0);

	private slots:
		// o slot "showTime" ser� conectado 
		// ao signal "timeout" de um objeto QTimer
		// e desse modo ser� acionado 
		// sempre que um intervalo de tempo for atingido.
		void showTime();
};


#endif //  ( _DIGITAL_CLOCK_H_INC_ )

