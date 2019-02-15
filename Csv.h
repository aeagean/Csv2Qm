#ifndef Csv_H
#define Csv_H

#include <QFile>
#include <QString>
#include <QVariantMap>

class Csv
{
public:
    Csv();
    virtual ~Csv();

    bool open(const QString &fileName);
    QList<QVariantMap> readAll();
    bool write(const QList<QVariantMap> &maps);
    void close();

private:
    QStringList splitCSVLine(const QString &lineStr);

private:
    QFile *m_file;
};

#endif // Csv_H
