#ifndef WRITEFILE_H
#define WRITEFILE_H

#include <QObject>
#include <QVector>
#include <QFile>
#include <QPair>

class WriteFile : public QObject
{
    Q_OBJECT
public:
    explicit WriteFile(QObject *parent = 0);
	void generateData(int rowCount);
	bool writeTxt();

signals:

public slots:

	private:
		bool writeLoop(int index, int begin, int end);

		int m_rowCount;

		// nosso "banco de dados" ser� um vetor:
		QVector < QPair < QString, double > > m_data;

		// o "banco de dados" ser� gravado em um arquivo texto
		// nome com 40 posi��es, valor com 12 posi��es,
		// cada linha terminada com "\r\n"
		QVector <QFile*> m_txtFiles;
		// para gravar, usaremos um thread por core, cada qual
		// gravando em um lugar do arquivo
		// se o vetor tiver 1000 linhas, por exemplo e eu tenho 2 cores
		// o primeiro thread grava de 0 a 499 e o segundo de 500 a 999

		// os threads ser�o acionados pela fun��o QtConcurrent::run(...);
};

#endif // WRITEFILE_H
