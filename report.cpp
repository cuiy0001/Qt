#include "widget.h"
//Sample Data
double sample[31]={
0.3,0.31,0.33,0.321,0.345,0.342,0.35,0.354,0.36,0.375,0.38,0.4,0.42,0.44,0.425,0.43,0.46,0.477,
0.46,0.44,0.426,0.41,0.4,0.39,0.385,0.371,0.364,0.358,0.342,0.338,0.33

  };
char g_CodeIndex[37][10]={
"bwbWBwBwb","BwbWbwbwB","bwBWbwbwB",
"BwBWbwbwb","bwbWBwbwB","BwbWBwbwb",
"bwBWBwbwb","bwbWbwBwB","BwbWbwBwb",
"bwBWbwBwb","bWbwBwBwb","BwbwbWbwB",
"bwBwbWbwB","BwBwbWbwb","bwbwBWbwB",
"BwbwBWbwb","bwBwBWbwb","bwbwbWBwB",
"BwbwbWBwb","bwBwbWBwb","bwbwBWBwb",
"bwbwBWBwb","bwBwbwbWB","BwBwbwbWb",
"bwbwBwbWB","BwbwBwbWb","bwBwBwbWb",
"bwbwbwBWB","BwbwbwBWb","bwBwbwBWb",
"bwbwBwBWb","BWbwbwbwB","bWBwbwbwB",
"BWBwbwbwb","bWbwBwbwB","BWbwBwbwb",
"bWBwBwbwb"};//0-9 * A--Z

widget::widget(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	// Qt::WindowMinimizeButtonHint Qt::FramelessWindowHint|Qt::CustomizeWindowHint|
	flags =Qt::WindowStaysOnTopHint  ;
	setWindowFlags( flags );
	
	m_width =794;//800
	m_height =1123;//1000
	
	vtran=0;//vertical translation
	int chartFont1=16;
	int chartFont2=12;
	//create actions
	print = new QAction(QString::fromLocal8Bit("打印"), this);
    print->setShortcuts(QKeySequence::Print);
	connect(print,SIGNAL(triggered()),this,SLOT(print_report()));

	quit = new QAction(QString::fromLocal8Bit("关闭"),this);
	connect(quit,SIGNAL(triggered()),this,SLOT(close()));

	printer=new QPrinter;
}


void widget::barcode(char* code,QPainter& painter)
{	
	//positions
	int x=0; int y=0;
	int wide=5;int narrow=2;int height =50;
	int index;char symbol;
	painter.setPen(QPen(Qt::transparent,0));
	painter.setBrush(QBrush(Qt::black,Qt::SolidPattern));
	painter.setRenderHint(QPainter::Antialiasing,false);
	for(int i=0;i<strlen(code);i++)
		{
			if(isdigit(code[i]))
				index=code[i]-48;
			else if(isupper(code[i]))
				index=code[i]-54;
			else
				index=10;
			for(int j=0;j<10;j++)
				{
					symbol=g_CodeIndex[index][j];
					switch(symbol)
						{
						case 'B':
							painter.drawRect(x,y,wide, height);
							x+=5;
							break;
						case 'b':
							painter.drawRect(x,y,narrow, height);
							x+=2;
							break;
						case 'W':
							x+=5;
							break;
						case 'w':
							x+=2;
							break;
						}
				}
				x+=2;
		}
}
void widget::drawChart(QPainter& painter,int w,int h)
{
	int chartw=510;
	int charth= 250;//width and height of the central chart
	int chartx=70;
	int charty=50;//starting point x and y
	int xstep=17;
	int ystep=charth/10;
	double ymin=0.19;
	double ymax=0.5;
	QPoint point[31];
	painter.setPen(QPen(Qt::black,2,Qt::SolidLine,Qt::RoundCap));
	painter.setBrush(QBrush(Qt::black,Qt::SolidPattern));
	painter.drawRect(0,0,w,h);
	//draw  headline
	painter.setPen(QPen(Qt::white,3,Qt::SolidLine,Qt::RoundCap));
	painter.setFont(QFont(QString::fromLocal8Bit("宋体"),16,75,false));
	painter.drawText(w/2-64,30,QString::fromLocal8Bit("总氮月检测报表"));
	painter.setFont(QFont(QString::fromLocal8Bit("宋体"),12,75,false));
	painter.drawText(w/2-64,h,QString::fromLocal8Bit("每日6点测量曲线图"));
	//
	
	//create gradient for the central chart
	QLinearGradient gra(chartx,charty,chartx+chartw,charty);
	gra.setColorAt(0.0,QColor(1,25,187));
	gra.setColorAt(1.0,QColor(0,255,255));//blue and cyan
	painter.setPen(QPen(Qt::transparent));
	painter.setBrush(QBrush(gra));
	painter.drawRect(chartx,charty,chartw,charth);
	
	
	//draw your curve

	//recalculate coordinates
 painter.setPen(QPen(QColor(79, 106, 25), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
                    
 painter.setBrush(QColor(122, 163, 39));
	for (int i=0;i<30;i++)
		{
			point[i].setX(chartx+xstep*i);
			point[i].setY(charty+charth-(int)((sample[i]-ymin)*charth/ymax));
			//painter.setPen(QPen(Qt::red,10));
			//painter.drawPoint(point[i]);
			painter.drawRect(point[i].x(),point[i].y(),8,(int)((sample[i]-ymin)*charth/ymax));
		}
	
	/******************************/
	//x-axis marker and number
	painter.setPen(QPen(Qt::black,1));
	for(int i=0;i<30;i++)
		{
			int x=chartx+xstep*i;
			int y=charty+charth;
			painter.drawLine(x,y,x,y-5);
		}
	painter.setFont(QFont(QString::fromLocal8Bit("宋体"),12,-1,false));
	painter.setPen(QPen(Qt::white,1));
	for(int i=0;i<15;i++)
		{
			int x=chartx+2*i*xstep;
			int y=charty+charth+20;
			painter.drawText(x,y,QString::number(2*i+1));
		}
	//y-axis number
	for(int i=0;i<=10;i++)
		{
			double step= (ymax-ymin)/10;
			int x= chartx-45;
			int y=charty+i*ystep;
			painter.drawText(x,y,QString::number(ymax-step*i,'g',3));
		}
}

void widget::paintEvent(QPaintEvent*)
{
	paint_(this);
	
}







void	widget::wheelEvent ( QWheelEvent * event )
{
	int numDegrees = event->delta() / 8;
    scroll = numDegrees / 15;
	vtran+=(-20*scroll);
	if(vtran<0)
		vtran=0;
	if(vtran>(m_height-850))
		vtran=m_height-850;
	event->accept();
	update();
}
void widget::mousePressEvent(QMouseEvent* event)
	{
	
		
	}
void widget::contextMenuEvent(QContextMenuEvent* event)
	{
		QMenu menu(this);
		menu.addAction(print);
		menu.addAction(quit);
		menu.exec(event->globalPos());
	}
void widget::print_report()
	{
		QPrintDialog dlg(printer,this);
		if(dlg.exec())
			{
				paint_(printer);
			}
	}
void widget::paint_(QPaintDevice* dc)
{
	QPainter painter(dc);
	int split1=200;
	int split2=50;
	int split3=400;
	int fontsize1=15;
	int fontsize2=12;
	int indent =40;
	int space = 35;
	int upper=70;
	int bottom=120;
	////////////////////////////////
	
	painter.setViewport(width()/2-m_width/2,0,m_width,m_height);
	painter.setWindow(0,0,m_width,m_height);
	QTransform move;
	move.translate(0,-vtran);

	painter.setWorldTransform(move);
	//white background
	painter.setPen(QPen(Qt::transparent,2,Qt::SolidLine,Qt::RoundCap));
	painter.setBrush(QBrush(Qt::white,Qt::SolidPattern));
	painter.drawRect(0,0,m_width,m_height);
	//draw barcode
	painter.save();
	painter.translate(indent+split1+50,upper+25);
	barcode("*LX65496148571*",painter);
	painter.restore();
	//draw Chart
	painter.setRenderHint(QPainter::Antialiasing,true);
	painter.setPen(QPen(Qt::black,2,Qt::SolidLine,Qt::RoundCap));
	painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));
	painter.save();
	painter.translate(indent+split2+30,upper+3*space+30);
	//painter.drawRect(0,0,600,250);
	drawChart(painter,600,350);
	painter.restore();
	//begin drawing
	painter.setRenderHint(QPainter::Antialiasing,false);
	painter.setPen(QPen(Qt::black,3,Qt::SolidLine,Qt::RoundCap));

	painter.drawText(30,50,QString(tr("智能在线检测")));
	painter.drawText(680,50,QString(tr("编号:Lx654961")));
	painter.setFont(QFont(QString::fromLocal8Bit("楷体_GB2312"),25,-1,false));
	painter.drawText(160,50,QString(tr("海安站 总氮水检测月报告")));
	//框架线
	//draw frames now
	painter.setPen(QPen(Qt::black,3,Qt::SolidLine,Qt::SquareCap));
	
	painter.drawLine(indent,70,m_width-indent,70);
	painter.drawLine(indent,70,indent,m_height-bottom);
	painter.drawLine(indent,m_height-bottom,m_width-indent,m_height-bottom);
	painter.drawLine(m_width-indent,70,m_width-indent,m_height-bottom);
	//draw lines inside the frame
	
	painter.setFont(QFont(QString::fromLocal8Bit("宋体"),fontsize1,-1,false));
	painter.setPen(QPen(Qt::black,2,Qt::SolidLine,Qt::RoundCap));
	
	painter.drawLine(indent,upper+3*space,m_width-indent,upper+3*space);
	painter.drawLine(indent+split1,upper,indent+split1,upper+3*space);
	painter.drawLine(indent,upper+space,indent+split1,upper+space);
	painter.drawLine(indent,upper+2*space,indent+split1,upper+2*space);
	painter.drawLine(indent+split1/2,upper+space,indent+split1/2,upper+space*3);

	painter.drawLine(indent+split2,upper+3*space,indent+split2,m_height-bottom);
	painter.drawLine(indent,upper+3*space+split3,m_width-indent,upper+3*space+split3);
	//左上角
	painter.drawText(indent+21,upper+space -11,QString(tr("总氮检测报告代码")));
	painter.drawText(indent+15,upper+2*space -11,QString(tr("报告日期")));
	painter.drawText(indent+15,upper+3*space -11,QString(tr("检测日期")));
	painter.setFont(QFont(QString::fromLocal8Bit("宋体"),fontsize2,-1,false));
	painter.drawText(indent+split1/2+15,upper+2*space -11,QString(tr("2013.03.01")));
	painter.drawText(indent+split1/2+15,upper+3*space -11,QString(tr("2013.03.01")));
	//竖排标题
	painter.setFont(QFont(QString::fromLocal8Bit("黑体"),fontsize2+3,-1,false));
	QString text1(tr("检测项目名称"));
	int step= 0;
	for(int i=0;i<text1.size();i++)
	{
		
		painter.drawText(indent +18,upper+3*space+65+step,QString(text1[i]));
		step+=35;
	}
	step =0;
	QString text2(tr("检测结论"));
	for(int i=0;i<text2.size();i++)
	{
		painter.drawText(indent +18,upper+3*space+split3+95+step,QString(text2[i]));
		step+=35;
	}
	//结论 内容
	painter.setFont(QFont(QString::fromLocal8Bit("宋体"),fontsize1,-1,false));
	painter.drawText(indent+split2+20,upper+3*space+split3+30,QString::fromLocal8Bit("结论:"));
	painter.drawText(indent+split2+45,upper+3*space+split3+75,QString::fromLocal8Bit("本次检测实时水体每升平均参数为 0.201  毫克。"));
	painter.drawText(indent+split2+45,upper+3*space+split3+130,QString::fromLocal8Bit("根据国家标准GB 3838—2002 地表水环境质量标准，"));
	painter.drawText(indent+split2+45,upper+3*space+split3+185,QString::fromLocal8Bit("符合总氮指标 I 类水的要求。"));
	painter.drawText(indent+split2+480,upper+3*space+split3+250,QString::fromLocal8Bit("南通市环保局"));
	

	//末行
	painter.setFont(QFont(QString::fromLocal8Bit("黑体"),10,-1,false));
	painter.drawText(indent,m_height-bottom/2,QString::fromLocal8Bit("附: 本次检测对本次样品负责"));
	painter.drawText(indent+600,m_height-bottom/2,QString::fromLocal8Bit("操作员:▁▁▁▁▁▁"));
}
widget::~widget()
{
}

