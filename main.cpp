#include "widget.h"
#include "gridreport.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
	widget *w=new widget;
	
	QTextCodec::setCodecForTr( QTextCodec::codecForName("GBK") );
	w->resize(820,850);
	w->show();
	return a.exec();
}
