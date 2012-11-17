#ifndef _THREAD_H_INC_
#define _THREAD_H_INC_

#include <QThread>
#include <QDateTime>


class workerThread : public QThread
{
	Q_OBJECT

	public:
		workerThread()
		{}

	protected:
		void run()// para a aplica��o o thread come�a a rodar aqui
		{
			// inicia o gerador de rand�micos
			// que ser�o usados na tarefa executada neste exemplo:
			qsrand ( QDateTime::currentDateTime().toTime_t() );

			// entra no event-loop:
			exec();  // da classe base QThread.
			// a partir daqui o thread s� trabalhar� se receber 
			// um "signal" ou um evento (postEvent)
		}

	public:	
		signals:
			// o thread emite este signal para notificar a janela principal
			// de que sua tarefa foi executada e qual foi o resultado:
		void taskDone( const QString & taskResult );
	
	private slots:
		// o thread oferece esse slot para executar uma tarefa:
		void on_doTask( )
		{
			static int times = 0;
			// a tarefa � gerar a soma de uma quantidade de n�meros rand�micos;
			// ent�o, gera esse soma e emite o signal
			// para que o resultado seja recuperado em outro thread:
			unsigned long sum = 0;
			enum { TOTAL_RANDS = 50*1000*1000 };
			for (int n=0; n < TOTAL_RANDS; ++n )
			{
				sum += qrand(); // ser� que "qrand" � "thread-safe"?
			}

			++times;
			QString result =
					QString( "Execuc�o #%0 - resultado=[%1]").arg(times).arg(sum) ;

			emit taskDone ( result );
		}
	
};

#endif // ( _THREAD_H_INC_ )

