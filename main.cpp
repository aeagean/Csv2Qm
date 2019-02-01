#include "Csv.h"

#include <QCoreApplication>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

const QStringList Languages = {
    "zh_CN", "en"
};

static QString convertContext(QList<QVariantMap> source, QString contextTitle, QString language)
{
    QString context = QString("<context>\n");
    context += QString("\t<name>%1</name>\n").arg(contextTitle);
    for (int i = 0; i < source.count(); i++) {
        QVariantMap item;
        item["source"] = source.at(i).value("key").toString();
        if (Languages.contains(language)) {
            item["translation"] = source.at(i).value(language).toString();
        }
        else {
            qDebug()<<"Failed!!!";
            exit(-1);
        }

        context += QString("\t<message>\n\t\t<source>%1</source>\n\t\t<translation>%2</translation>\n\t</message>\n")
                    .arg(item["source"].toString())
                    .arg(item["translation"].toString());
    }
    context += QString("</context>\n");
    return context;
}

static bool convert(QList<QVariantMap> source, QString language, QString outputFile)
{
    QString head = QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n") +
        QString("<!DOCTYPE TS>\n") +
        QString("<TS version=\"2.0\" language=\"%1\">\n").arg(language);

    QString content;
    content += convertContext(source, "QObject", language);

    QString tail = QString("</TS>");

    QString result = head + content + tail;
    QFile file(outputFile);
    file.open(QFile::WriteOnly);
    file.write(result.toUtf8());
    file.close();
}

int main(int argc, char *argv[])
{
    QString file = "translation.csv";
    if (argc == 2) {
        file = argv[1];
    }

    Csv csvor;
    csvor.open(file);

    QList<QVariantMap> csvContexts = csvor.readAll();

    if (csvContexts.isEmpty()) {
        qDebug()<<"csvContexts Failed!!!";
        return -1;
    }

    convert(csvContexts, "zh_CN", "zh_CN.ts");
    convert(csvContexts, "en", "en.ts");

    if (system("lrelease *.ts") == 0)
        qDebug()<<"Success!!!";
    else
        qDebug()<<"Failed!!!";

    return 0;
}
