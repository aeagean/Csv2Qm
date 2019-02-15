#include "Csv.h"

#include <QDebug>
#include <QStringList>

#include <utility>

Csv::Csv()
{
    m_file = NULL;
}

Csv::~Csv()
{
    if (m_file)
        delete m_file;
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

QStringList Csv::splitCSVLine(const QString &lineStr)
{
    QStringList strList;
    QString str;

    int length = lineStr.length();
    int quoteCount = 0;
    int repeatQuoteCount = 0;

    for(int i = 0; i < length; ++i) {
        if(lineStr[i] != '\"') {
            repeatQuoteCount = 0;
            if(lineStr[i] != ',') {
                str.append(lineStr[i]);
            }
            else {
                if(quoteCount % 2) {
                    str.append(',');
                }
                else {
                    strList.append(str.trimmed());
                    quoteCount = 0;
                    str.clear();
                }
            }
        }
        else {
            ++quoteCount;
            ++repeatQuoteCount;
            if(repeatQuoteCount == 4) {
                str.append('\"');
                repeatQuoteCount = 0;
                quoteCount -= 4;
            }
        }
    }
    strList.append(str.trimmed());

    return strList;
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

        QStringList contents = splitCSVLine(QString(line));

        if (isFirstLine) {
            isFirstLine = false;
            titles = contents;
        }

        QVariantMap item;
        for (int i = 0; i < contents.count(); i++) {
            if (i >= titles.count())
                continue;

            item.insert(/*item.find(titles[i]), */titles[i], contents.at(i));
        }

        data.append(item);
    }

    return data;
}

bool Csv::write(const QList<QVariantMap> &maps)
{
    if (m_file == NULL)
        return false;

    m_file->resize(0);

    /* 将内容写入到文件 */
    QString content;
    QString titles;
    for (int i = 0; i < maps.size(); i++) {
        const QVariantMap &item = maps[i];

        QMapIterator<QString, QVariant> iter(item);
        while (iter.hasNext()) {
            iter.next();

            if (i == 0) {
                titles.append(QString("%1,").arg(iter.key()));
                if (!iter.hasNext())
                    titles.append(QString("\n"));
            }
            else {
                content.append(QString("%1,").arg(iter.value().toString()));
                if (!iter.hasNext())
                    content.append(QString("\n"));
            }
        }

        if (i == 0)
            m_file->write(titles.toLocal8Bit(), titles.size());

        m_file->write(content.toLocal8Bit(), content.size());
    }

    return true;
}

void Csv::close()
{
    m_file->close();
}
