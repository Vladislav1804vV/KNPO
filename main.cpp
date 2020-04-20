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
    //Получить список массивов, содержащих строки кода каждого файла на языке Java
    QList <QStringList> list = getFilesArray(openAllFiles(absolutePath(fileName)));
    deleteComments(list);           //Удалить комментарии
    deleteAnnotations(list);        //Удалить аннотации
    structuralViewOfCode(list);     //Привести код к структурному виду
    //Выделить пакеты и импорты
    QList <QStringList>  list2 = takePackageAndImports(list);
    //Создать файл для записи данных (классов) в формате Xml и определить класс для данной реализации
    QFile file("xml.txt");
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    //Записать данные в формате XML
        //Начать документ
        xmlWriter.writeStartDocument();
            //Определить корневой элемент программы
            xmlWriter.writeStartElement("program");
            int count = 0;
            QString strPack;
            for(int i = 0; i < list.size(); i++)    //Для всех файлов программы определить общее кол-во классов
            {
                strPack = list2[i][0];
                list2[i].removeAll("");
                count += findClasses(list[i], list2[i].size(), list[i].size()-1).size();
            }
            //Записать атрибут кол-ва классов
            xmlWriter.writeAttribute("count", QString::number(count));
            for(int i = 0; i < list.size(); i++)    //Для всех файлов программы распарсить классы
            {
                parsingClasses(list[i], strPack, list2[i].size(), xmlWriter);
            }
            //Закрыть корневой элемент программы
            xmlWriter.writeEndElement();
        //Закончить документ
        xmlWriter.writeEndDocument();
    //Закрыть файл
    file.close();
    //Создать файл для записи данных (импортов) в формате Xml и определить класс для данной реализации
    QFile file2("import.txt");
    file2.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter2(&file2);
    xmlWriter2.setAutoFormatting(true);
    //Записать данные в формате Xml
        //Начать документ
        xmlWriter2.writeStartDocument();
            //Определить корневой элемент программы
            xmlWriter2.writeStartElement("import");
            for(int i = 0; i < list2.size(); i++)   //Для всех файлов программы определить импорты
            {
                int j = 0;
                QString strPack = "";
                if(list2[i][j] != "")
                {
                    strPack = list2[i][j];
                    j++;
                }
                for(j; j < list2[i].size(); j++)
                {
                    //Если в текущем файле отсутствует название пакета, определить элемент импорта без его использования
                    if(strPack == "")
                        xmlWriter2.writeStartElement(list2[i][j]);
                    //Иначе, определить элемент импорта, приписав в начале каждого импорта название пакета
                    else
                        xmlWriter2.writeStartElement(strPack + '.' + list2[i][j]);
                    //Закрыть элемент импорта
                    xmlWriter2.writeEndElement();
                }
            }

            //Закрыть коревой элемент программы
            xmlWriter2.writeEndElement();
        //Закончить документ
        xmlWriter2.writeEndDocument();
    //Закрыть файл
    file2.close();
}

HANDLE openFile(QString Filename)
{
    //Получить дескриптор открытого файла
    HANDLE myFile = CreateFileA(Filename.toStdString().c_str(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    //Вернуть дескриптор открытого файла
    return myFile;
}

QList <HANDLE> openAllFiles(QString Filename)
{
    //Получить дескриптор открытого файла
    HANDLE myFile = CreateFileA(Filename.toStdString().c_str(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    //Если дескриптор получен, получить заданные пути в файле и дескрипторы файлов по данным путям
    if(myFile != INVALID_HANDLE_VALUE)
    {
        //Прочитать данные в файле в отдельную строку
        const int size = GetFileSize(myFile, NULL) + 1;
        char str[size];
        str[size-1] = '\0';
        ReadFile(myFile, str, size, NULL, NULL);
        //Разбить строку по символу переноса новой строки
        QString string = QString::fromUtf8(str);
        QStringList list = string.split("\r\n");
        //Удалить пустые строки
        list.removeAll("");
        for(int i = 0; i < list.size(); i++)    //Для каждого пути получить его абсолютный путь
            list[i] = absolutePath(list[i]).simplified();
        //Удалить одинаковые пути
        list.removeDuplicates();
        //Получить дескрипторы всех файлов по заданным путям
            QList <HANDLE> list_handle;
            for(int i = 0; i < list.size(); i++)    //Для каждого пути проверить на принадлежность файла к языку Java и получить его дескриптор
            {
                //Проверить на принадлежность текущего файла к языку Java
                fileExtensionJava(list[i]);
                //Получить дескриптор текущего открытого файла
                HANDLE file = openFile(list[i]);
                //Если дескриптор текущего файла получен, записать его в список дескрипторов
                if(file != INVALID_HANDLE_VALUE)
                    list_handle << file;
                //Иначе, закрыть дескриптор текущего файла и объявить соответствующую ошибку
                else
                {
                    //Закрыть дескриптор текущего файла
                    CloseHandle(file);
                    //Объявить ошибку не существования текущего файла
                    errorMessage(list[i] + " does not exist");
                }
            }
        //Закрыть дескриптор файла с путями к файлам программы
        CloseHandle(myFile);
        //Если имеются пути к файлам, определить принадлежность файлов к одному проекту и вернуть список дескрипторов
        if(list.size())
        {
            //Определить принадлежность файлов к одному проекту
            sameProject(list);
            //Вернуть список дескрипторов
            return list_handle;
        }
        //Иначе, объявить ошибку пустого файла
        else
            errorMessage(Filename + "  is empty");

    }
    //Закрыть дескриптор файла с путями к файлам программы
    CloseHandle(myFile);
    //Объявить ошибку не существования файла с путями к файлам программы
    errorMessage(Filename + " does not exist");
}

QString absolutePath(QString Filename)
{
    //Определить текущий путь исполняемого файла
    QString cur_dir;
    cur_dir = QDir::currentPath();
    //Если заданный путь не является абсолютным, сделать его таковым, используя текущий путь исполняемого файла
    QString strong_filename;
    if (Filename[1] != ':')
    {
        //Вставить в начало строки текущий путь исполняемого файла
        strong_filename.prepend(cur_dir);
        //Вставить в конец строки символ '\'
        strong_filename.append("\\");
    }
    //Иначе, очистить строку
    else
        strong_filename.clear();
    //Вставить в конец строки заданный путь
    strong_filename.append(Filename);
    //Вернуть абсолютный путь
    return strong_filename;
}

void fileExtensionJava(QString path)
{
    //Определить позицию последнего вхождения символа '.' в заданном пути
    int point = path.lastIndexOf('.');
    //Если символ '.' найден, проверить строку на соответствующее вхождение
    if (point != NULL)
    {
        //Если строка после вхождения не соответсвует расширению языка Java, объявить ошибку непринадлежности файла к языку Java
        if(path.mid(point) != ".java")
            errorMessage("extension does not match the Java language extension in " + path);
    }
    //Иначе, объявить ошибку отсутствия расширения в заданном файле
    else
        errorMessage("missing file extension in " + path);
}

void errorMessage(QString message)
{
    //Напечатать сообщение об ошибке
    printf("%s\n", message.toStdString().c_str());
    _getch();
    //Досрочный выход из программы
    exit(0);
}

void sameProject(QStringList listFiles)
{
    QStringList srcList;
    for(int i = 0; i < listFiles.size(); i++) //Для всех путей файлов программы определить строки после вхождения строки "src", заключенной в символы обратной слэша
    {
        //Определить абсолютный путь текущего файла
        QString srcPath = absolutePath(listFiles[i]);
        //Если текущий путь файла содержит строку "src",заключенную в символы обратного слэша, выделить строку после данной
        if(srcPath.contains("\\src\\"))
        {
            //Определить позицию вхождения строки "src",заключенной в символы обратного слэша
            int src = srcPath.indexOf("\\src\\");
            //Выделить строку после данной
            srcPath.truncate(src+4);
            //Записать полученную строку
            srcList << srcPath;
        }
        //Иначе, объявить ошибку непринадлежности файлов к одному проекту
        else
            errorMessage("files do not belong to one project");
    }
    //Выделить строку первого пути после заданного вхождения
    QString comparePath = srcList[0];
    for(int i = 1; i < srcList.size(); i++) //Для всех полученных строк, кроме первой, сравнить их с первой строкой
        //Если текущая строка не равна первой, объявить ошибку непринадлежности файлов к одному проекту
        if(srcList[i] != comparePath)
            errorMessage("files do not belong to one project");
}

QStringList fileToStringList(HANDLE handle)
{
    //Прочитать данные в файле в отдельную строку
    const int size = GetFileSize(handle, NULL) + 1;
    char str[size];
    str[size-1] = '\0';
    ReadFile(handle, str, size, NULL, NULL);
    //Разбить строку по символу переноса новой строки
    QString string = QString::fromUtf8(str);
    QStringList list = string.split("\r\n");
    for(int i = 0; i < list.size(); i++) //Для каждой строки файла удалить лишние белые разделители
        list[i] = list[i].simplified();
    //Удалить пустые строки
    list.removeAll("");
    //Вернуть список строк файла
    return list;
}

QList <QStringList> getFilesArray(QList <HANDLE> list_handle)
{
    QList <QStringList> list;
    for(int i = 0; i < list_handle.size(); i++) //Для каждого дескриптора открытого файла, открыть файл языка Java в список строк кода
    {
        //Открыть файл языка Java в строки кода
        list << fileToStringList(list_handle[i]);
        //Закрыть дескриптор открытого файла
        CloseHandle(list_handle[i]);
    }
    //Вернуть список строк кода
    return list;
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
