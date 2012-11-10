
#include <QImage>
 #include <QList>
 #include <QThread>
 #include <QDebug>
 #include <QApplication>
 #include <qtconcurrentmap.h>

#include <iostream>
#include <iomanip>

 #ifndef QT_NO_CONCURRENT

QMutex mut_cout;

struct ImageTest
{
	int id;
	QImage img;
	ImageTest(){}
	ImageTest(int id, const QImage & img)
	{
		this->id = id;
		this->img = img;
	}
};

// fun��o  que ser� usada por "blockingMapped"
// para mudar dimens�es de uma imagem:
ImageTest scale(const ImageTest & imgTest);

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	std::cout << "main iniciando no thread: "
			<< (unsigned int)(QThread::currentThreadId()) << "\n\n";

	const int imageCount = 20;

	// Create a list containing imageCount images.
	QList<ImageTest> images;
	for (int i = 0; i < imageCount; ++i)
	{
		// acrescenta na lista um objeto "ImageTest"
		// cuja construtora recebe um n�mero("id") e um objeto "QImage":
		images.append( ImageTest( i+1, 
				QImage(1600, 1200,
					   QImage::Format_ARGB32_Premultiplied) ) );
	}

	// Use QtConcurrentBlocking::mapped to apply the scale function to all the
	// images in the list.
	QList<ImageTest> thumbnails = 
						QtConcurrent::blockingMapped(images, scale);

	// agora salva o resultado ("thumbnails") gravando 
	// cada uma das imagens da lista "thumbnails"
	// em um arquivo:
	foreach ( const ImageTest & imgTest, thumbnails )
	{
		imgTest.img.save(	
			QString("file_%1.png").arg( imgTest.id, 2, 10, QChar('0') ) , // nome do arquivo
								"PNG"  // tipo da imagem
				);
	}
// obs:  alternativas quando "QString::arg" recebe um n�mero:
// arg(num); // apenas o n�mero
// ou, usando largura e caracter de preenchimento:
// arg(num, width, base, fill ); 
// no exemplo acima: num=imgTest.id,  width=2, base=10, fill='0'
	std::cout << "tecle <enter> para sair" << std::endl;
	std::cin.get();
	return 0;
}

int g_red = 8;
int g_green = 4;
int g_blue = 0;
ImageTest scale(const ImageTest &imgTest)
{
	mut_cout.lock();
		
		// imprime mensagem informando qual imagem est� sendo modificada
		// e qual o thread que est� fazendo isso para essa imagem:
		std::cout.fill('0');
		std::cout << "Scaling image [#" 
			<< std::setw(2) << imgTest.id 
			<<  "] in thread: " 
			<< (unsigned int)(QThread::currentThreadId()) << std::endl;
	
		int red = g_red+=24;
		int green = g_green+=8;
		int blue = g_blue += 2;

	mut_cout.unlock();

	ImageTest result;
	// altera dimens�es do objeto "QImage" em uma c�pia
	result.img = imgTest.img.scaled(QSize(100, 100), 
					Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	result.id = imgTest.id;

	// altera cores do objeto "QImage":
	int w = result.img.width();
	int h = result.img.height();
	for ( int nw=0; nw< w; ++nw)
	{
		++red; ++green; ++blue;
		for ( int nh=0; nh< h; ++nh)
		{
			result.img.setPixel(nw, nh,
							qRgb( red&255, green&255, blue&255 ) );
		}	
	}
	return result;
}

 #else

int main()
{
	std::cout << "Qt Concurrent is not yet supported on this platform" << std::endl;
	return 0;
}

 #endif
