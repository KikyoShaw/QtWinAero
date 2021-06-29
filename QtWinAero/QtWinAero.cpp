#include "QtWinAero.h"
#include <QPainter>
#include <QMouseEvent>
#include <windows.h>
#include <WinUser.h>
#include "WindowCompositionAttribute.h"
#include <QPainter>
#include <QMouseEvent>
#include <QBitmap>
#include <QColorDialog>

#pragma comment (lib,"user32.lib")

#pragma comment (lib,"Dwmapi.lib")

#include <QtWin>

QtWinAero::QtWinAero(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

	m_bgColor = QColor(255, 180, 180, 50);
	m_alph = 50;

	m_mainframe = new mainframe();
	connect(m_mainframe, SIGNAL(chBgColor(QColor)), this, SLOT(sltChBgColor(QColor)));
	connect(m_mainframe, SIGNAL(setAlph(int)), this, SLOT(sltSetAlph(int)));
	//setStyleSheet("border-radius:10px;");

	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(/*Qt::Dialog|*/Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

	////����DWM���״̬(win7�µ�ë����)
	//if (QtWin::isCompositionEnabled()) {
	//	//����Ч��
	//	QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
	//	//��͸������
	//	setAttribute(Qt::WA_TranslucentBackground, true);
	//	//�����ޱ���
	//	setAttribute(Qt::WA_NoSystemBackground, false);
	//	setStyleSheet("background: transparent;");
	//}
	//else
	//{
	//	QtWin::resetExtendedFrame(this);
	//	ui.frame->setAttribute(Qt::WA_TranslucentBackground, false);
	//	setStyleSheet(QString("background: %1;").arg(QtWin::realColorizationColor().name()));
	//}

	//win10�µ�ë����
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

	BOOL bEnable = false;
	::DwmIsCompositionEnabled(&bEnable);
	if (bEnable)
	{
		DWMNCRENDERINGPOLICY ncrp = DWMNCRP_ENABLED;
		::DwmSetWindowAttribute((HWND)winId(), DWMWA_NCRENDERING_POLICY, &ncrp, sizeof(ncrp));
		MARGINS margins = { -1 };
		::DwmExtendFrameIntoClientArea((HWND)winId(), &margins);
	}
	
	connect(ui.pushButton_close, &QPushButton::clicked, this, [=]() {
		qApp->exit();
	});

	connect(ui.pushButton, SIGNAL(clicked(bool)), this, SLOT(sltMainframe()));
}

QtWinAero::~QtWinAero()
{
}

void QtWinAero::sltMainframe()
{
	m_mainframe->show();
}

void QtWinAero::sltChBgColor(QColor color)
{
	m_bgColor = color;
	update();
}

void QtWinAero::sltSetAlph(int alph)
{
	m_alph = alph;
	m_bgColor.setAlpha(m_alph);
	update();
}

void QtWinAero::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(m_bgColor);
	painter.drawRoundedRect(rect(), 20, 20);
}

void QtWinAero::mouseMoveEvent(QMouseEvent * event)
{
	//�ж�����Ƿ񱻰��£�ֻ����������ˣ��䷵��ֵ����1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void QtWinAero::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bMove = true;
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void QtWinAero::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}
