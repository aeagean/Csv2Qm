#ifndef CSV_H
#define CSV_H

#include <QFile>
#include <QString>
#include <QVariantMap>

class Csv
{
public:
    Csv();
    QList<QVariantMap> readAll();
    bool open(const QString &fileName);

private:
    QFile *m_file;
};

#endif // CSV_H
