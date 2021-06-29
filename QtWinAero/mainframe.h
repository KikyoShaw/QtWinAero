#pragma once

#include <QDialog>
#include <QSlider>
#include "ui_mainframe.h"

class mainframe : public QDialog
{
	Q_OBJECT

public:
	mainframe(QWidget *parent = 0);
	~mainframe();

public slots:
	void slotvalueChanged(int);

signals:
	void chBgColor(QColor);
	void setAlph(int);

private:
	virtual void paintEvent(QPaintEvent *);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
	Ui::mainframe ui;
	QPoint m_startPos;
	volatile bool m_bMove = false;
	QColor bgColor;
	QSlider *pslider;
	QVector<QLabel*> m_Vlabel;
	int m_alph;
};