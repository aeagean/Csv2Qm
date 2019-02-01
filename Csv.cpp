#include "Csv.h"

#include <QStringList>
#include <QDebug>

Csv::Csv()
{

}

bool Csv::open(const QString &fileName)
{
    m_file = new QFile(fileName);
    if (!m_file->open(QIODevice::ReadWrite)) {
        qDebug()<<"[error] "<<QString("Open %1 file failed!").arg(fileName);
        return false;
    }
    else {
        return true;
    }
}

QList<QVariantMap> Csv::readAll()
{
    QList<QVariantMap> data;

    if (m_file == NULL)
        return data;

    bool isFirstLine = true;
    QStringList titles;

    while (!m_file->atEnd()) {
        QByteArray line = m_file->readLine();
        QStringList contents = QString::fromLocal8Bit(line)
                               .simplified()
                               .split(',', QString::SkipEmptyParts);
        if (isFirstLine) {
            isFirstLine = false;
            titles = contents;
            continue;
        }

        QVariantMap item;
        for (int i = 0; i < contents.count(); i++) {
            if (i >= titles.count())
                continue;

            item.insert(item.find(titles[i]), titles[i], contents.at(i));
        }

        data.append(item);
    }

    return data;
}
