#include "DarkenEffect.h"
#include <QPixmap>
#include <QPainter>
#include <QGraphicsRectItem>

DarkenEffect::DarkenEffect(QObject *parent /* = Q_NULLPTR */)
	: QGraphicsEffect(parent),
	_distance(4.0f),
	_blurRadius(10.0f),
	_color(21, 32, 52, 200)
{
}

DarkenEffect::~DarkenEffect()
{
}

QT_BEGIN_NAMESPACE
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

void DarkenEffect::draw(QPainter * painter)
{
	//// ¿¹¾â³Ý + Æ½»¬±ßÔµ´¦Àí
	//painter->setRenderHints(QPainter::Antialiasing, true);
	//painter->setRenderHints(QPainter::SmoothPixmapTransform, true);
	//QPixmap pixmap;
	//QPoint offset;
	//if (sourceIsPixmap())
	//	pixmap = sourcePixmap(Qt::LogicalCoordinates, &offset);
	//else
	//{
	//	pixmap = sourcePixmap(Qt::DeviceCoordinates, &offset);
	//	painter->setWorldTransform(QTransform());
	//}
	//QPainterPath path;
	//painter->setBrush(QColor(21, 32, 52, 100));
	//path.addRoundedRect(pixmap.rect(), 20, 20);
	//painter->setClipPath(path);
	//painter->drawRect(pixmap.rect());
	//painter->setOpacity(1);
	//painter->setClipRegion();
	//painter->drawPixmap(offset, pixmap);

	 // if nothing to show outside the item, just draw source
	if ((blurRadius() + distance()) <= 0) {
		drawSource(painter);
		return;
	}

	PixmapPadMode mode = QGraphicsEffect::PadToEffectiveBoundingRect;
	QPoint offset;
	const QPixmap px = sourcePixmap(Qt::DeviceCoordinates, &offset, mode);

	// return if no source
	if (px.isNull())
		return;

	// save world transform
	QTransform restoreTransform = painter->worldTransform();
	painter->setWorldTransform(QTransform());

	// Calculate size for the background image
	QSize szi(px.size().width() + 2 * distance(), px.size().height() + 2 * distance());

	QImage tmp(szi, QImage::Format_ARGB32_Premultiplied);
	QPixmap scaled = px.scaled(szi);
	tmp.fill(0);
	QPainter tmpPainter(&tmp);
	tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
	tmpPainter.drawPixmap(QPointF(-distance(), -distance()), scaled);
	tmpPainter.end();

	// blur the alpha channel
	QImage blurred(tmp.size(), QImage::Format_ARGB32_Premultiplied);
	blurred.fill(0);
	QPainter blurPainter(&blurred);
	qt_blurImage(&blurPainter, tmp, blurRadius(), false, true);
	blurPainter.end();

	tmp = blurred;

	// blacken the image...
	tmpPainter.begin(&tmp);
	tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	tmpPainter.fillRect(tmp.rect(), color());
	tmpPainter.end();

	// draw the blurred shadow...
	painter->drawImage(offset, tmp);

	// draw the actual pixmap...
	painter->drawPixmap(offset, px, QRectF());

	// restore world transform
	painter->setWorldTransform(restoreTransform);

}

QRectF DarkenEffect::boundingRectFor(const QRectF& rect) const
{
	qreal delta = blurRadius() + distance();
	return rect.united(rect.adjusted(-delta, -delta, delta, delta));
}


