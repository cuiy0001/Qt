#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>
#include <QtGui>


class widget : public QWidget
{
  Q_OBJECT

public:
	widget(QWidget *parent = 0, Qt::WFlags flags = 0);
	~widget();
protected:
	void paintEvent(QPaintEvent* event);
	void	wheelEvent ( QWheelEvent * event );
	void mousePressEvent ( QMouseEvent * event );
	void contextMenuEvent(QContextMenuEvent* event);

	QAction* print;
	QAction* quit;
private slots:
	void print_report();
private:
	void drawChart(QPainter&,int,int);
	void barcode(char*,QPainter&);
	void paint_(QPaintDevice*);
	QPrinter* printer;
	int m_width;
	int m_height;
	int scroll;
	int vtran;
	int chartFont1;
	int chartFont2;
};

#endif // WIDGET_H
