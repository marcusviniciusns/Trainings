
#ifndef MainWin_H
#define MainWin_H

#include <QWidget>
#include <QSlider>
class QLabel;

class MainWin : public QWidget
{
	Q_OBJECT

	QSlider * m_slider;
	QWidget * m_thermometer;
	QLabel * m_redLabel;
	QLabel * m_whiteLabel;
	int m_thermometerMargin;
	public:
		MainWin(QWidget * pParent = NULL );

	private slots:
		void updateThermometer ( int sliderValue );

	protected:
                void resizeEvent(QResizeEvent *)
		{
			// se janela for redimensionada, o "term�metro" tem que ser atualizado,
			// pois n�o estamos usando "painting" (paintEvent)
			// e sim redimensionamento de widgets:
			updateThermometer( m_slider->value() );
		}

};

#endif // ( MY_CLASS_H )


