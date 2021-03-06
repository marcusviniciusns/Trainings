#include <QtCore/QCoreApplication>
#include <QThread>
#include <iostream>
#include <QMutex>
#include <QTime>
#include <cmath>
#include <cstdlib>

class Thread : public QThread
{
	public:
		static void cout_lock (){cout_mutex.lock(); }
		static void cout_unlock(){ cout_mutex.unlock(); }
		static void finish_lock (){finish_mutex.lock(); }
		static void finish_unlock(){ finish_mutex.unlock(); }

	protected:
		void run(); // aqui come�a o novo thread...
	
	private:
		static QMutex cout_mutex;
		static QMutex finish_mutex;
		static int finish_;
};

QMutex Thread::cout_mutex;
QMutex Thread::finish_mutex;
volatile int Thread::finish_ = 0;

bool g_canRun = true;
const int MaxThreads = 30;
QTime time;

void Thread::run()
{
#ifdef Q_OS_WIN
	qulonglong  thId = reinterpret_cast<qulonglong>(currentThreadId());
#else
	qulonglong thId = static_cast<qulonglong>(currenThreadId());
#endif
	
	cout_lock();
	std::cout << "thread #"	<<thId << "iniciando\n";
	cout_unlock();
	
	for (int n=0; n<10 && g_canRun; ++n)
	{
		int rnd = 0;
		for ( int r=0; r<1000 && g_canRun; ++r)
		{
			rnd += std::rand()*20/3 ;
			rnd += int(ceil(sqrt(double(rand()))));
			for (int r2= 0; r2<200 && g_canRun; ++r2)
				rnd += int(pow( fmod(double(rand()+rand()+2), 100), 100));

			rnd += thId;
		}
		rnd += (thId%7);
		
		cout_lock();
		std::cout << "thread #" << thId << " processando " << n <<  " - " << rnd << '\n'; 
		cout_unlock();
		
		//sleep(1); // dorme 1 segundo
		//msleep(1); // dorme 1 milisegundo
		//usleep(1); // dorme 1 microsegundo
		// nanosleep(1); // dorme 1 nanosegundo - n�o existe no Windows

		//		yieldCurrentThread(); // encerra seu tempo de processamento atual
																// passando a bola para o pr�ximo thread
	} 
	cout_lock();
	std::cout << "thread #"	<<thId << "finalizando\n";
	cout_unlock();

	finish_lock();
	++finish_;
	if ( finish_ >= MaxThreads)
	{
		cout_lock();
		std::cout << "tempo decorrido: " << time.elapsed() ;
		cout_unlock();
	}
	finish_unlock();
		
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	 time.start();
	 
	 Thread threads [MaxThreads];
	 
	 std::cout << "vai executar threads. tecle <enter> p/sair\n";
	 for (int n=0; n<MaxThreads; ++n)
		 threads[n].start();
	 
	 // o main thread continua rodando normalmente com a sua "interface de usu�rio"
	 std::cin.get();
	 
	 g_canRun = false;  // sinaliza para que os threads encerrem
	 int timedouts = 0;
	 for (int n=0; n<MaxThreads; ++n)  // aguarda que realmente tenham encerrado
	 { 
		 if ( !threads[n].wait(1000) ) // espera ate o m�ximo de 1 segundo
			 ++timedouts;
	 }
	 std::cout << "\nthreads que n�o encerram a tempo(timeout) = "
														<< timedouts <<  '\n';

		std::cin.get();
		return 0;
}
