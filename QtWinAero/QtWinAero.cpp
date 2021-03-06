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

#include <tchar.h>
#include "dwmapi.h"

#pragma comment (lib,"user32.lib")

#pragma comment (lib,"Dwmapi.lib")

#include <QtWin>
#include "DarkenEffect.h"

static bool IsWin10System()
{
	//string与CString转换
	//string sPath = (LPCSTR)(CStringA)(strPath);
	std::string vname;
	// 先判断是否为win8.1或win10
	typedef void(__stdcall*NTPROC)(DWORD*, DWORD*, DWORD*);
	HINSTANCE hinst = LoadLibrary(_T("ntdll.dll"));
	DWORD dwMajor, dwMinor, dwBuildNumber;
	NTPROC proc = (NTPROC)GetProcAddress(hinst, "RtlGetNtVersionNumbers");
	proc(&dwMajor, &dwMinor, &dwBuildNumber);

	if (dwMajor == 10 && dwMinor == 0)
	{
		return true;
	}

	return false;
}

QtWinAero::QtWinAero(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

	//m_bgColor = QColor(255, 180, 180, 200);
	//m_alph = 50;

	m_mainframe = new mainframe();
	connect(m_mainframe, SIGNAL(chBgColor(QColor)), this, SLOT(sltChBgColor(QColor)));
	connect(m_mainframe, SIGNAL(setAlph(int)), this, SLOT(sltSetAlph(int)));
	//setStyleSheet("border-radius:10px;");

	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(/*Qt::Dialog|*/ Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

	//DarkenEffect *bodyShadow = new DarkenEffect();
	//bodyShadow->setBlurRadius(200);
	//bodyShadow->setDistance(6.0);
	//bodyShadow->setColor(QColor(21, 32, 52, 100));
	//ui.frame->setGraphicsEffect(bodyShadow);

	//setGraphicsEffect(new DarkenEffect);

	//返回DWM组合状态(win7下的毛玻璃)
	//https://doc.qt.io/qt-5/qtwinextras-overview.html
	if (QtWin::isCompositionEnabled()) {
		QRegion reg;
		int border = 5;
		QRect rect = QRect();
		if (rect.isValid()) {
			reg += rect;
		}
		else {
			rect = this->rect();
			reg += rect;
		}
		//圆角无法模糊，需要透明处理
		auto right = rect.width();
		auto bottom = rect.height();
		reg -= QRect(0, 0, border, border);
		reg -= QRect(right - border, 0, border, border);
		reg -= QRect(0, bottom - border, border, border);
		reg -= QRect(right - border, bottom - border, border, border);
		QtWin::enableBlurBehindWindow(this, reg);
		////去边框
		//QtWin::enableBlurBehindWindow(this);
		//玻璃效果
		QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
		////半透明背景
		//setAttribute(Qt::WA_TranslucentBackground, true);
		////禁用无背景
		//setAttribute(Qt::WA_NoSystemBackground, false);
		//setStyleSheet("background: transparent;");
	}
	else
	{
		QtWin::resetExtendedFrame(this);
		ui.frame->setAttribute(Qt::WA_TranslucentBackground, false);
		setStyleSheet(QString("background: %1;").arg(QtWin::realColorizationColor().name()));
	}

	////判断是不是win10
	////if (IsWin10System()) {
	//if(QSysInfo::windowsVersion() == QSysInfo::WV_10_0){
	//	//win10下的毛玻璃
	//	HWND hWnd = HWND(winId());
	//	HMODULE hUser = GetModuleHandle(L"user32.dll");
	//	if (hUser)
	//	{
	//		pfnSetWindowCompositionAttribute setWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
	//		if (setWindowCompositionAttribute)
	//		{
	//			ACCENT_POLICY accent = { ACCENT_ENABLE_BLURBEHIND, 0, 0, 0 };
	//			WINDOWCOMPOSITIONATTRIBDATA data;
	//			data.Attrib = WCA_ACCENT_POLICY;
	//			data.pvData = &accent;
	//			data.cbData = sizeof(accent);
	//			setWindowCompositionAttribute(hWnd, &data);
	//		}
	//	}
	//}
	//else if(QSysInfo::windowsVersion() == QSysInfo::WV_6_1) {
	//	//win7改进版 毛玻璃特效
	//	//DWM_BLURBEHIND bb = { 0 };
	//	//bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
	//	//bb.fEnable = true;
	//	//bb.hRgnBlur = NULL;
	//	//DwmEnableBlurBehindWindow((HWND)winId(), &bb);

	//	//MARGINS margins = { 50, 50, 50, 50 };
	//	//DwmExtendFrameIntoClientArea((HWND)winId(), &margins);
	//}

	//该方法在win7下有瑕疵，点击出其他窗口后，该窗口的毛玻璃特效会失效
	//BOOL bEnable = false;
	//::DwmIsCompositionEnabled(&bEnable);
	//if (bEnable)
	//{
	//	DWMNCRENDERINGPOLICY ncrp = DWMNCRP_ENABLED;
	//	::DwmSetWindowAttribute((HWND)winId(), DWMWA_NCRENDERING_POLICY, &ncrp, sizeof(ncrp));
	//	MARGINS margins = { -1 };
	//	::DwmExtendFrameIntoClientArea((HWND)winId(), &margins);
	//}

	//模糊部分地方
	//MARGINS margins = { 50, 50, 50, 50 };
	//DwmExtendFrameIntoClientArea(hWnd, &margins);
	
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
	//QPainter painter(this);
	//painter.setRenderHint(QPainter::Antialiasing);
	//painter.setPen(Qt::NoPen);
	//painter.setBrush(m_bgColor);
	//painter.drawRoundedRect(rect(), 20, 20);
}

void QtWinAero::mouseMoveEvent(QMouseEvent * event)
{
	//判断左键是否被按下，只有左键按下了，其返回值就是1(true)
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
