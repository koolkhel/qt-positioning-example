#include <QGeoPositionInfoSource>
#include <QCoreApplication>
#include <QObject>
#include <QtDebug>
#include <QStringList>
#include <QTimer>

void printAvailablePositionSources() {
	qDebug() << "QGeoPositionInfoSource";
	QStringList sources = QGeoPositionInfoSource::availableSources();
	foreach (QString source, sources) {
		qDebug() << "gps source: " << source;
	}
}

int main(int argc, char **argv) {
	QCoreApplication app(argc, argv);

	printAvailablePositionSources();

	auto positionSource = QGeoPositionInfoSource::createDefaultSource(&app);
	QObject::connect(positionSource, &QGeoPositionInfoSource::positionUpdated, 
			[](const QGeoPositionInfo &pos) {
				qDebug() << "positionUpdated:" << pos;
			});
	
	auto timer = new QTimer();
	timer->setInterval(1000);
	QObject::connect(timer, &QTimer::timeout, [positionSource]() {
				qDebug() << "last known position:" << positionSource->lastKnownPosition();
			});
	timer->setSingleShot(false);
	timer->start();
	
	positionSource->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
        positionSource->setUpdateInterval(5000);
        positionSource->startUpdates();

	return app.exec();
}
