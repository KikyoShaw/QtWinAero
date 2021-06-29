#pragma once

#include <QtWidgets/QDialog>
#include "mainframe.h"
#include "ui_QtWinAero.h"

class QtWinAero : public QDialog
{
    Q_OBJECT

public:
    QtWinAero(QWidget *parent = Q_NULLPTR);
	~QtWinAero();

public slots:
	void sltMainframe();
	void sltChBgColor(QColor);
	void sltSetAlph(int);

private:
	virtual void paintEvent(QPaintEvent *);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::QtWinAeroClass ui;
	//�����ƶ�����ֵ
	QPoint m_point;
	volatile bool m_bMove = false;
	//��ɫ��
	mainframe *m_mainframe;
	//��ɫ
	QColor m_bgColor;
	//͸����
	int m_alph;
};
