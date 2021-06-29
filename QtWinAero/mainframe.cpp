#include "mainframe.h"
#include <windows.h>
#include <WinUser.h>
#include "WindowCompositionAttribute.h"
#include <QMouseEvent>
#include <QPainter>
#include <QLayoutItem>
mainframe::mainframe(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	pslider = new QSlider(this);
	ui.gridLayout->addWidget(pslider,1,5,4,1,Qt::AlignHCenter);
	connect(pslider, SIGNAL(valueChanged(int)), this, SLOT(slotvalueChanged(int)));
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

	for (int  i = 0; i < ui.gridLayout->count(); i++)
	{
		QLabel *plabel = static_cast<QLabel *>(ui.gridLayout->itemAt(i)->widget());
		m_Vlabel.append(plabel);
	}

	bgColor = QColor(255, 180, 180, 50);
	m_alph = 50;

	HWND hWnd = HWND(winId());
	HMODULE hUser = GetModuleHandle(L"user32.dll");
	if (hUser)
	{
		pfnSetWindowCompositionAttribute setWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
		if (setWindowCompositionAttribute)
		{
			ACCENT_POLICY accent = { ACCENT_ENABLE_BLURBEHIND, 0, 0, 0 };
			WINDOWCOMPOSITIONATTRIBDATA data;
			data.Attrib = WCA_ACCENT_POLICY;
			data.pvData = &accent;
			data.cbData = sizeof(accent);
			setWindowCompositionAttribute(hWnd, &data);
		}
	}

}

mainframe::~mainframe()
{

}
void mainframe::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(bgColor);
	painter.drawRoundedRect(rect(), 20, 20);
}

void mainframe::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_startPos = event->pos();
		m_bMove = true;
	}
	QPoint point = event->pos();
	for (int i=0;i<m_Vlabel.size();i++)
	{
		if (m_Vlabel.at(i)->geometry().contains(point))
		{
			QPalette pal = m_Vlabel.at(i)->palette();
			QBrush brush = pal.background();
			QColor col = brush.color();
			col.setAlpha(m_alph);
			emit chBgColor(col);
			return;
		}
	}

}

void mainframe::mouseMoveEvent(QMouseEvent *event)
{
	//判断左键是否被按下，只有左键按下了，其返回值就是1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_startPos);
	}
	QWidget::mouseMoveEvent(event);
}

void mainframe::mouseReleaseEvent(QMouseEvent *event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}

void mainframe::slotvalueChanged(int alph)
{
	m_alph = alph;
	bgColor.setAlpha(m_alph);
	emit setAlph(alph);
	update();
}
