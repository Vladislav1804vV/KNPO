#include <QCoreApplication>
#include "main.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc > 1)
        JavaParssing(argv[1]);

    printf("Done");

    return a.exec();
}


void JavaParssing(const char *fileName)
{

}

HANDLE openFile(QString Filename)
{

}

QList <HANDLE> openAllFiles(QString Filename)
{

}

QString absolutePath(QString Filename)
{

}

void fileExtensionJava(QString path)
{

}

void errorMessage(QString message)
{

}

void sameProject(QStringList listFiles)
{

}

QStringList fileToStringList(HANDLE handle)
{

}

QList <QStringList> getFilesArray(QList <HANDLE> list_handle)
{

}

void deleteComments(QList <QStringList> &list)
{

}

bool quotedThing(QStringList list, int index, int position)
{

}

bool thingInRoundBrackets(QStringList list, int index, int position)
{

}

void deleteAnnotations(QList <QStringList> &list)
{

}


QList <int> findEndBlock(QStringList list, int startPosition, int indexBrace)
{

}

void structuralViewOfCode(QList <QStringList> &list)
{

}

QList <QStringList> takePackageAndImports(QList <QStringList> list)
{

}

void parsingClasses(QStringList list, QString namePackage, int startPos, QXmlStreamWriter &xmlWriter)
{

}

QList <int> findClasses(QStringList list, int start, int end)
{

}

QList <int> findFields(QStringList list, int start, int end)
{

}

QList <int> findMethods(QStringList list, int start, int end)
{

}

void parsingFields(QStringList list, QList <int> listFields, QXmlStreamWriter &xmlWriter)
{

}

void parsingMethods(QStringList list, QList <int> listMethods, QXmlStreamWriter &xmlWriter)
{

}

QStringList correctlyBrokenByCommas(QString value)
{

}
