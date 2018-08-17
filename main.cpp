#include <QGeoPositionInfoSource>
#include <QCoreApplication>
#include <QObject>
#include <QtDebug>
#include <QStringList>
#include <QTimer>
#include <QDateTime>

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

	// first way
	auto positionSource = QGeoPositionInfoSource::createDefaultSource(&app);
	QObject::connect(positionSource, &QGeoPositionInfoSource::positionUpdated, 
			[](const QGeoPositionInfo &pos) {
			            static QDateTime lastPositionTime = QDateTime::fromMSecsSinceEpoch(0);

				    // this one seems to be really important,
				    // QNmeaPositionInfoSource doesn't really guarantee time always
				    // increasing, but it could break further layers of an app
				    // so we recheck
				    if (pos.timestamp() > lastPositionTime) {
				    	lastPositionTime = pos.timestamp();
				    	qDebug() << "positionUpdated" << pos;

					// any additional processing
				    }
			});
	
	auto timer = new QTimer();
	timer->setInterval(1000);
	QObject::connect(timer, &QTimer::timeout, [positionSource]() {
				qDebug() << "last known position:" << positionSource->lastKnownPosition();
			});
	timer->setSingleShot(false);
	timer->start();
	
	positionSource->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
        positionSource->startUpdates();

	return app.exec();
}
