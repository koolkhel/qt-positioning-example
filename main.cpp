#include <QGeoPositionInfoSource>
#include <QCoreApplication>
#include <QObject>
#include <QtDebug>
#include <QStringList>

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
				qDebug() << pos;
			});
	positionSource->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
        positionSource->setUpdateInterval(5000);
        positionSource->startUpdates();
	return app.exec();
}
