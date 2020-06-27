/*!
 * \file main.cpp
 * \brief файл, содержащий реализацию главных функций
*/
#include "main.h"

/*!
 * \mainpage Парсинг Java-классов
 * \brief  Программа предназначена для парсинга Java-классов (на поля, методы и т.д.).
 * <br>
 * Разработка проводится на основании задания на курсовой проект по дисциплине "Качество и надежность программного обеспечения", выданного доцентом кафедры ПОАС Сычёвым О.А. 11 февраля 2020 г.
 */
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Если второй аргумент командной строки не равен ключу "-test", выполнить функцию, чтобы разбить программу на языке Java на элементы (классы, методы и т.д.)
    if(argc == 2 && argv[1] != QString("-test"))
    {
        JavaParssing(argv[1]);
        printf("Done\n");
        _getch();
    }
    //Иначе, если второй аргумент командной строки равен ключу "-test", выполнить тестирование функций программы
    else if(argc == 2 && argv[1] == QString("-test"))
    {
        Test_findClasses findClasses;
        Test_findFields findFields;
        Test_findMethods findMethods;
        Test_parsingClasses parsingClasses;
        Test_parsingFields parsingFields;
        Test_parsingMethods parsingMethods;

        QTest::qExec(&findClasses);
        QTest::qExec(&findFields);
        QTest::qExec(&findMethods);
        QTest::qExec(&parsingClasses);
        QTest::qExec(&parsingFields);
        QTest::qExec(&parsingMethods);
    }
    //Иначе, если дополнительных аргументов объявлено не было, сделать запрос аргумента у пользователя
    else if(argc == 1)
    {
        char path[100] = "";
        gets(path);
        JavaParssing(path);
        printf("Done\n");
        _getch();
    }
    //Иначе, выдать сообщение о введении неверного запроса
    else
        errorMessage("invalid request");

    return 0;
}


void JavaParssing(const char *fileName)
{
    //Объявить список структур для хранения найденных классов
    QList <dataWrittenClass> dataXML;
    //Объявить список строк для хранения выделенных "импортов"
    QStringList imports;
    //Получить список массивов, содержащих строки кода каждого файла на языке Java
    QList <QStringList> list = getFilesArray(openAllFiles(absolutePath(fileName)));
    for(int i = 0; i < list.size(); i++)
        if(list[i].isEmpty())
            list.removeAt(i);
    deleteComments(list);           //Удалить комментарии
    deleteAnnotations(list);        //Удалить аннотации
    structuralViewOfCode(list);     //Привести код к структурному виду
    //Выделить пакеты и импорты
    QList <QStringList>  list2;
    takePackageAndImports(list, list2);
    int count = 0;
    QStringList strPack;
    for(int i = 0; i < list.size(); i++)    //Для всех файлов программы определить общее кол-во классов
    {
        strPack << list2[i][0];
        list2[i].removeAll("");
        count += findClasses(list[i], list2[i].size(), list[i].size()-1).size();
    }
    for(int i = 0; i < list.size(); i++)    //Для всех файлов программы распарсить классы
        parsingClasses(list[i], strPack[i], list2[i].size(), dataXML);
    QStringList strNamePack;
    for(int i = 0; i < list2.size(); i++)   //Для всех файлов программы определить импорты
    {
        int j = 0;
        QString namePack = "";
        //Если первая строка текущего файла непустая, сохранить её и перейти к следующей строке
        if(!list2[i].isEmpty() && list2[i][j] != "")
        {
            namePack = list2[i][j] + '.';
            j++;
        }
        for(j; j < list2[i].size(); j++)    //Для всех строк текущего файл определить импорты
        {
            //Если текущий импорт не повторяет уже существующий, определить его
            if(!strNamePack.contains(namePack + list2[i][j]))
            {
                //Сохранить текущий импорт в список
                strNamePack << namePack + list2[i][j];
                //Определить элемент программы равный названию импорта
                imports.append(namePack + list2[i][j]);
            }
        }
    }
    //Записать данные в формате XML
    writeToXML(dataXML, imports);
}

HANDLE openFile(QString fileName)
{
    //Получить дескриптор открытого файла
    HANDLE myFile = CreateFileA(fileName.toLocal8Bit(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    //Вернуть дескриптор открытого файла
    return myFile;
}

QList <HANDLE> openAllFiles(QString fileName)
{
    //Получить дескриптор открытого файла
    HANDLE myFile = CreateFileA(fileName.toLocal8Bit(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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
        QStringList list = string.split("\n");
        for(int i = 0; i < list.size(); i++) //Для каждой строки файла удалить лишние белые разделители
            list[i] = list[i].simplified();
        //Удалить пустые строки
        list.removeAll("");
        for(int i = 0; i < list.size(); i++)    //Для каждого пути получить его абсолютный путь
            list[i] = absolutePath(list[i]);
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
        //Если имеются пути к файлам, определить принадлежность файлов к одному проекту, если кол-во файлов больше одного и вернуть список дескрипторов
        if(list.size())
        {
            //Определить принадлежность файлов к одному проекту, если кол-во файлов больше одного
            if(list.size() > 1)
                sameProject(list);
            //Вернуть список дескрипторов
            return list_handle;
        }
        //Иначе, объявить ошибку пустого файла
        else
            errorMessage(fileName + "  is empty");

    }
    //Закрыть дескриптор файла с путями к файлам программы
    CloseHandle(myFile);
    //Объявить ошибку не существования файла с путями к файлам программы
    errorMessage(fileName + " does not exist");
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
    if (point != -1)
    {
        //Если строка после вхождения не соответствует расширению языка Java, объявить ошибку непринадлежности файла к языку Java
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
        //Если текущий путь файла содержит строку "src", заключенную в символы обратного слэша, выделить строку после данной
        if(srcPath.contains("\\src\\"))
        {
            //Определить позицию вхождения строки "src", заключенной в символы обратного слэша
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
    QStringList list = string.split("\n");
    for(int i = 0; i < list.size(); i++) //Для каждой строки файла удалить лишние белые разделители
        list[i] = list[i].simplified();
    //Удалить пустые строки
    list.removeAll("");
    //Вернуть список строк файла
    return list;
}

QList <QStringList> getFilesArray(QList <HANDLE> listHandle)
{
    QList <QStringList> list;
    for(int i = 0; i < listHandle.size(); i++) //Для каждого дескриптора открытого файла, открыть файл языка Java в список строк кода
    {
        //Открыть файл языка Java в строки кода
        list << fileToStringList(listHandle[i]);
        //Закрыть дескриптор открытого файла
        CloseHandle(listHandle[i]);
    }
    //Вернуть список строк кода
    return list;
}

void deleteComments(QList <QStringList> &list)
{
    int position = 0;
    for(int i = 0; i < list.size(); i++)    //Для всех файлов программы удалить комментарии
    {
        for(int j = 0; j < list[i].size(); j++) //Для каждой строки кода файла программы найти комментарий
        {
            //Если текущая строка кода файла программы содержит однострочный комментарий или начало многострочного, удалить комментарий, определив его вид
            if(list[i][j].contains("//") || list[i][j].contains("/*"))
            {
                //Определить вид комментария
                    int positionSingleComment = list[i][j].mid(position).indexOf("//");
                    int positionMultiComment = list[i][j].mid(position).indexOf("/*");
                    //Если однострочный комментарий не найден, считать наличие многострочного комментария
                    if(positionSingleComment == -1)
                        position += positionMultiComment;
                    //Иначе, если многострочный комментарий не найден, считать наличие однострочного комментария
                    else if(positionMultiComment == -1)
                        position += positionSingleComment;
                    //Иначе, если признак однострочного комментария находится раньше признака многострочного, считать наличие однострочного комментария
                    else if(positionSingleComment < positionMultiComment)
                        position += positionSingleComment;
                    //Иначе, считать наличие многострочного комментария
                    else
                        position += positionMultiComment;
                //Если комментарий не "закавычен", удалить комментарий
                if(!quotedThing(list[i], j, position))
                {
                    //Если определен однострочный комментарий, удалить его
                    if(list[i][j].mid(position).startsWith("//"))
                    {
                        list[i][j].truncate(position);
                    }
                    //Иначе, если определен многострочный комментарий, определить конец комментария и удалить его
                    else if(list[i][j].mid(position).startsWith("/*"))
                    {
                        //Если в текущей строке не содержится конец комментария, найти конец, удаляя все предыдущие строки кода
                        if(!list[i][j].mid(position).contains("*/"))
                        {
                            //Удалить текущую строку кода
                            list[i][j].truncate(position);
                            bool isFind = false;
                            for(++j; j < list[i].size() && isFind == false; j++) //Для каждой строки кода от последующей до последней найти конец комментария
                                //Если текущая строка содержит конец многострочного комментария, удалить комментарий
                                if(list[i][j].contains("*/"))
                                {
                                    //Определить позицию конца комментария
                                    position = list[i][j].indexOf("*/");
                                    //Если конец комментария не "закавычен", удалить комментарий
                                    list[i][j].remove(0, position + 2);
                                    j = j - 2;
                                    isFind = true;
                                }
                                //Иначе, удалить строку целиком
                                else
                                    list[i][j].clear();
                        }
                        //Иначе, удалить комментарий
                        else
                        {
                            int position2 = list[i][j].mid(position).indexOf("*/");
                            list[i][j].remove(position, position2+2);
                            j--;
                        }
                    }
                    position = 0;
                }
                //Иначе, инкрементировать позицию найденного комментария
                else
                {
                    position++;
                    j--;
                }
            }
        }
        for(int k = 0; k < list.size(); k++)    //Для каждой строки кода программы удалить лишние белые разделители
            list[i][k] = list[i][k].simplified();
        //Удалить все пустые строки кода программы
        list[i].removeAll("");
    }
}

bool quotedThing(QStringList list, int index, int position)
{
    //Убрать правую часть строки от заданной позиции
    list[index] = list[index].left(position);
    //Считать счетчик кол-ва кавычек равным 0
    int quotedCount = 0;
    for(int i = 0; i <= index; i ++)    //Для каждой строки кода программы от первой и до заданной (включительно) подсчитать кол-во кавычек
    {
        //Прибавить к общему кол-ву кавычек число кавычек, входящих в текущую строку
        quotedCount += list[i].count("\"");
        //Если в текущую строку входят экранированные кавычки, вычесть их кол-во из общего кол-ва кавычек
        if(list[i].contains("\\\""))
            quotedCount -= list[i].count("\\\"");
    }
    //Если общее кол-во кавычек нечетное, вернуть признак "закавыченности" символа в заданной строке и позиции
    if(quotedCount % 2 != 0)
        return true;
    //Иначе, вернуть признак отсутствия "закавыченности"
    else
        return false;
}

bool thingInRoundBrackets(QStringList list, int index, int position)
{
    //Убрать правую часть строки от заданной позиции
    list[index] = list[index].left(position);
    //Считать счетчик кол-ва открытых скобок равным 0
    int openBracketsCount = 0;
    //Считать счетчик кол-ва закрытых скобок равным 0
    int closeBracketsCount = 0;
    //Принять переменную для позиций вхождений
    int pos;
    for(int i = 0; i <= index; i ++)    //Для каждой строки кода программы от первой и до заданной (включительно) подсчитать кол-во открытых и закрытых скобок по отдельности
    {
        //Обнулить позицию вхождения
        pos = 0;
        for(int j = 0; j < list[i].count("("); j++) //Для текущей строки прибавить кол-во "незакавыченных" открытых скобок
        {
            //Определить позицию вхождения открытой скобки
            pos = list[i].indexOf("(", pos);
            //Если данная скобка не "закавычена", инкрементировать общее кол-во открытых скобок
            if(!quotedThing(list, i, pos))
                openBracketsCount++;
            //Инкрементировать позицию текущего вхождения открытой скобки
            pos++;
        }
        //Обнулить позицию вхождения
        pos = 0;
        for(int j = 0; j < list[i].count(")"); j++) //Для текущей строки прибавить кол-во "незакавыченных" закрытых скобок
        {
            //Определить позицию вхождения закрытой скобки
            pos = list[i].indexOf(")", pos);
            //Если данная скобка не "закавычена", инкрементировать общее кол-во закрытых скобок
            if(!quotedThing(list, i, pos))
                closeBracketsCount++;
            //Инкрементировать позицию текущего вхождения закрытой скобки
            pos++;
        }
    }
    //Если общее кол-во открытых скобок не равняется общему кол-ву закрытых скобок, вернуть признак заключения символа в скобках в заданной строке и позиции
    if(openBracketsCount != closeBracketsCount)
        return true;
    //Иначе, вернуть признак отсутствия заключения символа в скобках
    else
        return false;
}

void deleteAnnotations(QList <QStringList> &list)
{
    for(int i = 0; i < list.size(); i++)    //Для всех файлов программы удалить аннотации
    {
        for(int j = 0; j < list[i].size(); j++) //Для каждой строки кода текущего файла программы, удалить аннотации
        {
            //Если текущая строка содержит символ начала аннотации '@'
            if(list[i][j].contains("@"))
            {
                //Определить позицию вхождения символа начала аннотации
                int position = list[i][j].indexOf("@");
                //Принять массив для хранения индекса строки и позиции начала аннотации
                int startPosition[2];
                //Принять массив для хранения индекса строки и позиции конца аннотации
                int endPosition[2];
                //Если позиция вхождения символа не "закавычена" и не заключена в скобки, определить начало и конец аннотации и удалить ее
                if(!quotedThing(list[i], j, position) && !thingInRoundBrackets(list[i], j, position))
                {
                    //Считать индекс строки начала аннотации равным индексу текущей строки
                    startPosition[0] = j;
                    //Считать позицию в строке начала аннотации равным позиции вхождения символа
                    startPosition[1] = position;
                    list[i][j] = list[i][j].simplified();
                    //Если текущая строка непустая и заканчивается модификатором доступа аннотации, определить индекс строки и позиции начала аннотации
                    if(!list[i][j].isEmpty() && (list[i][j].left(position).endsWith("public ") || list[i][j].left(position).endsWith("private ")))
                    {
                        //Если текущая строка заканчивается модификатором доступа "public", считать позицию начала аннотации началом модификатора доступа "public"
                        if(list[i][j].left(position).endsWith("public "))
                            startPosition[1] = list[i][j].left(position).lastIndexOf("public");
                        //Иначе, если текущая строка заканчивается модификатором доступа "private", считать позицию начала аннотации началом модификатора доступа "private"
                        else if(list[i][j].left(position).endsWith("private "))
                            startPosition[1] = list[i][j].left(position).lastIndexOf("private");
                        //Считать индекс строки начала аннотации равным индексу текущей строки
                        startPosition[0] = j;
                    }
                    //Иначе, если индекс текущей строки не равен 0, определить индекс строки и позиции начала аннотации
                    else if(j != 0)
                    {
                        list[i][j-1] = list[i][j-1].simplified();
                        //Если предыдущая строка заканчивается модификатором доступа "public", определить индекс строки и позиции начала аннотации
                        if(list[i][j-1].endsWith("public"))
                        {
                            //Считать индекс строки начала аннотации равным индексу предыдущей строки
                            startPosition[0] = j-1;
                            //Считать позицию в строке начала аннотации началом модификатора доступа "public"
                            startPosition[1] = list[i][j-1].lastIndexOf("public");
                        }
                        //Иначе, если предыдущая строка заканчивается модификатором доступа "private", определить индекс строки и позиции начала аннотации
                        else if(list[i][j-1].endsWith("private"))
                        {
                            //Считать индекс строки начала аннотации равным индексу предыдущей строки
                            startPosition[0] = j-1;
                            //Считать позицию в строке начала аннотации началом модификатора доступа "private"
                            startPosition[1] = list[i][j-1].lastIndexOf("private");
                        }
                    }
                    //Удалить в текущей строке лишние белые разделители
                    list[i][j] = list[i][j].simplified();
                    //Если позиция вхождения символа начала аннотации находится в конце строки, перейти к следующей строке, обнуляя позицию вхождения
                    if(list[i][j].mid(position) == "@")
                    {
                        //Инкрементировать индекс текущей строки
                        j++;
                        //Обнулить позицию вхождения
                        position = 0;
                    }
                    //Иначе, инкрементировать позицию вхождения
                    else
                        position++;
                    //Удалить в текущей строке лишние белые разделители
                    list[i][j] = list[i][j].simplified();
                    //Если текущая позиция вхождения содержит пробельный симмвол, инкрементировать позицию вхождения
                    if(list[i][j].at(position) == " ")
                        position++;
                    //Если строка от заданной позиции начинается строкой "interface" и не является концом текущей строки, сместить позицию вхождения на следующее слово после слова "interface"
                    if(list[i][j].mid(position).startsWith("interface "))
                        position += list[i][j].mid(position).indexOf(" ")+1;
                    //Иначе, если строка от заданной позиции заканичает текущую строку словом "interface", перейти на следующую строку, обнуляя позицию вхождения
                    else if(list[i][j].mid(position) == "interface")
                    {
                        //Инкрементировать индекс текущей строки
                        j++;
                        //Перейти к следующему буквенному символу, обнулив позицию вхождения
                        list[i][j] = list[i][j].simplified();
                        position = 0;
                    }
                    //Найти позицию первого пробельного символа от заданной позиции
                    list[i][j] = list[i][j].replace("(", " (");
                    list[i][j] = list[i][j].replace("{", " {");
                    list[i][j] = list[i][j].simplified();
                    int pos = list[i][j].mid(position).indexOf(" ");
                    //Если позиция конца аннотации является концом текущей строки, определить позицию конца аннотации и перейти на следующую строку
                    if(pos == -1)
                    {
                        //Считать индекс строки конца аннотации равным индексу текущей строки
                        endPosition[0] = j;
                        //Считать позицию в строке конца аннотации концом текущей строки
                        endPosition[1] = list[i][j].length()-1;
                        //Инкрементировать индекс текущей строки
                        j++;
                        //Перейти к следующему буквенному символу, обнулив позицию вхождения
                        list[i][j] = list[i][j].simplified();
                        position = 0;
                    }
                    //Иначе, определить позицию конца аннотации на основе найденной позиции пробельного символа
                    else
                    {
                        //Определить позицию первого пробельного символа от начала текущей строки
                        position += pos;
                        //Считать индекс строки конца аннотации равным индексу текущей строки
                        endPosition[0] = j;
                        //Считать позицию в строке конца аннотации позицию до первого пробельного символа от начала текущей строки
                        endPosition[1] = position-1;
                        //Инкрементировать позицию вхождения
                        position++;
                    }
                    //Если текущая позиция вхождения является началом аргумента аннотации или началом тела в случае аннотации интерфейса, определить позицию конца аннотации
                    if(list[i][j].at(position) == '(' || list[i][j].at(position) == '{')
                    {
                        QList <int> pos;
                        //Если текущая позиция вхождения является началом аргумента аннотации, определить позицию конца аннотации
                        if(list[i][j].at(position) == '(')
                            pos = findClosingSymbol(list[i], j, position, ')');
                        //Иначе, если текущая позиция вхождения является началом тела аннотации, определить позицию конца аннотации
                        else if(list[i][j].at(position) == '{')
                            pos = findClosingSymbol(list[i], j, position, '}');
                        //Считать индекс строки конца аннотации равным найденному индексу
                        endPosition[0] = pos[0];
                        //Считать позицию в строке конца аннотации равной найденной позиции
                        endPosition[1] = pos[1];
                    }
                    //Считать индекс текущей строки равным индексу начала аннотации
                    j = startPosition[0];
                    //Если аннотация находится на одной строке, удалить ее, зная начало и конец аннотации
                    if(startPosition[0] == endPosition[0])
                        list[i][j].remove(startPosition[1], endPosition[1] - startPosition[1] + 1);
                    //Иначе, удалить аннотацию, удаляя строки до ее конца
                    else
                    {
                        //Вырезать строку от начала аннотации до конца текущей строки
                        list[i][j].truncate(startPosition[1]);
                        //Инкрементировать индекс текущей строки
                        j++;
                        while(j != endPosition[0])  //Пока индекс текущей строки не равен индексу конца аннотации, удалить строку и перейти к следующей
                        {
                            //Удалить текущую строку
                            list[i][j].clear();
                            //Инкрементировать индекс текущей строки
                            j++;
                        }
                        //Вырезать последнюю строку, содержащую аннотация, от начала текущей строки до конца аннотации
                        list[i][j].remove(0, endPosition[1] + 1);
                    }
                    //Декрементировать индекс текущей строки
                    j--;
                }
            }
        }
    //Для каждой строки кода текущего файла программы, удалить лишние белые разделители
    for(int k = 0; k < list[i].size(); k++)
        list[i][k] = list[i][k].simplified();
    //Удалить пустые строки
    list[i].removeAll("");
    }
}


QList <int> findClosingSymbol(const QStringList &list, int startPosition, int indexBrace, QChar closeSymbol)
{
    //Определить открывающийся символ
    QChar openSymbol = list[startPosition].at(indexBrace);
    //Считать счетчик открытых/закрытых символов равным 1
    int symbolCount = 1;
    //Считать индексы открытого и закрытого символов равными заданному индексу
    int indexOpen = indexBrace;
    int indexClose = indexBrace;
    int sizeOpen;
    int sizeClose;
    QString spaceStr;
    for(int i = startPosition; i < list.size(); i++)    //Для каждой строки кода текущего файла программы от заданной до последней, определить позицию закрывающегося символа
    {
        //Если в текущей строке содержатся символы закрытия, определить значение счетчика открытых/закрытых символов
        if(list[i].mid(indexClose).contains(closeSymbol))
        {
            //Определить кол-во символов закрытия
            sizeClose = list[i].mid(indexClose).count(closeSymbol);
            for(int j = 0; j < sizeClose; j++)  //Для каждого найденного символа закрытия определить символы открытия в их промежутках
            {
                //Определить индекс текущего символа закрытия
                indexClose += list[i].mid(indexClose).indexOf(closeSymbol);
                //Если индекс текущей строки равен заданному индексу, выделить подстроку между индексом открытого и закрытого символов (не включая их)
                if(list[i].count() > 1)
                    spaceStr = list[i].mid(indexOpen + 1, indexClose - indexOpen - 1);
                //Иначе, выделить подстроку между индексом открытого и закрытого символов (не включая индекс закрытого)
                else
                   spaceStr = list[i].mid(indexOpen, indexClose - indexOpen);
                //Определить кол-во символов открытия в найденном промежутке
                sizeOpen = spaceStr.count(openSymbol);
                for(int k = 0; k < sizeOpen; k++)   //Для каждого найденного символа открытия определить его "закавыченность"
                {
                    //Определить индекс текущего символа открытия
                    indexOpen += list[i].mid(indexOpen+1).indexOf(openSymbol);
                    //Если индекс текущей строки равен заданному индексу, инкрементировать индекс открытого символа
                    if(i == startPosition)
                        indexOpen++;
                    //Если текущий символ открытия не "закавычен", инкрементировать счетчик открытых/закрытых символов
                    if(!quotedThing(list, i, indexOpen))
                        symbolCount++;
                    //Инкрементировать индекс открытого символа
                    indexOpen++;
                }
                //Если текущий символ закрытия не "закавычен", декрементировать счетчик открытых/закрытых символов
                if(!quotedThing(list, i, indexClose))
                    symbolCount--;
                //Если счетчик открытых/закрытых символов равен 0, вернуть позицию закрывающегося символа
                if(symbolCount == 0)
                    return QList <int>() << i << indexClose;
                //Инкрементировать индекс закрытого символа
                indexClose++;
            }
        }
        //Иначе, определить значение счетчика, уменьшив его на кол-во найденных "незакавыченных" символов открытия
        else
        {
            //Если индекс текущей строки равен заданному индексу, декрементировать счетчик открытых/закрытых символов
            if(i == startPosition)
                symbolCount--;
            //Если длина текущей строки равна 1, определить наличие в ней символа открытия
            if(list[i].length() == 1)
                sizeOpen = list[i].count(openSymbol);
            //Иначе, определить кол-во символов открытия
            else
                sizeOpen = list[i].mid(indexOpen).count(openSymbol);
            for(int j = 0; j < sizeOpen; j++)   //Для каждого найденного символа открытия определить "незакавыченные" символы открытия
            {
                //Определить индекс текущего символа открытия
                indexOpen += list[i].mid(indexOpen).indexOf(openSymbol);
                //Если текущий символ открытия не "закавычен", инкрементировать счетчик открытых/закрытых символов
                if(!quotedThing(list, i, indexOpen))
                    symbolCount++;
                 //Инкрементировать индекс открытого символа
                indexOpen++;
            }
            //Декрементировать индекс открытого символа
            indexOpen--;
        }
        sizeOpen = 0;
        //Если индекс открытого символа не равен индексу закрытого символа, определить кол-во символов открытия
        if(indexOpen != indexClose)
            sizeOpen = list[i].mid(indexClose).count(openSymbol);
        indexOpen = indexClose;
        for(int j = 0; j < sizeOpen; j++) //Для каждого найденного символа открытия определить значение счетчика открытых/закрытых символов
        {
            //Определить индекс текущего символа открытия
            indexOpen += list[i].mid(indexOpen + 1, list[i].length() - indexOpen - 2).indexOf(openSymbol) + 2;
            //Если текущий символ открытия не "закавычен", инкрементировать счетчик открытых/закрытых символов
            if(!quotedThing(list, i, indexOpen))
                symbolCount++;
        }
        //Обнулить индексы символов открытия и закрытия
        indexOpen = 0;
        indexClose = 0;

    }
    //Вернуть неопределенную ошибку
    errorMessage("Java program contains an unrecognized error");
}

void structuralViewOfCode(QList <QStringList> &list)
{
    for(int i = 0; i < list.size(); i++) //Для всех файлов программы привести код к структурному виду
    {
        //"Склеить" строки кода текущего файла программы в одну строку
        QString str = list[i].join(" ");
        //Удалить код текущего файла
        list[i].clear();
        //Разместить каждый простой оператор и символ тела блока в отдельных строках
        str.replace(";", ";\n");
        str.replace("{", "\n{\n");
        str.replace("}", "\n}\n");
        list[i] = str.split("\n");
        for(int j = 0; j < list[i].size(); j++) //Для каждой строки кода текущего файла программы отредактировать неправильное размещение кода в отдельных строках
        {
            //Удалить пустые строки
            list[i][j] = list[i][j].simplified();
            list[i].removeAll("");
            //Определить последний символ в текущей строке
            QChar symbol = list[i][j].at(list[i][j].length()-1);
            //Если символ пустого оператора "закавычен" или заключен в скобках, "приклеить" к текущей строке последующую
            if(symbol == ";" && (quotedThing(list[i], j, list[i][j].length()-1) || thingInRoundBrackets(list[i], j, list[i][j].length()-1)))
            {
                list[i][j] += " " + list[i][j+1];
                list[i].removeAt(j+1);
                j--;
            }
            //Иначе, если символ тела блока "закавычен" или заключен в скобках, "приклеить" к текущей строке предыдущую и последующую
            else if((symbol == "{" || symbol == "}") && (quotedThing(list[i], j, list[i][j].length()-1) || thingInRoundBrackets(list[i], j, list[i][j].length()-1)))
            {
                list[i][j-1] += " " + list[i][j] + " " + list[i][j+1];
                list[i].removeAt(j);
                list[i].removeAt(j);
                j--;
            }
            //Иначе, если символ равен символу равенства, приклеить" к текущей строке последующие до найденной строки
            else if(symbol == "=")
            {
                int pos = findClosingSymbol(list[i], j+1, 0, '}')[0];
                for(int k = j; k <= pos; k++)
                {
                    list[i][k] += " " + list[i][k+1];
                    list[i].removeAt(k+1);
                    k--;
                    pos--;
                }
                j--;
            }
        }
        //Удалить строки состоящие только из символа пустого оператора
        list[i].removeAll(";");
    }
}

void takePackageAndImports(const QList <QStringList> &list, QList <QStringList>& retList)
{
    for(int i = 0; i < list.size(); i++) //Для всех файлов программы определить "импорты" и наличие/отсутствие пакета
    {
        QStringList strList;
        int j = 0;
        //Если первая строка кода текущего файла программы начинается словом "package", сохранить название пакета и перейти к следующей строке
        if(list[i][j].startsWith("package "))
        {
            strList << list[i][j].mid(QString("package").length()+1, list[i][j].mid(QString("package").length()+1).length()-1);
            j++;
        }
        //Иначе, сохранить название пакета как пустую строку
        else
            strList << "";
        while(list[i][j].startsWith("import "))  //Пока последующие строки начинаются словом "import", сохранять названия "импортов"
        {
            strList << list[i][j].mid(QString("import").length()+1, list[i][j].mid(QString("import").length()+1).length()-1);
            j++;
        }
        //Сохранить данные текущего файла об "импортах" и наличии/отсутствии пакета
        retList << strList;
    }
}

void parsingClasses(const QStringList &list, QString namePackage, int startPos, QList <dataWrittenClass> &dataClasses)
{
    //Определить позиции заголовков всех классов заданного файла программы
    QList <int> listClasses = findClasses(list, startPos, list.size()-1);
    for(int i = 0; i < listClasses.size(); i++) //Для все найденных классов провести их "парсинг"
    {
        //Объявить структуру для хранения текущего класса
        dataWrittenClass dataClass;
        //Определить строку, содержащую имя заголовка текущего класса и имя пакета при его наличии
            QString nameClass = namePackage;
            QStringList str = list[listClasses[i]].mid(list[listClasses[i]].indexOf("class")).split(" ");
            //Если название пакета отсутствует, считать строку состоящей только из имени заголовка текущего класса
            if(namePackage == "")
                nameClass += str[1].trimmed();
            //Иначе, к имени пакета добавить имя заголовка текущего класса
            else
                 nameClass += '.' + str[1].trimmed();
        //Определить элемент программы, содержащий все найденные классы
        dataClass.nameClass = nameClass;
            //Определить наличие операторов "implements" и "extends"
                QString strOperator = "";
                //Если после имени класса имеется оператор, выделить строку после имени класса
                if(str.size() > 2)
                    for(int j = 2; j < str.size(); j++) //Для всех частей строки после имени класса склеить последующие строки
                        strOperator += str[j] + ' ';
                //Если выделенная строка начинается оператором "implements", определить его как атрибут элемент программы
                if(strOperator.startsWith("implements "))
                    dataClass.implementsValue = strOperator.mid(QString("implements").length()+1).simplified();
                //Иначе, если выделенная строка начинается оператором "extends", определить его как атрибут элемент программы
                else if(strOperator.startsWith("extends "))
                    dataClass.extendsValue = strOperator.mid(QString("extends").length()+1).simplified();
            //Обработать все вложенные классы
                //Найти позиции заголовков всех вложенных классов текущего класса
                QList <int> listClasses2 = findClasses(list, listClasses[i]+1, findClosingSymbol(list, listClasses[i]+1, 0, '}')[0]);
            //Обработать все поля текущего класса
                //Найти позиции всех полей текущего класса заданного файла программы
                QList <int> listFields = findFields(list, listClasses[i]+2, findClosingSymbol(list, listClasses[i]+1, 0, '}')[0]);
                //"Распарсить" поля текущего класса
                parsingFields(list, listFields, dataClass.fields);
            //Обработать все методы текущего класса
                //Найти позиции всех методов текущего класса заданного файла программы
                QList <int> listMethods = findMethods(list, listClasses[i]+2, findClosingSymbol(list, listClasses[i]+1, 0, '}')[0]);
                //"Распарсить" методы текущего класса
                parsingMethods(list, listMethods, dataClass.methods);
            //Если найдены вложенные классы, "распарсить" их
            if(listClasses2.size() != 0)
                parsingClasses(list, nameClass, listClasses[i]+1, dataClass.nestedClasses);
            //Сохранить заполненную структуру текущего класса
            dataClasses.append(dataClass);
    }
}

QList <int> findClasses(const QStringList &list, int start, int end)
{
    QList <int> pos;
    for(int i = start; i <= end; i++) //Для всех строк кода заданного файла программы от заданного начала и конца определить позиции всех заголовков классов
    {
        //Если текущая строка содержит строку "class" и она не "закавычена" и не заключена в скобках, определить позицию найденного заголовка класса
        if(list[i].contains("class ") && !quotedThing(list, i, list[i].indexOf("class ")) && !thingInRoundBrackets(list, i, list[i].indexOf("class ")))
        {
            //Сохранить позицию заголовка класса как индекс текущей строки
            pos << i;
            //Найти конец тела данного класса
            i = findClosingSymbol(list, ++i, 0, '}')[0];
        }
    }
    //Вернуть позиции всех заголовков классов заданного файла программы
    return  pos;
}

QList <int> findFields(const QStringList &list, int start, int end)
{
    QList <int> pos;
    for(int i = start; i <= end; i++) //Для всех строк кода заданного файла программы от заданного начала и конца определить позиции всех полей
    {
        //Если текущая строка содержит "незакавыченную" круглую открывающуюся скобку и перед ней есть символ "равно", определить все поля в строке
        if(list[i].contains("(") && !quotedThing(list, i, list[i].indexOf("(")) && list[i].left(list[i].indexOf("(")).contains("="))
        {
            //Если текущая строка содержит символ "запятая", определить все поля в строке
            if(list[i].contains(","))
            {
                //Заменить все фигурные и квадратные скобки на аналогичные круглые скобки
                QString str = list[i];
                str = str.replace("{", "(");
                str = str.replace("}", ")");
                str = str.replace("<", "(");
                str = str.replace(">", ")");
                //Разбить строку по запятым
                QStringList listComma = str.split(",");
                //Сохранить первую часть разбиения
                QString str2 = listComma[0];
                for(int j = 1; j < listComma.size(); j++)   //Для всех частей разбиения, определить "закавыченность" запятых, "приклеивая" к первой части разбиения текущие
                {
                    //"Приклеить" текущую строку
                    str2 += ',' + listComma[j];
                    //Если последняя запятая в строке не "закавычена" и не заключена в скобках, сохранить позицию поля как индекс текущей строки
                    if(!quotedThing(list, i, str2.lastIndexOf(",")) && !thingInRoundBrackets(list, i, str2.lastIndexOf(",")))
                        pos << i;
                }
            }
            //Сохранить позицию поля как индекс текущей строки
            pos << i;
        }
        //Иначе, если текущая строка не содержит круглую открывающуюся и фигурную скобки, определить все поля в строке
        else if(!list[i].contains("(") && list[i] != '{')
        {
            //Если текущая строка содержит "незакавыченное" слово "class" и последующая строка начинается символом открытия тела, определить конец тела
            if((list[i].startsWith("class ") || list[i].contains(" class ")) && !quotedThing(list, i, list[i].indexOf("class ")) && list[++i].startsWith("{"))
                i = findClosingSymbol(list, i, 0, '}')[0];
            //Иначе, если текущая строка содержит "незакавыченное" слово "interface" и последующая строка начинается символом открытия тела, определить конец тела
            else if((list[i].startsWith("interface ") || list[i].contains("interface ")) && !quotedThing(list, i, list[i].indexOf("interface ")) && list[++i].startsWith("{"))
                i = findClosingSymbol(list, i, 0, '}')[0];
            //Иначе, если текущая строка не начинается символами открытия и закрытия тела и не равна строке "static",  определить все поля в строке
            else if(!list[i].startsWith("{") && !list[i].startsWith("}") && list[i] != "static")
            {
                //Если текущая строка содержит символ "запятая", определить все поля в строке
                if(list[i].contains(","))
                {
                    //Создать копию кода текущего файла программы
                    QStringList dopList = list;
                    //Заменить все фигурные и квадратные скобки на аналогичные круглые скобки в данной копии
                    dopList[i] = dopList[i].replace("{", "(");
                    dopList[i] = dopList[i].replace("}", ")");
                    dopList[i] = dopList[i].replace("<", "(");
                    dopList[i] = dopList[i].replace(">", ")");
                    //Разбить текущую строку копии по запятым
                    QStringList listComma = dopList[i].split(",");
                    //Сохранить первую часть разбиения
                    QString str2 = listComma[0];
                    for(int j = 1; j < listComma.size(); j++) //Для всех частей разбиения, определить "закавыченность" запятых, "приклеивая" к первой части разбиения текущие
                    {
                         //"Приклеить" текущую строку
                        str2 += ',' + listComma[j];
                        //Если последняя запятая в строке не "закавычена" и не заключена в скобках, сохранить позицию поля как индекс текущей строки
                        if(!quotedThing(dopList, i, str2.lastIndexOf(",")) && !thingInRoundBrackets(dopList, i, str2.lastIndexOf(",")))
                            pos << i;
                    }
                }
                //Сохранить позицию поля как индекс текущей строки
                pos << i;
            }

        }
        //Иначе, если текущая строка равняется фигурной открывающейся скобке, определить конец тела
        else if(list[i] == '{')
            i = findClosingSymbol(list, i, 0, '}')[0];
        //Иначе, если последующая строка равняется фигурной открывающейся скобке, определить конец тела
        else if(list[++i].startsWith("{"))
            i = findClosingSymbol(list, i, 0, '}')[0];
    }
    //Вернуть позиции всех полей заданного файла
    return pos;
}

QList <int> findMethods(const QStringList &list, int start, int end)
{
    QList <int> pos;
    for(int i = start; i <= end; i++)   //Для всех строк кода заданного файла программы от заданного начала и конца определить позиции всех методов
    {
        //Если текущая строка содержит "незакавыченную" круглую открывающуюся скобку и перед ней нет символа "равно", определить метод
        if(list[i].contains("(") && !quotedThing(list, i, list[i].indexOf("(")) && !list[i].left(list[i].indexOf("(")).contains("="))
        {
            //Если текущая строка содержит "незакавыченное" слово "class" и последующая строка начинается символом открытия тела, определить конец тела
            if((list[i].startsWith("class ") || list[i].contains(" class ")) && !quotedThing(list, i, list[i].indexOf("class")) && list[i+1].startsWith("{"))
                i = findClosingSymbol(list, i, 0, '}')[0];
            //Иначе, если текущая строка не начинается символами открытия и закрытия тела, сохранить позицию метода как индекс текущей строки и при необходимости определить конец тела
            else if(!list[i].startsWith("{") && !list[i].startsWith("}"))
            {
                //Сохранить позицию поля как индекс текущей строки
                pos << i;
                //Если последующая строка начинается символом открытия тела и текущая строка не заканчивается символом пустого оператора, определить конец тела
                if(list[i+1].startsWith("{") && !list[i].endsWith(";"))
                {
                    i = findClosingSymbol(list, i+1, 0, '}')[0];
                }
            }
        }
        //Иначе, если текущая строка содержит "незакавыченное" слово "class" и последующая строка начинается символом открытия тела, определить конец тела
        else if((list[i].startsWith("class ") || list[i].contains(" class ")) && !quotedThing(list, i, list[i].indexOf("class")) && list[++i].startsWith("{"))
            i = findClosingSymbol(list, i, 0, '}')[0];
        //Иначе, если текущая строка равняется символу открытия тела, сохранить позицию метода как индекс текущей строки и определить конец тела
        else if(list[i] == '{')
        {
            //Сохранить позицию поля как индекс текущей строки
            pos << i;
            //Определить конец тела
            i = findClosingSymbol(list, i, 0, '}')[0];
        }
    }
    //Вернуть позиции всех методов заданного файла
    return pos;
}

void parsingFields(const QStringList &list, QList <int> &listFields, QList <dataWrittenField> &dataFields)
{
    for(int j = 0; j < listFields.size()-1; j++) //Для всех заданных позиций полей, удалить повторяющиеся
        if(listFields[j] == listFields[j+1])
        {
            listFields.removeAt(j+1);
            j--;
        }
    for(int i = 0; i < listFields.size(); i++) //Для всех заданных позиций полей, провести их "парсинг"
    {
        //Объявить структуру для хранения текущего поля
        dataWrittenField dataField;
        //Создать копию текущей строки
        QString str = list[listFields[i]];
        //Вставить перед и после следующих символов: "равно", открытые и закрытые угловые, квадратные скобки, "запятая" пробельные символы
        str = str.replace("=", " = ");
        str = str.replace("<", " < ");
        str = str.replace(">", " > ");
        str = str.replace("[", " [ ");
        str = str.replace("]", " ] ");
        str = str.replace(",", " , ");
        //Разбить строку по пробельному символу
        QStringList listStr = str.split(" ", QString::SkipEmptyParts);
        //Вернуть строку к первоначальному виду, удалив вставленные пробельные символы
        str = str.replace(" = ", "=");
        str = str.replace(" < ", "<");
        str = str.replace(" > ", ">");
        str = str.replace(" [ ", "[");
        str = str.replace(" ] ", "]");
        str = str.replace(" , ", ",");
        //Обнулить переменную для хранения индексов составляющих частей в разбиении
        int pos = 0;
        //Считать, что значение у поля отсутствует
        bool isValue = false;
        int posStartType;
        int posEndType;
        QString value = "";
        //Пока текущая часть разбиения строки по пробельному символу содержит модификатор доступа или другой модификатор перейти к следующей
        while(listStr[pos] == "private" || listStr[pos] == "public" || listStr[pos] == "protected"
                    || listStr[pos] == "static" || listStr[pos] == "final" || listStr[pos] == "transient" || listStr[pos] == "volatile")
            pos++;
        //Сохранить позиция начала типа поля как текущий индекс составляющей части в разбиении
        posStartType = pos;
        //Сохранить позицию конца типа поля как позиция начала типа поля
        posEndType = posStartType;
        //Если последующая составляющая часть в разбиении равна открывающейся угловой скобке, переназначить позицию конца типа поля
        if(listStr[pos+1] == '<')
        {
            //Определить позицию закрывающейся угловой скобки
            pos = findClosingSymbol(listStr, pos + 1, 0, '>')[0];
            //Сохранить позицию конца типа поля как позиция найденной закрывающейся угловой скобки
            posEndType = pos;
        }
        //Иначе, если последующая составляющая часть в разбиении равна открывающейся квадратной скобке, переназначить позицию конца типа поля
        else if(listStr[pos+1] == '[')
        {
            //Увеличить индекс составляющих частей в разбиении на 2
            pos = pos + 2;
            while(listStr[pos] == '[' || listStr[pos] == ']')   //Пока текущий индекс равен открывающейся или закрывающейся квадратной скобке, инкрементировать индекс составляющих частей в разбиении
                pos++;
            //Декрементировать индекс составляющих частей в разбиении
            pos--;
            //Сохранить позицию конца типа поля как значение индекса составляющих частей в разбиении
            posEndType = pos;
        }
        //Если последующая составляющая часть в разбиении равна символу равенства, переназначить позицию конца типа поля
        if(listStr[pos+1] == "=")
        {
            //Декрементировать индекс составляющих частей в разбиении
            pos--;
            //Сохранить позицию конца типа поля как значение равное -1
            posEndType = -1;
        }
        //Определить дополнительный тип поля в случае, когда тип массива определяется после имени поля
            QString dopType = "";
            //Если позиция конца типа поля равна -1 и поле заканчивается своим именем, определить дополнительный тип поля
            if(posEndType != -1 && posEndType + 2 < listStr.size()-1)
            {
                while(listStr[posEndType + 2] == "[" || listStr[posEndType + 2] == "]") //Пока после имени поля следуют открывающаяся и закрывающаяся квадратные скобки, вырезать составляющие части разбиения
                {
                    //Прибавить к строке дополнительного типа текущую строку составляющей части разбиения
                    dopType += listStr[posEndType + 2];
                    //Удалить текущую строку составляющей части разбиения
                    listStr.removeAt(posEndType + 2);
                }
            }
        //Определить элемент программы равный имени поля
            //Если у поля отсутствует значение, определить элемент программы равный имени поля
            if(listStr[++pos].endsWith(";"))
                dataField.nameField = listStr[pos].left(listStr[pos].length()-1);
            //Иначе, определить элемент программы равный имени поля и определить значение данного поля
            else
            {
                //Определить элемент программы как строку текущей составляющей части разбиения
                dataField.nameField = listStr[pos];
                //Считать, что значение поля присутствует
                isValue = true;
                //Если в одной строке определяется несколько полей, декрементировать позицию составляющих частей в разбиении
                if(listStr[pos+1] == ",")
                    pos--;
                for(int j = pos+2; j < listStr.size(); j++) //Для всех частей разбиения от первого имени поля, выделить все части до конца
                    value += listStr[j];
                //Вернуть выделенную строку к первоначальному виду, удалив вставленные пробельные символы
                value = value.simplified();
                value = value.replace(" < ", "<");
                value = value.replace(" > ", ">");
                value = value.replace(" [ ", "[");
                value = value.replace(" ] ", "]");
            }
        //Выделить первоначальный тип поля
            QString type;
            for(int j = posStartType; j <= posEndType; j++) //Для всех составляющих частей разбиения от индекса начала типа до индекса конца типа, склеить составляющие части
                type += listStr[j] + " ";
            //Вернуть склеенную  строку к первоначальному виду, удалив вставленные пробельные символы
            type = type.replace(" < ", "<");
            type = type.replace(" > ", ">");
            type = type.replace(" [ ", "[");
            type = type.replace(" ] ", "]");
            type = type.replace(" , ", ",");
            type = type.simplified();
            //Определить элемент программы равным типу поля
            dataField.typeField = type + dopType;
        //Определить модификаторы доступа и другие модификаторы поля
            //Обнулить индекс составляющих частей разбиения
            pos = 0;
            QString access_modifier;
            QStringList modifier;
            //Пока текущая часть разбиения строки содержит модификатор доступа или другой модификатор перейти к следующей, определяя их по виду
            while(listStr[pos] == "private" || listStr[pos] == "public" || listStr[pos] == "protected"
                        || listStr[pos] == "static" || listStr[pos] == "final" || listStr[pos] == "transient" || listStr[pos] == "volatile")
            {
                //Если текущая часть разбиения строки содержит модификатор доступа, определить его
                if(listStr[pos] == "private" || listStr[pos] == "public" || listStr[pos] == "protected")
                    dataField.accessModifier = listStr[pos];
                //Иначе, если текущая часть разбиения строки содержит другой модификатор, определить его
                else if(listStr[pos] == "static" || listStr[pos] == "final" || listStr[pos] == "transient" || listStr[pos] == "volatile")
                    dataField.otherModifier.append(listStr[pos]);
                //Инкрементировать индекс составляющих частей разбиения
                pos++;
            }
        //Если в одной строке определено несколько полей или первое поле имеет значение, определить все поля с их значениями при наличии
        if(isValue)
        {
            //Разбить выделенную строку от имени первого поля по "незакавыченным" запятым
            QStringList listVal;
            correctlyBrokenByCommas(value, listVal);
            //Если в строке определено только одно поле, определить элемент программы равный значению поля (без символа пустого оператора)
            if(listVal[0].endsWith(";"))
                dataField.valueField = listVal[0].left(listVal[0].length()-1);
            //Иначе, если определено несколько полей, определить элемент программы равный значению поля
            else if(listVal[0] != "")
                dataField.valueField = listVal[0];
            //Сохранить заполненную структуру текущего поля
            dataFields.append(dataField);
            //Если в одной строке определено несколько полей, определить все поля с их значениями при наличии
            if(listVal.size() > 1)
                for(int j = 1; j < listVal.size(); j++) //Для всех полей кроме первого определить поля
                {
                    //Определить дополнительный тип поля в случае, когда тип массива определяется после имени поля
                        dopType = "";
                        //Вставить перед и после открывающихся и закрывающихся квадратных скобок пробельные символы
                        listVal[j] = listVal[j].replace("[", " [ ");
                        listVal[j] = listVal[j].replace("]", " ] ");
                        //Разбить строку по пробельному символу
                        QStringList indexListVal = listVal[j].split(" ", QString::SkipEmptyParts);
                        //Вернуть строку к первоначальному виду, удалив вставленные пробельные символы
                        listVal[j] = listVal[j].replace(" [ ", "[");
                        listVal[j] = listVal[j].replace(" ] ", "]");
                        //Если после имени содержится дополнительный тип, выделить его
                        if(indexListVal.size() > 1)
                        {
                            while(indexListVal[1] == "[" || indexListVal[1] == "]") //Пока после имени поля следуют открывающаяся и закрывающаяся квадратные скобки, вырезать составляющие части разбиения
                            {
                                //Прибавить к строке дополнительного типа текущую строку составляющей части разбиения
                                dopType += indexListVal[1];
                                //Удалить текущую строку составляющей части разбиения
                                indexListVal.removeAt(1);
                            }
                            //Склеить оставшиеся части разбиения
                            listVal[j] = indexListVal.join(" ");
                        }
                    //Если текущее поле последнее и не содержит значения, определить элемент программы как имя текущего поля (без символа пустого оператора)
                    if(listVal[j].left(listVal[j].indexOf("=")).endsWith(";"))
                        dataField.nameField = listVal[j].left(listVal[j].indexOf("=")).left(listVal[j].left(listVal[j].indexOf("=")).length()-1).trimmed();
                    //Иначе, определить элемент программы как имя текущего поля
                    else
                        dataField.nameField = listVal[j].left(listVal[j].indexOf("=")).trimmed();
                    //Определить элемент программы равным типу текущего поля
                    dataField.typeField = type + dopType;
                    //Если текущее поле содержит значение, определить его как элемент программы
                    if(listVal[j].contains("="))
                    {
                        //Если текущая строка заканчивается символом пустого оператора, определить элемент программы равный значению поля (без символа пустого оператора)
                        if(listVal[j].mid(listVal[j].indexOf("=")+1).endsWith(";"))
                            dataField.valueField = listVal[j].mid(listVal[j].indexOf("=")+1).left(listVal[j].mid(listVal[j].indexOf("=")+1).length()-1);
                        //Иначе, определить элемент программы равный значению поля
                        else
                            dataField.valueField = listVal[j].mid(listVal[j].indexOf("=")+1);
                    }
                    //Сохранить заполненную структуру текущего поля
                    dataFields.append(dataField);
                }
        }
        //Иначе, сохранить заполненную структуру текущего поля
        else
            dataFields.append(dataField);
    }
}

void parsingMethods(const QStringList &list, const QList <int> &listMethods, QList <dataWrittenMethod> &dataMethods)
{
    for(int i = 0; i < listMethods.size(); i++) //Для всех заданных методов, провести их "парсинг"
    {
        //Объявить структуру для хранения текущего метода
        dataWrittenMethod dataMethod;
        //Если текущий метод начинается открывающейся фигурной скобкой, считать, что это блок-инициализации
        if(list[listMethods[i]] == '{')
        {
            //Обозначить, что текущий метод является блоком инициализации
            dataMethod.isInitializationBlock = true;
            //Считать блок-инициализации нестатическим
            QString modifier = "non-static";
            //Если перед началом тела блока-инициализации содержится модификатор "static", считать блок-инициализации статическим
            if(list[listMethods[i]-1] == "static")
                modifier = "static";
            //Определить атрибут программы как тип блока-инициализации
            dataMethod.typeInitializationBlock = modifier;
            //Найти конец тела блока-инициализации
            int posEnd = findClosingSymbol(list, listMethods[i], 0, '}')[0];
            QStringList listValue;
            for(int j = listMethods[i]+1; j < posEnd; j++)  //Для всех строк между началом и концом тела блока-инициализации выделить их в список
                listValue << list[j];
            //Склеить строки списка символом переноса строки
            QString value = listValue.join("\n");
            //Определить элемент программы равным значению блока-инициализации
            dataMethod.valueInitializationBlock = value;
            //Сохранить заполненную структуру текущего метода
            dataMethods.append(dataMethod);
            //Перейти к следующей итерации цикла
            continue;
        }
        //Создать копию текущей строки
        QString str = list[listMethods[i]];
        //Вставить перед и после следующих символов: открытые и закрытые угловые, квадратные, круглые скобки, "запятая" пробельные символы
        str = str.replace("<", " < ");
        str = str.replace(">", " > ");
        str = str.replace("[", " [ ");
        str = str.replace("]", " ] ");
        str = str.replace(",", " , ");
        str = str.replace("(", " ( ");
        str = str.replace(")", " ) ");
        //Разбить строку по пробельному символу
        QStringList listStr = str.split(" ", QString::SkipEmptyParts);
        //Вернуть строку к первоначальному виду, удалив вставленные пробельные символы
        str = str.replace(" < ", "<");
        str = str.replace(" > ", ">");
        str = str.replace(" [ ", "[");
        str = str.replace(" ] ", "]");
        str = str.replace(" , ", ",");
        str = str.replace(" ( ", "(");
        str = str.replace(" ) ", ")");
        //Обнулить переменную для хранения индексов составляющих частей в разбиении
        int pos = 0;
        int posStartType;
        int posEndType;
        //Пока текущая часть разбиения строки по пробельному символу содержит модификатор доступа или другой модификатор перейти к следующей
        while(listStr[pos] == "private" || listStr[pos] == "public" || listStr[pos] == "protected"
                    || listStr[pos] == "static" || listStr[pos] == "final" || listStr[pos] == "abstract" || listStr[pos] == "synchronized")
            pos++;
        //Сохранить позиция начала типа метода как текущий индекс составляющей части в разбиении
        posStartType = pos;
        //Сохранить позицию конца типа поля как позиция начала типа поля
        posEndType = posStartType;
        //Если последующая составляющая часть в разбиении равна открывающейся угловой скобке, переназначить позицию конца типа поля
        if(listStr[pos+1] == '<')
        {
            //Определить позицию закрывающейся угловой скобки
            pos = findClosingSymbol(listStr, pos + 1, 0, '>')[0];
            //Сохранить позицию конца типа поля как позиция найденной закрывающейся угловой скобки
            posEndType = pos;
        }
        //Иначе, если последующая составляющая часть в разбиении равна открывающейся квадратной скобке, переназначить позицию конца типа поля
        else if(listStr[pos+1] == '[')
        {
            //Увеличить индекс составляющих частей в разбиении на 2
            pos = pos + 2;
            while(listStr[pos] == '[' || listStr[pos] == ']')   //Пока текущий индекс равен открывающейся или закрывающейся квадратной скобке, инкрементировать индекс составляющих частей в разбиении
                pos++;
            //Декрементировать индекс составляющих частей в разбиении
            pos--;
            //Сохранить позицию конца типа поля как значение индекса составляющих частей в разбиении
            posEndType = pos;
        }

        //Выделить тип метода
            QString type;
            for(int j = posStartType; j <= posEndType; j++) //Для всех составляющих частей разбиения от индекса начала типа до индекса конца типа, склеить составляющие части
                type += listStr[j] + " ";
            //Вернуть склеенную  строку к первоначальному виду, удалив вставленные пробельные символы
            type = type.replace(" < ", "<");
            type = type.replace(" > ", ">");
            type = type.replace(" [ ", "[");
            type = type.replace(" ] ", "]");
            type = type.replace(" , ", ",");
            type = type.simplified();
            //Если последующая составляющая часть в разбиении не равна открывающейся круглой скобке, определить элемент и атрибут программы
            if(listStr[pos+1] != "(")
            {
                //Определить элемент программы равный имени метода
                dataMethod.nameMethod = listStr[pos+1];
                //Определить атрибут программы равный типу метода
                dataMethod.typeMethod = type;
            }
            //Иначе, если последующая составляющая часть в разбиении равна открывающейся круглой скобке, определить элемент программы равный имени метода
            else if(listStr[pos+1] == "(")
                dataMethod.nameMethod = listStr[pos];
        //Определить модификаторы доступа и другие модификаторы поля
            //Обнулить индекс составляющих частей разбиения
            pos = 0;
            //Пока текущая часть разбиения строки содержит модификатор доступа или другой модификатор перейти к следующей, определяя их по виду
            while(listStr[pos] == "private" || listStr[pos] == "public" || listStr[pos] == "protected"
                        || listStr[pos] == "static" || listStr[pos] == "final" || listStr[pos] == "abstract" || listStr[pos] == "synchronized")
            {
                //Если текущая часть разбиения строки содержит модификатор доступа, определить его
                if(listStr[pos] == "private" || listStr[pos] == "public" || listStr[pos] == "protected")
                    dataMethod.accessModifier = listStr[pos];
                //Иначе, если текущая часть разбиения строки содержит другой модификатор, определить его
                else if(listStr[pos] == "static" || listStr[pos] == "final" || listStr[pos] == "abstract" || listStr[pos] == "synchronized")
                    dataMethod.otherModifier.append(listStr[pos]);
                //Инкрементировать индекс составляющих частей разбиения
                pos++;
            }
        //Определить параметры текущего метода
            //Найти позицию начала параметров
            int posStartParam = list[listMethods[i]].indexOf("(");
            //Найти позицию конца параметров
            int posEndParam = findClosingSymbol(list,listMethods[i], posStartParam, ')')[1];
            //Выделить строку с параметрами метода
            QString strParam = list[listMethods[i]].mid(posStartParam+1, posEndParam-posStartParam-1).simplified();
            //Разбить строку на отдельные параметры метода
            QStringList param;
            correctlyBrokenByCommas(strParam, param);
            for(int j = 0; j < param.size(); j++)   //Для всех параметров метода определить их как атрибуты элемента программы
                dataMethod.parameters.append(param[j].simplified());
        //Определить наличие оператора "throws" у метода
            //Выделить строку после параметров метода
            QString strThrows = list[listMethods[i]].mid(posEndParam+1).simplified();
            //Если выделенная строка начинается оператором "throws", определить его как атрибут элемента программы
            if(strThrows.startsWith("throws "))
                dataMethod.throwsValue = strThrows.mid(QString("throws").length()+1);
        //Определить тело метода
            //Если заголовок метода не является прототипом, найти начало и конец метода, определив его
            if(!list[listMethods[i]].endsWith(";"))
            {
                //Найти конец тела метода
                int posEnd = findClosingSymbol(list, listMethods[i]+1, 0, '}')[0];
                QStringList listValue;
                for(int j = listMethods[i]+2; j < posEnd; j++)  //Для всех строк тела метода сохранить их в список строк
                    listValue << list[j].simplified();
                //Склеить сохраненные строки символом переноса новой строки
                QString value = listValue.join("\n");
                //Определить тело метода как значение элемента программы
                dataMethod.valueMethod = value;
            }
        //Сохранить заполненную структуру текущего метода
        dataMethods.append(dataMethod);
    }
}

void correctlyBrokenByCommas(QString value, QStringList &listVal)
{
    //Вставить перед открывающимися фигурными и угловыми скобками открывающиеся круглые скобки и после соответствующие круглые скобки
    value = value.replace("{", "({");
    value = value.replace("}", "})");
    value = value.replace("<", "(<");
    value = value.replace(">", ">)");
    //Определить кол-во запятых в строке
    int countComma = value.count(",");
    //Считать переменную для хранения индекса запятых равным 0
    int pos = 0;
    for(int j = 1; j <= countComma; j++)    //Для всего кол-ва запятых определить их "незакавыченность" и заключенность в скобки
    {
        //Определить индекс текущей запятой в строке
        pos += value.mid(pos).indexOf(",");
        //Если найденная запятая не "закавычена" и не заключена в скобки, вставить после найденной запятая символ переноса новой строки
        if(!quotedThing((QStringList)value, 0, pos) && !thingInRoundBrackets((QStringList)value, 0, pos))
            value.insert(pos+1, '\n');
        //Инкрементировать индекс запятых
        pos++;
    }
    //Вернуть строку к первоначальному виду, удалив вставленные круглые скобки
    value = value.replace("({", "{");
    value = value.replace("})", "}");
    value = value.replace("(<", "<");
    value = value.replace(">)", ">");
    //Разбить строку по запятой с символом переноса новой строки
    listVal = value.split(",\n");
}

void writeToXML(const QList <dataWrittenClass> &dataXML, const QStringList &imports)
{
    //Создать файл для записи данных (классов) в формате XML и определить класс для данной реализации
    QFile file("xml.txt");
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    //Записать данные в формате XML
        //Начать документ
        xmlWriter.writeStartDocument();
            //Определить корневой элемент программы
            xmlWriter.writeStartElement("program");
            //Записать атрибут кол-ва классов
            xmlWriter.writeAttribute("countClass", QString::number(dataXML.size()));
            for(int i = 0; i < dataXML.size(); i++) //Для всех синтаксически разложенных классов записать их в формате XML
                writeClassToXML(dataXML[i], xmlWriter);
            //Закрыть корневой элемент программы
            xmlWriter.writeEndElement();
        //Закончить документ
        xmlWriter.writeEndDocument();
    //Закрыть файл
    file.close();
    //Создать файл для записи данных (импортов) в формате XML и определить класс для данной реализации
    QFile file2("imports.txt");
    file2.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter2(&file2);
    xmlWriter2.setAutoFormatting(true);
    //Записать данные в формате XML
        //Начать документ
        xmlWriter2.writeStartDocument();
            //Определить корневой элемент программы
            xmlWriter2.writeStartElement("import");
            for(int i = 0; i < imports.size(); i++) //Для всех выделенных "импортов" записать их в формате XML
            {
                //Определить элемент программы равный названию импорта
                xmlWriter2.writeStartElement(imports[i]);
                //Закрыть элемент импорта
                xmlWriter2.writeEndElement();
            }
            //Закрыть коревой элемент программы
            xmlWriter2.writeEndElement();
        //Закончить документ
        xmlWriter2.writeEndDocument();
    //Закрыть файл
    file2.close();
}

void writeClassToXML(const dataWrittenClass &dataClass, QXmlStreamWriter &xmlWriter)
{
    //Определить элемент программы, содержащий все найденные классы
    xmlWriter.writeStartElement(dataClass.nameClass);
    //Если поле "implements" непустое, определить его как атрибут элемент программы
    if(!dataClass.implementsValue.isNull())
        xmlWriter.writeAttribute("implements", dataClass.implementsValue);
    //Иначе, если поле "extends" непустое, определить его как атрибут элемент программы
    else if(!dataClass.extendsValue.isNull())
        xmlWriter.writeAttribute("extends", dataClass.extendsValue);
    //Определить атрибут, содержащий кол-во вложенных классов
    xmlWriter.writeAttribute("countNestedClass", QString::number(dataClass.nestedClasses.size()));
    //Определить элемент программы, содержащий все найденные поля текущего класса
    xmlWriter.writeStartElement("fields");
    //Определить атрибут, содержащий кол-во полей текущего класса
    xmlWriter.writeAttribute("count", QString::number(dataClass.fields.size()));
    for(int j = 0; j < dataClass.fields.size(); j++)    //Для всех синтаксически разложенных полей определить их составляющие
    {
        //Определить элемент программы равный имени поля
        xmlWriter.writeStartElement(dataClass.fields[j].nameField);
        //Определить элемент программы равным типу поля
        xmlWriter.writeAttribute("type", dataClass.fields[j].typeField);
        //Определить элементы программы равные модификатору доступа поля и всем другим модификаторам
            //Если в строке определен модификатор доступа, определить его как элемент программы
            if(!dataClass.fields[j].accessModifier.isNull())
                xmlWriter.writeAttribute("access_modifier", dataClass.fields[j].accessModifier);
            for(int k = 0; k < dataClass.fields[j].otherModifier.size(); k++)   //Для всех других модификаторов провести их определение
                xmlWriter.writeAttribute("modifier", dataClass.fields[j].otherModifier[k]);
        //Определить элемент программы равный значению поля (без символа пустого оператора)
        xmlWriter.writeCharacters(dataClass.fields[j].valueField);
        //Закрыть элемент программы равный значению поля
        xmlWriter.writeEndElement();
    }
    //Закрыть элемент программы, содержащий все найденные поля текущего класса
    xmlWriter.writeEndElement();
    //Определить элемент программы, содержащий все найденные методы текущего класса
    xmlWriter.writeStartElement("methods");
    //Определить атрибут, содержащий кол-во методов текущего класса
    xmlWriter.writeAttribute("count", QString::number(dataClass.methods.size()));
    for(int j = 0; j < dataClass.methods.size(); j++)   //Для всех синтаксически разложенных методов определить их составляющие
    {
        //Если текущий метод является блоком инициализации, определить его и перейти на следующую итерацию
        if(dataClass.methods[j].isInitializationBlock)
        {
            //Определить элемент программы как имя для блока-инициализации
            xmlWriter.writeStartElement("initialization-block");
            //Определить атрибут программы как тип блока-инициализации
            xmlWriter.writeAttribute("type", dataClass.methods[j].typeInitializationBlock);
            //Определить элемент программы равным значению блока-инициализации
            xmlWriter.writeCharacters(dataClass.methods[j].valueInitializationBlock);
            //Закрыть элемент программы равный имени для блока-инициализации
            xmlWriter.writeEndElement();
            continue;
        }
        //Определить элемент программы равный имени метода
        xmlWriter.writeStartElement(dataClass.methods[j].nameMethod);
        //Определить атрибут программы равный типу метода
        if(!dataClass.methods[j].typeMethod.isNull())
            xmlWriter.writeAttribute("type", dataClass.methods[j].typeMethod);
        //Определить элементы программы равные модификатору доступа поля и всем другим модификаторам
            //Если в строке определен модификатор доступа, определить его как элемент программы
            if(!dataClass.methods[j].accessModifier.isNull())
                xmlWriter.writeAttribute("access_modifier", dataClass.methods[j].accessModifier);
            for(int k = 0; k < dataClass.methods[j].otherModifier.size(); k++)  //Для всех других модификаторов провести их определение
                xmlWriter.writeAttribute("modifier", dataClass.methods[j].otherModifier[k]);
        for(int k = 0; k < dataClass.methods[j].parameters.size(); k++)   //Для всех параметров метода определить их как атрибуты элемента программы
            xmlWriter.writeAttribute("param", dataClass.methods[j].parameters[k]);
        //Если в строке определен оператор "throws", определить его как атрибут элемента программы
        if(!dataClass.methods[j].throwsValue.isNull())
            xmlWriter.writeAttribute("throws", dataClass.methods[j].throwsValue);
        //Определить тело метода как значение элемента программы
        xmlWriter.writeCharacters(dataClass.methods[j].valueMethod);
        //Закрыть элемент программы равный имени метода
        xmlWriter.writeEndElement();
    }
    //Закрыть элемент программы, содержащий все найденные методы текущего класса
    xmlWriter.writeEndElement();
    for(int j = 0; j < dataClass.nestedClasses.size(); j++) //Для всех найденных вложенных классов провести их "парсинг"
        writeClassToXML(dataClass.nestedClasses[j], xmlWriter);
    //Закрыть элемент программы, содержащий все найденные классы
    xmlWriter.writeEndElement();
}
