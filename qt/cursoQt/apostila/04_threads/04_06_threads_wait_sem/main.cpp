
#include <QtCore>

#include <iostream>
#include <cstdlib>

enum { TOTAL_TEST_THREADS = 3 , TOTAL_PROCESS = 2 };

class SyncMainThread
{
		// sem�foro para "main" aguardar
		// um ponto de processamento de cada "TestThread":
		QSemaphore sem_waitOther;

	public:
		// espera que um outro thread atinja um ponto de processamento:
		void waitOther() {
			sem_waitOther.acquire(1);
		}
		// um outro thread libera o main-thread 
		// que est� (ou vai estar...) em "wait":
		void wakeup() {
			sem_waitOther.release(1);
		}
} ;

// cria objeto para sincroniza��o do main-thread:
SyncMainThread syncMain;

// mutex para proteger o acesso ao console:
static QMutex mut_cout;

// sinaliza se o "TestThread" deve executar ou parar:
bool g_bTestThreadsProcess;

// classe para teste de novos threads:
class TestThread : public QThread
{
	int _m_nThreadNumber;
	QSemaphore sem_waitTask;
	
	public:
		void  setNumber( int nThreadNumber ) { _m_nThreadNumber = nThreadNumber; }
		
		// fun��o para que este thread aguarde 
		// at� que outro thread ordene a execu��o de uma tarefa
		void waitForTask( ) { 
			sem_waitTask.acquire(1);
		}

		// fun��o para que outro thread ordene que este thread
		// execute uma tarefa (liberando-o da "wait")
		void doTask() {
			sem_waitTask.release(1);
		}

	protected: 
		void run(); // redefine virtual da base
} ; 

void TestThread::run()
{
	// realiza alguma tarefa da qual 'main' depende neste momento
	// ..............................

	// liberar o main-thread que est� aguardando at� que cada thread 
	// tenha iniciado e realizado determinada tarefa inicial:
	syncMain.wakeup();
	
	mut_cout.lock();
		std::cout << "TestThread(#" << _m_nThreadNumber << ")::run: vou aguardar que haja algo para fazer." << std::endl;
	mut_cout.unlock();

	while ( g_bTestThreadsProcess )
	{
		// aguarda at� que exista algo para fazer
		// (isto �, quando o main-thread liberar o sem�foro):
		waitForTask();

		if ( g_bTestThreadsProcess )
		{
			mut_cout.lock();
				std::cout << "TestThread(#" << _m_nThreadNumber << ")::run: agora vou trabalhar." << std::endl;
			mut_cout.unlock();
			
			// Tarefa_a_Executar(); // aqui estaria o processamento deste thread...				
			
			// acordar o main-thread que est� aguardando que esta tarefa seja executada
			syncMain.wakeup();
		}
	}

	mut_cout.lock();
		std::cout << "TestThread(#" << _m_nThreadNumber << ")::run: vou encerrar." << std::endl;
	mut_cout.unlock();
}

int main()
{
	g_bTestThreadsProcess = true;

	TestThread testThreads[ TOTAL_TEST_THREADS ];

	// 'main' inicia os threads 
	// e espera que iniciem:

	mut_cout.lock();  // garante que o primeiro cout ser� de main

	int nTh;
	for ( nTh = 0 ; nTh < TOTAL_TEST_THREADS ; ++nTh )
	{
		testThreads[ nTh ].setNumber( nTh+1 );
		// inicia nova inst�ncia de um "TestThread":	
		testThreads[ nTh ].start();

		// aguarda que este "TestThread" tenha iniciado:
		syncMain.waitOther( );
	}

		std::cout << "\n* main: as 3 instancias de TestThread iniciaram" << std::endl; 

	mut_cout.unlock();

	int nProc; // manda os TestThreads executar uma tarefa diversas vezes
	for ( nProc = 0; nProc < TOTAL_PROCESS; ++nProc )
	{
		mut_cout.lock();
			std::cout << "\n* main: Vou fazer com que trabalhem : " << nProc+1 << std::endl;
		mut_cout.unlock();
		
		for ( nTh = 0; nTh  < TOTAL_TEST_THREADS ; ++nTh ) 
		{
			// acorda cada "TestThread" para que execute uma tarefa:
			testThreads[ nTh ].doTask() ; 

			// e 'main' espera at� que cada um encerre o processamento da tarefa
			syncMain.waitOther( );
		}
	}

	mut_cout.lock();
		std::cout << "\n* main: agora vou fazer com que encerrem." << std::endl;
	mut_cout.unlock();

	g_bTestThreadsProcess = false; // posiciona o flag para encerramento

	// aguarda o encerramento dos threads:
	for ( nTh = 0 ; nTh < TOTAL_TEST_THREADS ; ++nTh )
	{
		// acorda todos os threads para executar nova tarefa
		// - e neste caso a tarefa � encerrar 
		// (pois "g_bTestThreadsProcess=false") 
		testThreads[ nTh ].doTask() ; 

		// aguarda que esse thread realmente encerre:
		testThreads[ nTh ].wait(); // "wait" do pr�prio thread (QThread::wait);
	}

	std::cout << "\n* main: threads encerraram; agora, aplicacao vai encerar.\nFIM" << std::endl;
	return 0;
}


