#include <QList>
#include <QMap>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QTime>
#include <QApplication>
#include <QMutex>
#include <qtconcurrentmap.h>
#include <iostream>

QMutex mut_cout;

using namespace QtConcurrent;

// Teremos uma lista de vetores de inteiros.
// A fun��o "countNumbers" ira totalizar os n�meros existentes em cada vetor,
// isto �, em cada elemento da lista.
// Ela retorna um "map" que tem como chave
// cada n�mero diferente localizado no vetor
// e, como valor associado � chave, a quantidade de ocorr�ncias desse n�mero.
// Esse retorno sera passado para a fun��o "reduce" (abaixo)
// a qual dever� criar um total �nico de toda a lista,
// ou seja, uma �nica ocorr�ncia para cada n�mero diferente,
// com o respectivo total de ocorr�ncias.
// A fun��o "countNumbers" pode ser chamada paralelamente
// por diversos threads.
// Por isso, deve ser "thread-safe".
QMap<int, int> countNumbers( const QVector < int > & listVecInt);

// A fun��o "reduce" totaliza os resultados do "map"
// que ela receber� como segundo argumento ("numbersInOneVector")
// - e que ser� o resultado de cada chamada � fun��o "countNumbers"-
// de forma a criar um totalizador �nico para
// a quantidade de n�meros encontrados em todos os vetores processados.
// Esse resultado � gravado em outro "map", que � o seu primeiro argumento
// ("result_NumbersInAllVectors), e ser� o resultado final do processamento.
// A fun��o "reduce" � executada em um �nico thread a cada vez.
void reduce(QMap<int, int>
			&result_NumbersInAllVectors,
			const QMap<int, int> &numbersInOneVector);

 int main(int argc, char** argv)
 {
	QApplication app(argc, argv);
	std::cout << "main iniciando no thread: "
			<< (unsigned int)(QThread::currentThreadId()) << "\n\n";

	QList < QVector< int > > listVecInt;
	// Alimentar a lista de vetores.
	// Cada vetor conter� os n�meros 1, 2 e 3 - duplicados (duas ocorr�ncias de cada um).
	// A lista conter� 5 vetores.
	// Logo, cada n�mero dever� ter um total final de 10 ocorr�ncias:
	for ( int nlist=0; nlist<5; ++nlist )
	{
		QVector< int > vecInt;
		for ( int nvec = 0; nvec < 3; ++nvec  )
		{
			vecInt.append( nvec+1 );
			vecInt.append( nvec+1 );
		}
		listVecInt.append( vecInt );
	}

	// - conta a quantidade de n�meros em cada vetor da lista
	//     usando diversos threads:
	std::cout << "\n* main: processando com multiplos threads:\n" << std::endl;
	QMap<int, int> total = mappedReduced(listVecInt,
										 countNumbers, reduce);

	std::cout << "\n ========= resultado da totalizacao final\n";
	QMap<int, int>::iterator it;
	for ( it=total.begin() ; it!= total.end() ; ++it )
	{
		std::cout << it.key() << " : " << it.value() << '\n';
	}

	std::cout << "tecle <enter> para sair" << std::endl;
	std::cin.get();
	return 0;
 }
 
// "countNumbers" : conta os n�meros existentes em um �nico vetor da lista,
// criando um mapa de totais para os n�meros diferentes existentes nesse vetor.
// Chamada por diversos threads, cada qual tratando um vetor.
// Deve ser "thread-safe".
QMap<int, int> countNumbers(const QVector<int> & vecInt)
{
	mut_cout.lock();
	std::cout << "begin countNumbers() - thread " << (unsigned int)(QThread::currentThreadId()) << std::endl;
	mut_cout.unlock();


	QMap<int, int> numbersInThisVector;
	int size = vecInt.size();
	for ( int n =0; n<size; ++n)
	{
		const int & number = vecInt[n];
		numbersInThisVector[ number ] += 1; // busca "number" no map; se j� estiver l� apenas soma mais 1 no valor associado
											// do contr�rio, insere "number" e inicia o valor associado com 0+1.
	}
	mut_cout.lock();
	std::cout << "end countNumbers() - thread " << (unsigned int)(QThread::currentThreadId()) << std::endl;
	mut_cout.unlock();

	return numbersInThisVector;

}

 // A fun��o "reduce" totaliza os resultados do "map"
// que ela receber� como segundo argumento ("numbersInOneVector")
// - e que ser� o resultado de cada chamada � fun��o "countNumbers"-
// de forma a criar um totalizador �nico para
// a quantidade de n�meros encontrados em todos os vetores processados.
// Esse resultado � gravado em outro "map", que � o seu primeiro argumento
// ("result_NumbersInAllVectors), e ser� o resultado final do processamento.
// A fun��o "reduce" � executada em um �nico thread a cada vez.
void reduce(QMap<int, int> &result_NumbersInAllVectors,
			const QMap<int, int> & numbersInOneVector)
{
	mut_cout.lock();
	std::cout << "\tbegin reduce() -resultSize: " << result_NumbersInAllVectors.size() << " -inputSize: " << numbersInOneVector.size()
								<< " - thread " << (unsigned int)(QThread::currentThreadId()) << std::endl;
	mut_cout.unlock();

	QMapIterator<int, int> i (numbersInOneVector);
	while (i.hasNext()) 
	{
		i.next();
		result_NumbersInAllVectors[i.key()] += i.value();
	}

	mut_cout.lock();
	std::cout    << "\tend reduce() -resultSize: " << result_NumbersInAllVectors.size()
						<< " -inputSize: " << numbersInOneVector.size()
						<< " - thread " << (unsigned int)(QThread::currentThreadId()) << std::endl;
	mut_cout.unlock();
}

/*
 // ==== Resultado de uma execu��o deste programa em uma m�quina com 2 cores.
 // OBS: em cada execu��o, a ordem com que os n�meros ser�o tratados poder� ser diferente,
 //  bem como o id do thread em que s�o tratados.

  main iniciando no thread: 528


* main: processando com multiplos threads:

begin countNumbers() - thread 524
end countNumbers() - thread 524
		begin reduce() -resultSize: 0 -inputSize: 3 - thread 524
		end reduce() -resultSize: 3 -inputSize: 3 - thread 524
begin countNumbers() - thread 524
end countNumbers() - thread 524
		begin reduce() -resultSize: 3 -inputSize: 3 - thread 524
begin countNumbers() - thread 2788
		end reduce() -resultSize: 3 -inputSize: 3 - thread 524
end countNumbers() - thread 2788
begin countNumbers() - thread 524
		begin reduce() -resultSize: 3 -inputSize: 3 - thread 2788
end countNumbers() - thread 524
		end reduce() -resultSize: 3 -inputSize: 3 - thread 2788
begin countNumbers() - thread 524
		begin reduce() -resultSize: 3 -inputSize: 3 - thread 2788
end countNumbers() - thread 524
		end reduce() -resultSize: 3 -inputSize: 3 - thread 2788
		begin reduce() -resultSize: 3 -inputSize: 3 - thread 2788
		end reduce() -resultSize: 3 -inputSize: 3 - thread 2788

 ========= resultado da totalizacao final
1 : 10
2 : 10
3 : 10
tecle <enter> para sair

*/
