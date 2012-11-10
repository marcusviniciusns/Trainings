#ifndef _THREAD_H_INC_
#define _THREAD_H_INC_

#include <QThread>
#include <QDateTime>
#include <QSemaphore>

class workerThread : public QThread
{
	Q_OBJECT

private:
	QSemaphore waitForTask;// contador iniciado com zero
	bool stopped;

	public:
		workerThread()
		{}

		void doTask () {  // ir� "acordar" o thread que aguarda nesse sem�foro na fun��o "run"
			waitForTask.release(1) ;
		}
		void stop( ) {  // encerra o loop da fun��o "run"
			stopped = true;
			doTask();
		}

		signals:
			// o thread emite este signal para notificar a janela principal
			// de que sua tarefa foi executada e qual foi o resultado:
		void taskDone( const QString & taskResult );

	protected:
		void run()// para a aplica��o o thread come�a a rodar aqui
		{
			stopped = false;
			// inicia o gerador de rand�micos
			// que ser�o usados na tarefa executada neste exemplo:
			qsrand ( QDateTime::currentDateTime().toTime_t() );

			while (!stopped )
			{
				// aguarda indefinidamente at� que uma terefa seja solicitada:
				waitForTask.acquire(1);

				if ( !stopped )
				{
					static int times = 0;
					// a tarefa � gerar a soma de uma quantidade de n�meros rand�micos;
					// ent�o, gera esse soma e emite o signal
					// para que o resultado seja recuperado em outro thread:
					unsigned long sum = 0;
					enum { TOTAL_RANDS = 50*1000*1000 };
					for (int n=0; n < TOTAL_RANDS; ++n )
					{
						// hummm... n�o vou usar um mutex aqui...
						sum += qrand(); // ... ser� que "qrand" � "thread-safe"??? ...
					}

					++times;
					QString result = QString( "Execuc�o #%0 - resultado=[%1]").arg(times).arg(sum) ;

					emit taskDone ( result );
				}
			}
		}
};

#endif // ( _THREAD_H_INC_ )

