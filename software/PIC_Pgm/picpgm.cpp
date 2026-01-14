#include "picpgm.h"
#include "ui_picpgm.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDomDocument>
#include <QThread>


//Переменные и объекты
QSerialPort serial;
bool isOpen = false;
//Массивы для пакетов приёма отправки
char *inBuf;
char *outBuf;
int w_index = 0;
//
QStringList digs;
//
char *common_buff;
char *codeR_buf;
char *codeW_buf;
char *dataR_buf;
char *dataW_buf;

//
bool codeCompareEnable = false;
bool dataCompareEnable = false;
bool writeNeeded = false;
//
int device;
int f_size;
int f_psize;
int ee_size;
int f_begin;
int ee_begin;
int config1;
int config2;
int config3;
int config4;
int config5;
int config6;
int config7;
int oscal1;
int oscal2;
int oscal3;
QString devId;
int wr_delay;
int code_word;
int ee_type;

//
int main_counter;


PicPgm::PicPgm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PicPgm)
{
    ui->setupUi(this);

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
#ifdef Q_OS_LINUX
        ui->portCB->addItem("/dev/" + serialPortInfo.portName());
#else
        ui->portCB->addItem(serialPortInfo.portName());
#endif
    }

    inBuf = new char[250];
    outBuf = new char[250];

    digs << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9";
    digs << "A" << "B" << "C" << "D" << "E" << "F";

    //Чтение сохранённой модели пика
    QFile fileR("settings.txt");
    QString str = "";
    if ((fileR.exists()) && fileR.open(QIODevice::ReadOnly))
    {
        str = fileR.readLine();
        fileR.close();
    }

    addModels();
    addParams(ui->devCB->currentText());

    if(str != "") ui->devCB->setCurrentText(str);

}

PicPgm::~PicPgm()
{
    delete ui;
}

QString PicPgm::print_array(char *arr, int count)
{
    QString rd;
    QString tmp;

    rd = "OFFSET   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\r\n\r\n";


    for (int m = 0; m < count; m += 16)
    {
        tmp = "0";
        tmp += hbyteToString(m >> 16 & 0xFF);
        tmp += hbyteToString(m >> 8 & 0xFF);
        tmp += hbyteToString(m & 0xFF) + "  ";

        for (int n = 0; n < 16; n++)
        {
            tmp += hbyteToString(arr[m + n]) + " ";
        }
        tmp += "\r\n";
        rd += tmp;
    }

    return rd;
}

char PicPgm::calcCRC(QString hx)
{
    int ch_sum = 0;
    QString tmp = hx.mid(1, (hx.length() - 1));
    for(int i = 0; i < tmp.length(); i += 2)
    {
        ch_sum += (int)strToByte(tmp.mid(i, 2));
    }
    ch_sum &= 0xFF;
    return (char)((256 - ch_sum) & 0xFF);
}

//Перевод Str->Hex, Hex->Str
uchar PicPgm::strToByte(QString hStr)
{
    uchar tmp = 0x00;

    if(hStr[0] == '0') tmp = 0x00;
    else if(hStr[0] == '1') tmp = 0x10;
    else if(hStr[0] == '2') tmp = 0x20;
    else if(hStr[0] == '3') tmp = 0x30;
    else if(hStr[0] == '4') tmp = 0x40;
    else if(hStr[0] == '5') tmp = 0x50;
    else if(hStr[0] == '6') tmp = 0x60;
    else if(hStr[0] == '7') tmp = 0x70;
    else if(hStr[0] == '8') tmp = 0x80;
    else if(hStr[0] == '9') tmp = 0x90;
    else if(hStr[0] == 'a') tmp = 0xA0;
    else if(hStr[0] == 'A') tmp = 0xA0;
    else if(hStr[0] == 'b') tmp = 0xB0;
    else if(hStr[0] == 'B') tmp = 0xB0;
    else if(hStr[0] == 'c') tmp = 0xC0;
    else if(hStr[0] == 'C') tmp = 0xC0;
    else if(hStr[0] == 'd') tmp = 0xD0;
    else if(hStr[0] == 'D') tmp = 0xD0;
    else if(hStr[0] == 'e') tmp = 0xE0;
    else if(hStr[0] == 'E') tmp = 0xE0;
    else if(hStr[0] == 'f') tmp = 0xF0;
    else if(hStr[0] == 'F') tmp = 0xF0;
    else tmp = 0x00;

    if(hStr[1] == '0') tmp |= 0x00;
    else if(hStr[1] == '1') tmp |= 0x01;
    else if(hStr[1] == '2') tmp |= 0x02;
    else if(hStr[1] == '3') tmp |= 0x03;
    else if(hStr[1] == '4') tmp |= 0x04;
    else if(hStr[1] == '5') tmp |= 0x05;
    else if(hStr[1] == '6') tmp |= 0x06;
    else if(hStr[1] == '7') tmp |= 0x07;
    else if(hStr[1] == '8') tmp |= 0x08;
    else if(hStr[1] == '9') tmp |= 0x09;
    else if(hStr[1] == 'a') tmp |= 0x0A;
    else if(hStr[1] == 'A') tmp |= 0x0A;
    else if(hStr[1] == 'b') tmp |= 0x0B;
    else if(hStr[1] == 'B') tmp |= 0x0B;
    else if(hStr[1] == 'c') tmp |= 0x0C;
    else if(hStr[1] == 'C') tmp |= 0x0C;
    else if(hStr[1] == 'd') tmp |= 0x0D;
    else if(hStr[1] == 'D') tmp |= 0x0D;
    else if(hStr[1] == 'e') tmp |= 0x0E;
    else if(hStr[1] == 'E') tmp |= 0x0E;
    else if(hStr[1] == 'f') tmp |= 0x0F;
    else if(hStr[1] == 'F') tmp |= 0x0F;
    else tmp |= 0x00;

    return tmp;
}

//Перевод байта в строку (HEX без "0x")
QString PicPgm::hbyteToString(uchar hData)
{
    QString result = "XY";
    result = digs.at(hData >> 4) + digs.at(hData & 0xF);
    return result;
}


//Работа с xml
void PicPgm::addModels()
{
    QFile file("pic.xml");
    if(file.exists())
    {
        QDomDocument doc( "pic" );
        if( !file.open( QIODevice::ReadOnly ) ) return;
        if( !doc.setContent( &file ) )
        {
            file.close();
            return;
        }
        file.close();
        QDomElement root = doc.documentElement();
        QDomNode n = root.firstChild();
        while( !n.isNull() )
        {
            QDomElement e = n.toElement();
            if( !e.isNull() )
            {
                ui->devCB->addItem(e.attribute("model")); //this gives you the name of the tag
            }
            n = n.nextSibling();
        }
    }
    else
    {
        return;
    }
}


void PicPgm::addParams(QString pname)
{
    QString eebegin;
    QString Conf1;
    QString Conf2;
    QString Conf3;
    QString Conf4;
    QString Conf5;
    QString Conf6;
    QString Conf7;
    QString Osc1;
    QString Osc2;
    QString Osc3;
    bool okay;

    QFile file("pic.xml");
    if(file.exists())
    {
        QDomDocument doc( "pic" );
        if( !file.open( QIODevice::ReadOnly ) ) return;
        if( !doc.setContent( &file ) )
        {
            file.close();
            return;
        }
        file.close();
        QDomElement root = doc.documentElement();
        QDomNode n = root.firstChild();
        while( !n.isNull() )
        {
            QDomElement e = n.toElement();
            if( !e.isNull() )
            {
                if(e.attribute("model") == pname) //Проходимся по его элементам
                {
                    devId = e.namedItem("id").toElement().text();
                    device = e.namedItem("type").toElement().text().toInt();
                    f_size = e.namedItem("f_size").toElement().text().toInt();
                    f_psize = e.namedItem("f_psize").toElement().text().toInt();
                    ee_size = e.namedItem("ee_size").toElement().text().toInt();
                    f_begin = e.namedItem("f_start").toElement().text().toInt();
                    eebegin = e.namedItem("ee_start").toElement().text();
                    ee_type = e.namedItem("ee_type").toElement().text().toInt();
                    Conf1 = e.namedItem("conf1").toElement().text();
                    Conf2 = e.namedItem("conf2").toElement().text();
                    Conf3 = e.namedItem("conf3").toElement().text();
                    Conf4 = e.namedItem("conf4").toElement().text();
                    Conf5 = e.namedItem("conf5").toElement().text();
                    Conf6 = e.namedItem("conf6").toElement().text();
                    Conf7 = e.namedItem("conf7").toElement().text();
                    Osc1 = e.namedItem("oscal1").toElement().text();
                    Osc2 = e.namedItem("oscal2").toElement().text();
                    Osc3 = e.namedItem("oscal3").toElement().text();
                    wr_delay = e.namedItem("delay").toElement().text().toInt();
                    code_word = e.namedItem("word").toElement().text().toInt(&okay, 16);
                }
            }
            n = n.nextSibling();
        }
    }
    else
    {
        return;
    }
    if(eebegin != "0") ee_begin = eebegin.toInt(&okay, 16) * 2;

    if (Conf1 != "none")
    {
        config1 = Conf1.toInt(&okay, 16) * 2;
        ui->conf1TB->setEnabled(true);
    }
    else
    {
        ui->conf1TB->setText("---");
        ui->conf1TB->setEnabled(false);
        config1 = -1;
    }

    if (Conf2 != "none")
    {
        config2 = Conf2.toInt(&okay, 16) * 2;
        ui->conf2TB->setEnabled(true);
    }
    else
    {
        ui->conf2TB->setText("---");
        ui->conf2TB->setEnabled(false);
        config2 = -1;
    }

    if (Conf3 != "none")
    {
        config3 = Conf3.toInt(&okay, 16) * 2;
        ui->conf3TB->setEnabled(true);
    }
    else
    {
        ui->conf3TB->setText("---");
        ui->conf3TB->setEnabled(false);
        config3 = -1;
    }

    if (Conf4 != "none")
    {
        config4 = Conf4.toInt(&okay, 16) * 2;
        ui->conf4TB->setEnabled(true);
    }
    else
    {
        ui->conf4TB->setText("---");
        ui->conf4TB->setEnabled(false);
        config4 = -1;
    }

    if (Conf5 != "none")
    {
        config5 = Conf5.toInt(&okay, 16) * 2;
        ui->conf5TB->setEnabled(true);
    }
    else
    {
        ui->conf5TB->setText("---");
        ui->conf5TB->setEnabled(false);
        config5 = -1;
    }

    if (Conf6 != "none")
    {
        config6 = Conf6.toInt(&okay, 16) * 2;
        ui->conf6TB->setEnabled(true);
    }
    else
    {
        ui->conf6TB->setText("---");
        ui->conf6TB->setEnabled(false);
        config6 = -1;
    }

    if (Conf7 != "none")
    {
        config7 = Conf7.toInt(&okay, 16) * 2;
        ui->conf7TB->setEnabled(true);
    }
    else
    {
        ui->conf7TB->setText("---");
        ui->conf7TB->setEnabled(false);
        config7 = -1;
    }

    if (Osc1 != "none")
    {
        oscal1 = Osc1.toInt(&okay, 16) * 2;
        ui->osc1TB->setEnabled(true);
    }
    else
    {
        ui->osc1TB->setText("---");
        ui->osc1TB->setEnabled(false);
        oscal1 = -1;
    }

    if (Osc2 != "none")
    {
        oscal2 = Osc2.toInt(&okay, 16) * 2;
        ui->osc2TB->setEnabled(true);
    }
    else
    {
        ui->osc2TB->setText("---");
        ui->osc2TB->setEnabled(false);
        oscal2 = -1;
    }

    if (Osc3 != "none")
    {
        oscal3 = Osc3.toInt(&okay, 16) * 2;
        ui->osc3TB->setEnabled(true);
    }
    else
    {
        ui->osc3TB->setText("---");
        ui->osc3TB->setEnabled(false);
        oscal3 = -1;
    }
}

//Scan port button
void PicPgm::on_scanBTN_clicked()
{
    ui->portCB->clear();

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
#ifdef Q_OS_LINUX
        ui->portCB->addItem("/dev/" + serialPortInfo.portName());
#else
        ui->portCB->addItem(serialPortInfo.portName());
#endif
    }
}

//Open port button
void PicPgm::on_openBTN_clicked()
{
    if(isOpen) //Close port
    {
        serial.clearError();
        serial.clear();
        serial.close();
        isOpen = false;
        ui->openBTN->setText("Open");
        ui->consoleTB->setText("Port closed");
    }
    else //Open port
    {
        if(ui->portCB->currentText() != "")
        {
            serial.setPortName(ui->portCB->currentText());
            serial.setBaudRate(QSerialPort::Baud57600);
            serial.setDataBits(QSerialPort::Data8);
            serial.setParity(QSerialPort::NoParity);
            serial.setStopBits(QSerialPort::OneStop);
            serial.setFlowControl(QSerialPort::NoFlowControl);
            //serial.setReadBufferSize(1048576);
            serial.open(QSerialPort::ReadWrite);
            isOpen = true;

            ui->openBTN->setText("Close");
            ui->consoleTB->setText("Port open");

            connect(&serial, SIGNAL(readyRead()), this, SLOT(readInBuff()));
        }
        else
        {
            ui->consoleTB->setText("Choose Port num and BaudeRate");
        }
    }
}


void PicPgm::on_checkBTN_clicked()
{
    if(isOpen)
    {
        outBuf[0] = 0xFE;
        outBuf[1] = 0x01;
        outBuf[2] = 0xCC;
        outBuf[3] = 0x00;
        serial.write(outBuf, 4);
    }
    else
    {
        ui->consoleTB->setText("Port clossed");
    }
}

//Выбор модели PIC
void PicPgm::on_devCB_currentTextChanged(const QString &arg1)
{
    addParams(arg1);
    //Запись выбранного пика в настройки
    QFile file("settings.txt");
    if (file.open(QIODevice::WriteOnly))
    {
        QString str = ui->devCB->currentText();
        file.write(str.toUtf8());
        file.close();
    }
}


//Событие по приёму по UART
void PicPgm::readInBuff()
{
    char *tbuf;
    int bufSize;
    tbuf = new char[256];

    bufSize = serial.read(tbuf, 200);
    for(int i = 0; i < bufSize; i++) inBuf[i + w_index] = tbuf[i];
    w_index += bufSize;
    if(w_index > 2 && w_index >= (static_cast<unsigned int>(inBuf[1] & 0x000000FF) + 3))
    {
        w_index = 0;
        deassemble_frame();
    }
}


//Load File
void PicPgm::on_loadBTN_clicked()
{
    QString rd;
    int fl_size;

    QString file_name = QFileDialog :: getOpenFileName (NULL, "Title", ".", "*hex");
    QFile rFile(file_name);

    if(!rFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->consoleTB->setText("Open File Error");
        return;
    }
    else
    {
        //Открываем файл для чтения
        QTextStream in(&rFile);
        int offset = 0, bofs = 0;
        int curr_offset = 0;
        int tmp_offs = 0;
        QString rds;
        fl_size = 0;

        //Рассчитываем дополнительный адрес смещения
        while (!in.atEnd())
        {
            rds = in.readLine();

            if (rds.mid(7, 2) == "04")
            {
                tmp_offs = (int)((strToByte(rds.mid(9, 2)) << 8) | strToByte(rds.mid(11, 2)));
                tmp_offs = tmp_offs << 16;
            }
            if (rds.mid(7, 2) == "00")
            {
                curr_offset = (int)((strToByte(rds.mid(3, 2)) << 8) | strToByte(rds.mid(5, 2)));
                curr_offset = curr_offset | tmp_offs;
                if (curr_offset >= offset)
                {
                    offset = curr_offset;
                    bofs = (int)(strToByte(rds.mid(1, 2)));
                }
            }
        }
        in.seek(0);

        fl_size = offset + bofs;
        if ((fl_size % 16) > 0) fl_size = fl_size + (16 - (fl_size % 16));

        //Конвертируем HEX
        common_buff = new char[fl_size];

        for (int i = 0; i < fl_size; i++) common_buff[i] = 0xFF;

        offset = 0;

        while (!in.atEnd())
        {
            rds = in.readLine();

            if (rds.mid(7, 2) == "04")
            {
                offset = (int)(((strToByte(rds.mid(9, 2)) << 8) | (strToByte(rds.mid(11, 2)))));
            }
            if (rds.mid(7, 2) == "00")
            {

                char str_size = strToByte(rds.mid(1, 2));
                int arr_offset = (int)(strToByte(rds.mid(5, 2)) | (strToByte(rds.mid(3, 2)) << 8));
                arr_offset |= (offset << 16);
                rds = rds.mid(9, str_size * 2);

                for (int h = 0; h < str_size; h++)
                {
                    common_buff[arr_offset + h] = strToByte(rds.mid(h * 2, 2));
                }
            }
        }
        in.reset();

        if(fl_size < f_size)
        {
            int new_sz = 0;
            char *tmp_arr;
            new_sz = config1 + 31;
            tmp_arr = new char[new_sz];
            for(int i = 0; i < new_sz; i++) tmp_arr[i] = 0xFF;
            for (int i = 0; i < fl_size; i++) tmp_arr[i] = common_buff[i];
            common_buff = new char[new_sz];
            for (int i = 0; i < new_sz; i++) common_buff[i] = tmp_arr[i];
            fl_size = new_sz;
        }

        char wrd = (char)(code_word >> 8);
        codeW_buf = new char[f_size];

        for (int i = 0; i < f_size; i += 2)
        {
            codeW_buf[i] = common_buff[i + f_begin];
            codeW_buf[i + 1] = (common_buff[i + f_begin + 1] & wrd);
        }
        ui->codeTB->setText(print_array(codeW_buf, f_size));

        if(config1 != -1)
        {
            ui->conf1TB->setText(hbyteToString(common_buff[config1 + 1]) + hbyteToString(common_buff[config1]));
        }
        if (config2 != -1)
        {
            ui->conf2TB->setText(hbyteToString(common_buff[config2 + 1]) + hbyteToString(common_buff[config2]));
        }
        if (config3 != -1)
        {
            ui->conf3TB->setText(hbyteToString(common_buff[config3 + 1]) + hbyteToString(common_buff[config3]));
        }
        if (config4 != -1)
        {
            ui->conf4TB->setText(hbyteToString(common_buff[config4 + 1]) + hbyteToString(common_buff[config4]));
        }
        if (config5 != -1)
        {
            ui->conf5TB->setText(hbyteToString(common_buff[config5 + 1]) + hbyteToString(common_buff[config5]));
        }
        if (config6 != -1)
        {
            ui->conf6TB->setText(hbyteToString(common_buff[config6 + 1]) + hbyteToString(common_buff[config6]));
        }
        if (config7 != -1)
        {
            ui->conf7TB->setText(hbyteToString(common_buff[config7 + 1]) + hbyteToString(common_buff[config7]));
        }

        if (ee_size > 0)
        {
            if(fl_size < (ee_begin + ee_size * ee_type))
            {
                char *new_buff;
                int new_size = ee_begin + ee_size * ee_type;
                new_buff = new char[new_size];
                for (int i = 0; i < new_size; i++) new_buff[i] = 0xFF;
                for (int i = 0; i < fl_size; i++) new_buff[i] = common_buff[i];
                common_buff = new char[new_size];
                for (int i = 0; i < new_size; i++) common_buff[i] = new_buff[i];
            }
            dataW_buf = new char[ee_size];
            if(ee_type == 2)
            {
                for (int i = 0; i < ee_size * 2; i+= 2)
                {
                    dataW_buf[i / 2] = common_buff[i + ee_begin];
                }
            }
            if (ee_type == 1)
            {
                for (int i = 0; i < ee_size; i ++)
                {
                    dataW_buf[i] = common_buff[i + ee_begin];
                }
            }

            ui->eepromTB->setText(print_array(dataW_buf, ee_size));
        }
    }
}

//Save File
void PicPgm::on_saveBTN_clicked()
{
    if(codeR_buf != NULL)
    {
        QString tmp, tmp1 = "";
        bool ext_add = false;
        int ff_count = 0;

        //Get the path of specified file
        QString filePath = QFileDialog :: getSaveFileName (NULL, "Title", ".", "*hex");
        QFile rFile(filePath);

        //Создаём пустой файл
        if(!rFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            ui->consoleTB->setText("Save File Error");
            return;
        }

        QTextStream wr(&rFile);

        tmp = ":020000040000FA";
        wr << tmp << "\n";

        for (int i = 0; i < f_size; i += 16)
        {
            //Проверяем на пустую строку
            ff_count = 0;
            for(int k = 0; k < 16; k++) if (codeR_buf[k + i] == 0xFF) ff_count++;
            if(ff_count == 16) continue;

            //Добавляем расширенный адрес
            if(i > 65535 && !ext_add)
            {
                tmp = ":02000004" + hbyteToString((char)((i >> 24) & 0xFF)) + hbyteToString((char)((i >> 16) & 0xFF));
                tmp += hbyteToString(calcCRC(tmp));
                ext_add = true;
                wr << tmp << "\n";
            }
            //Пишем строку
            tmp = ":10" + hbyteToString((char)((i >> 8) & 0xFF)) + hbyteToString((char)(i  & 0xFF)) + "00";
            for (int k = 0; k < 16; k++) tmp += hbyteToString(codeR_buf[k + i]);
            tmp += hbyteToString(calcCRC(tmp));
            wr << tmp << "\n";
        }
        //Config words
        if(config1 != -1)
        {
            if (config1 > 65535)
            {
                tmp = ":02000004" + hbyteToString((char)((config1 >> 24) & 0xFF)) +
                      hbyteToString((char)((config1 >> 16) & 0xFF));
                tmp += hbyteToString(calcCRC(tmp));
                wr << tmp << "\n";
            }
            tmp = ":" + hbyteToString((char)((config1 >> 8) & 0xFF)) + hbyteToString((char)(config1 & 0xFF)) + "00";
            tmp += ui->conf1TB->text().mid(2, 2) + ui->conf1TB->text().mid(0, 2);
            tmp1 = "02";

            if (config2 != -1)
            {
                tmp += ui->conf2TB->text().mid(2, 2) + ui->conf2TB->text().mid(0, 2);
                tmp1 = "04";
            }
            if (config3 != -1)
            {
                tmp += ui->conf3TB->text().mid(2, 2) + ui->conf3TB->text().mid(0, 2);
                tmp1 = "06";
            }
            if (config4 != -1)
            {
                tmp += ui->conf4TB->text().mid(2, 2) + ui->conf4TB->text().mid(0, 2);
                tmp1 = "08";
            }
            if (config5 != -1)
            {
                tmp += ui->conf5TB->text().mid(2, 2) + ui->conf5TB->text().mid(0, 2);
                tmp1 = "0A";
            }
            if (config6 != -1)
            {
                tmp += ui->conf6TB->text().mid(2, 2) + ui->conf6TB->text().mid(0, 2);
                tmp1 = "0C";
            }
            if (config7 != -1)
            {
                tmp += ui->conf7TB->text().mid(2, 2) + ui->conf7TB->text().mid(0, 2);
                tmp1 = "0E";
            }

            tmp.insert(1, tmp1);
            tmp += hbyteToString(calcCRC(tmp));
            wr << tmp << "\n";
        }

        if(ee_size > 0)
        {
            if(ee_begin + ee_size * 2 > 65535)
            {
                tmp = ":02000004" + hbyteToString((char)((ee_begin >> 24) & 0xFF)) +
                      hbyteToString((char)((ee_begin >> 16) & 0xFF));
                tmp += hbyteToString(calcCRC(tmp));
                wr << tmp << '\n';
            }
            if(ee_type == 1)
            {
                for(int i = 0; i < ee_size; i+= 16)
                {
                    tmp = ":10" + hbyteToString((char)(((i + ee_begin) >> 8) & 0xFF)) +
                          hbyteToString((char)((i + ee_begin) & 0xFF)) + "00";
                    for (int k = 0; k < 16; k++)
                    {
                        tmp += hbyteToString(dataR_buf[i + k]);
                    }
                    tmp += hbyteToString(calcCRC(tmp));
                    wr << tmp << "\n";
                }
            }
            if (ee_type == 2)
            {
                for (int i = 0; i < ee_size * 2; i += 16)
                {
                    tmp = ":10" + hbyteToString((char)(((i + ee_begin) >> 8) & 0xFF)) +
                          hbyteToString((char)((i + ee_begin) & 0xFF)) + "00";
                    for (int k = 0; k < 8; k ++)
                    {
                        tmp += hbyteToString(dataR_buf[i/2 + k]);
                        tmp += "00";
                    }
                    tmp += hbyteToString(calcCRC(tmp));
                    wr << tmp;
                }
            }
        }

        //Конец файла
        tmp = ":00000001FF";
        wr << tmp << "\n";
        rFile.close();
    }
    else
    {
        ui->consoleTB->setText("Нечего сохранять(((");
    }
}

//Get ID
void PicPgm::on_getIdBTN_clicked()
{
    if (isOpen && ui->devCB->currentText() != "")
    {
        outBuf[0] = 0xFE;
        outBuf[1] = 0x01;
        outBuf[2] = 0xA0;
        outBuf[3] = (char)(device);
        serial.write(outBuf, 4);
    }
    else
    {
        ui->consoleTB->setText("Сначала выберите модель PIC");
    }
}


void PicPgm::on_readBTN_clicked()
{
    if (isOpen && ui->devCB->currentText() != "")
    {
        main_counter = 0;

        codeR_buf = new char[f_size];
        if(ee_size > 0) dataR_buf = new char[ee_size];

        outBuf[0] = 0xFE;
        outBuf[1] = 0x08;
        outBuf[2] = 0xA1;
        outBuf[3] = (char)(device); //Device

        switch (f_size)
        {
        case 128: { outBuf[4] = 0x01; break; }
        case 256: { outBuf[4] = 0x02; break; }
        case 512: { outBuf[4] = 0x03; break; }
        case 1024: { outBuf[4] = 0x04; break; }
        case 2048: { outBuf[4] = 0x05; break; }
        case 4096: { outBuf[4] = 0x06; break; }
        case 8192: { outBuf[4] = 0x07; break; }
        case 16384: { outBuf[4] = 0x08; break; }
        case 32768: { outBuf[4] = 0x09; break; }
        case 65536: { outBuf[4] = 0x0A; break; }
        case 131072: { outBuf[4] = 0x0B; break; }
        case 262144: { outBuf[4] = 0x0C; break; }
        case 49152: { outBuf[4] = 0x0D; break; }
        default: { outBuf[4] = 0x01; break; }
        }

        outBuf[7] = (char)((ee_size >> 8) & 0xFF); //EE_Size
        outBuf[8] = (char)(ee_size & 0xFF);
        if (config1 != -1) outBuf[9] = 0x01;
        if (config2 != -1) outBuf[9] = 0x02;
        if (config3 != -1) outBuf[9] = 0x03;
        if (config4 != -1) outBuf[9] = 0x04;
        if (config5 != -1) outBuf[9] = 0x05;
        if (config6 != -1) outBuf[9] = 0x06;
        if (config7 != -1) outBuf[9] = 0x07;
        if (oscal1 != -1) outBuf[10] = (char)(oscal1 & 0xFF);
        serial.write(outBuf, 11);
    }
    else
    {
        ui->consoleTB->setText("Сначала выберите модель PIC");
    }
}


void PicPgm::on_writeBTN_clicked()
{
    if (isOpen && ui->devCB->currentText() != "" && codeW_buf != NULL)
    {
        writeNeeded = true;

        ui->consoleTB->setText("Стирание...");

        outBuf[0] = 0xFE;
        outBuf[1] = 0x01;
        outBuf[2] = 0xA2;
        outBuf[3] = (char)(device); //Device
        serial.write(outBuf, 4);
    }
    else
    {
        ui->consoleTB->setText("Сначала выберите модель PIC\r\nи загрузите HEX");
    }
}


void PicPgm::on_eraseBTN_clicked()
{
    if (isOpen && ui->devCB->currentText() != "")
    {
        outBuf[0] = 0xFE;
        outBuf[1] = 0x01;
        outBuf[2] = 0xA2;
        outBuf[3] = (char)(device);
        serial.write(outBuf, 4);
    }
    else
    {
        ui->consoleTB->setText("Сначала выберите модель PIC");
    }
}


void PicPgm::on_verifyBTN_clicked()
{
    if(codeCompareEnable && codeW_buf != NULL)
    {
        int code_err = 0;
        for(int i = 0; i < f_size; i++)
        {
            if(codeR_buf[i] != codeW_buf[i]) code_err++;
        }
        if(code_err > 0) ui->consoleTB->setText(QString::number(code_err) + " ошибок CODE\r\n");
        ui->consoleTB->setText("Ошибок CODE нет\r\n");
        codeCompareEnable = false;
        ui->consoleTB->moveCursor(QTextCursor::End);

        if (dataCompareEnable && dataW_buf != NULL)
        {
            int data_err = 0;
            for (int i = 0; i < ee_size; i++)
            {
                if (dataR_buf[i] != dataW_buf[i]) data_err++;
            }
            if (data_err > 0) ui->consoleTB->insertPlainText(QString::number(data_err) + " ошибок DATA");
            else ui->consoleTB->insertPlainText("Ошибок DATA нет");
            codeCompareEnable = false;
        }
    }
    else
    {
        ui->consoleTB->setText("Нечего сравнивать(((");
    }
}


void PicPgm::on_rdCfgBTN_clicked()
{
    if (isOpen && ui->devCB->currentText() != "")
    {
        outBuf[0] = 0xFE;
        outBuf[1] = 0x02;
        outBuf[2] = 0xA3;
        outBuf[3] = (char)(device); //Device
        if (config1 != -1) outBuf[4] = 0x01;
        if (config2 != -1) outBuf[4] = 0x02;
        if (config3 != -1) outBuf[4] = 0x03;
        if (config4 != -1) outBuf[4] = 0x04;
        if (config5 != -1) outBuf[4] = 0x05;
        if (config6 != -1) outBuf[4] = 0x06;
        if (config7 != -1) outBuf[4] = 0x07;
        serial.write(outBuf, 5);
    }
    else
    {
        ui->consoleTB->setText("Сначала выберите модель PIC");
    }
}


void PicPgm::on_wrCfgBTN_clicked()
{
    if (isOpen && ui->devCB->currentText() != "")
    {
        outBuf[0] = 0xFE;
        outBuf[1] = 0x10;
        outBuf[2] = 0xA4;
        outBuf[3] = (char)(device); //Device
        if(config1 != -1)
        {
            outBuf[4] = strToByte(ui->conf1TB->text().mid(2, 2));
            outBuf[5] = strToByte(ui->conf1TB->text().mid(0, 2));
            outBuf[18] = 0x01;
        }
        if (config2 != -1)
        {
            outBuf[6] = strToByte(ui->conf2TB->text().mid(2, 2));
            outBuf[7] = strToByte(ui->conf2TB->text().mid(0, 2));
            outBuf[18] = 0x02;
        }
        if (config3 != -1)
        {
            outBuf[8] = strToByte(ui->conf3TB->text().mid(2, 2));
            outBuf[9] = strToByte(ui->conf3TB->text().mid(0, 2));
            outBuf[18] = 0x03;
        }
        if (config4 != -1)
        {
            outBuf[10] = strToByte(ui->conf4TB->text().mid(2, 2));
            outBuf[11] = strToByte(ui->conf4TB->text().mid(0, 2));
            outBuf[18] = 0x04;
        }
        if (config5 != -1)
        {
            outBuf[12] = strToByte(ui->conf5TB->text().mid(2, 2));
            outBuf[13] = strToByte(ui->conf5TB->text().mid(0, 2));
            outBuf[18] = 0x05;
        }
        if (config6 != -1)
        {
            outBuf[14] = strToByte(ui->conf6TB->text().mid(2, 2));
            outBuf[15] = strToByte(ui->conf6TB->text().mid(0, 2));
            outBuf[18] = 0x06;
        }
        if (config7 != -1)
        {
            outBuf[16] = strToByte(ui->conf7TB->text().mid(2, 2));
            outBuf[17] = strToByte(ui->conf7TB->text().mid(0, 2));
            outBuf[18] = 0x07;
        }
        serial.write(outBuf, 19);
    }
    else
    {
        ui->consoleTB->setText("Сначала выберите модель PIC");
    }
}


void PicPgm::on_rdOscBTN_clicked()
{
    if (isOpen && ui->devCB->currentText() != "" && oscal1 != -1)
    {
        outBuf[0] = 0xFE;
        outBuf[1] = 0x02;
        outBuf[2] = 0xAB;
        outBuf[3] = (char)(device); //Device
        outBuf[4] = (char)(oscal1 & 0xFF);
        serial.write(outBuf, 5);
    }
    else
    {
        if (oscal1 == -1) ui->consoleTB->setText("Нет калибровочных байт");
        else ui->consoleTB->setText("Сначала выберите модель PIC");
    }
}


void PicPgm::on_wrOscBTN_clicked()
{
    if (isOpen && ui->devCB->currentText() != "" && oscal1 != -1)
    {
        outBuf[0] = 0xFE;
        outBuf[1] = 0x09;
        outBuf[2] = 0xAC;
        outBuf[3] = (char)(device); //Device
        if(oscal1 != -1)
        {
            outBuf[4] = strToByte(ui->osc1TB->text().mid(2, 2));
            outBuf[5] = strToByte(ui->osc1TB->text().mid(0, 2));
            outBuf[10] = (char)(oscal1 & 0xFF);
            outBuf[11] = 0x01;
        }
        if (oscal2 != -1)
        {
            outBuf[6] = strToByte(ui->osc2TB->text().mid(2, 2));
            outBuf[7] = strToByte(ui->osc2TB->text().mid(0, 2));
            outBuf[11] = 0x02;
        }
        if (oscal3 != -1)
        {
            outBuf[8] = strToByte(ui->osc3TB->text().mid(2, 2));
            outBuf[9] = strToByte(ui->osc3TB->text().mid(0, 2));
            outBuf[11] = 0x03;
        }

        serial.write(outBuf, 12);
    }
    else
    {
        if(oscal1 == -1) ui->consoleTB->setText("Нет калибровочных байт");
        ui->consoleTB->setText("Сначала выберите модель PIC");
    }
}

void PicPgm::deassemble_frame()
{
    if(inBuf[0] != (char)0xFE)
    {
        ui->consoleTB->setText("Frame error");
    }
    else
    {
        serial.clear();
        serial.clearError();

        switch(inBuf[2])
        {
            case (char)0xCC:
            {
                ui->consoleTB->setText("Programmer present");
                break;
            }
            case (char)0xA0:
            {
                QString tmp = "0x" + hbyteToString(inBuf[3]) + hbyteToString(inBuf[4]);
                ui->idTB->setText(tmp);
                if(devId.mid(0, 5) != tmp.mid(0, 5))
                {
                    ui->consoleTB->setText("Прочитанный ID не соответствует\r\nвыбранному МК!");
                }
                break;
            }
            case (char)0xA1:
            {
                for(int i = 0; i < 128; i+= 2)
                {
                    codeR_buf[main_counter + i] = inBuf[i + 4];
                    codeR_buf[main_counter + i + 1] = inBuf[i + 3];
                }
                main_counter += 128;

                if(main_counter >= f_size)
                {
                    main_counter = 0;
                    ui->consoleTB->setText("Чтение завершено");
                    ui->codeTB->setText(print_array(codeR_buf, f_size));
                    codeCompareEnable = true;

                    outBuf[0] = 0xFE;
                    outBuf[1] = 0x01;
                    outBuf[2] = 0x3D;
                    outBuf[3] = 0x00;
                    serial.write(outBuf, 4);
                }
                else
                {
                    ui->consoleTB->setText(QString::number(main_counter) + " байт CODE прочитано");


                    outBuf[0] = 0xFE;
                    outBuf[1] = 0x01;
                    outBuf[2] = 0x3D;
                    outBuf[3] = 0x00;
                    serial.write(outBuf, 4);
                }
                break;
            }
            case (char)0xA2:
            {
                ui->consoleTB->setText("Стирание завершено");

                if(writeNeeded)
                {
                    main_counter = 0;

                    outBuf[0] = 0xFE;
                    outBuf[1] = 0x09;
                    outBuf[2] = 0xA5; //Prepare to write
                    outBuf[3] = (char)(device); //Device
                    outBuf[4] = (char)((f_size >> 16) & 0xFF);
                    outBuf[5] = (char)((f_size >> 8) & 0xFF);
                    outBuf[6] = (char)(f_size & 0xFF);
                    outBuf[7] = (char)((ee_size >> 8) & 0xFF);
                    outBuf[8] = (char)(ee_size & 0xFF);
                    outBuf[9] = (char)((wr_delay >> 8) & 0xFF);
                    outBuf[10] = (char)(wr_delay & 0xFF);
                    outBuf[11] = (char)(f_psize);
                    serial.write(outBuf, 12);

                    writeNeeded = false;
                }
                break;
            }
            case (char)0xA3:
            {
                if (config1 != -1)
                {
                    QString tmp = hbyteToString(inBuf[3]) + hbyteToString(inBuf[4]);
                    ui->conf1TB->setText(tmp);
                }
                if (config2 != -1)
                {
                    QString tmp = hbyteToString(inBuf[5]) + hbyteToString(inBuf[6]);
                    ui->conf2TB->setText(tmp);
                }
                if (config3 != -1)
                {
                    QString tmp = hbyteToString(inBuf[7]) + hbyteToString(inBuf[8]);
                    ui->conf3TB->setText(tmp);
                }
                if (config4 != -1)
                {
                    QString tmp = hbyteToString(inBuf[9]) + hbyteToString(inBuf[10]);
                    ui->conf4TB->setText(tmp);
                }
                if (config5 != -1)
                {
                    QString tmp = hbyteToString(inBuf[11]) + hbyteToString(inBuf[12]);
                    ui->conf5TB->setText(tmp);
                }
                if (config6 != -1)
                {
                    QString tmp = hbyteToString(inBuf[13]) + hbyteToString(inBuf[14]);
                    ui->conf6TB->setText(tmp);
                }
                if (config7 != -1)
                {
                    QString tmp = hbyteToString(inBuf[15]) + hbyteToString(inBuf[16]);
                    ui->conf7TB->setText(tmp);
                }
                break;
            }
            case (char)0xA4:
            {
                ui->consoleTB->setText("Запись conf завершена");
                break;
            }
            case (char)0xA6:
            {
                if(main_counter >= f_size)
                {
                    ui->consoleTB->setText("Запись CODE завершена");
                    outBuf[0] = 0xFE;
                    outBuf[1] = 0x01;
                    outBuf[2] = 0xAF;
                    outBuf[3] = (char)(device);
                    serial.write(outBuf, 4);

                    main_counter = 0;
                    QThread::msleep(100);

                    if(ee_size > 0) //Prepare to write eeprom
                    {
                        ui->consoleTB->setText("Запись CODE завершена");
                        outBuf[0] = 0xFE;
                        outBuf[1] = 0x01;
                        outBuf[2] = 0xA7;
                        outBuf[3] = (char)(device);
                        serial.write(outBuf, 4);

                        return;
                    }

                    QThread::msleep(100);

                    if(config1 != -1)
                    {
                        outBuf[0] = 0xFE;
                        outBuf[1] = 0x10;
                        outBuf[2] = 0xA4;
                        outBuf[3] = (char)(device); //Device
                        if (config1 != -1)
                        {
                            outBuf[4] = strToByte(ui->conf1TB->text().mid(2, 2));
                            outBuf[5] = strToByte(ui->conf1TB->text().mid(0, 2));
                            outBuf[18] = 0x01;
                        }
                        if (config2 != -1)
                        {
                            outBuf[6] = strToByte(ui->conf2TB->text().mid(2, 2));
                            outBuf[7] = strToByte(ui->conf2TB->text().mid(0, 2));
                            outBuf[18] = 0x02;
                        }
                        if (config3 != -1)
                        {
                            outBuf[8] = strToByte(ui->conf3TB->text().mid(2, 2));
                            outBuf[9] = strToByte(ui->conf3TB->text().mid(0, 2));
                            outBuf[18] = 0x03;
                        }
                        if (config4 != -1)
                        {
                            outBuf[10] = strToByte(ui->conf4TB->text().mid(2, 2));
                            outBuf[11] = strToByte(ui->conf4TB->text().mid(0, 2));
                            outBuf[18] = 0x04;
                        }
                        if (config5 != -1)
                        {
                            outBuf[12] = strToByte(ui->conf5TB->text().mid(2, 2));
                            outBuf[13] = strToByte(ui->conf5TB->text().mid(0, 2));
                            outBuf[18] = 0x05;
                        }
                        if (config6 != -1)
                        {
                            outBuf[14] = strToByte(ui->conf6TB->text().mid(2, 2));
                            outBuf[15] = strToByte(ui->conf6TB->text().mid(0, 2));
                            outBuf[18] = 0x06;
                        }
                        if (config7 != -1)
                        {
                            outBuf[16] = strToByte(ui->conf7TB->text().mid(2, 2));
                            outBuf[17] = strToByte(ui->conf7TB->text().mid(0, 2));
                            outBuf[18] = 0x07;
                        }
                        serial.write(outBuf, 19);
                        return;
                    }
                    return;
                }
                else
                {
                    outBuf[0] = 0xFE;
                    outBuf[1] = 0x81;
                    outBuf[2] = 0xA6;
                    outBuf[3] = (char)(device);
                    for (int i = 0; i < 128; i++) outBuf[i + 4] = codeW_buf[i + main_counter];
                    main_counter += 128;

                    ui->consoleTB->setText(QString::number(main_counter) + " байт записано");
                    serial.write(outBuf, 132);
                }
                break;
            }
            case (char)0xA8:
            {
                if(main_counter >= ee_size)
                {
                    ui->consoleTB->setText("Запись DATA завершена");
                    outBuf[0] = 0xFE;
                    outBuf[1] = 0x01;
                    outBuf[2] = 0xAF;
                    outBuf[3] = (char)(device);
                    serial.write(outBuf, 4);

                    QThread::msleep(100);
                    main_counter = 0;

                    if (config1 != -1)
                    {
                        outBuf[0] = 0xFE;
                        outBuf[1] = 0x10;
                        outBuf[2] = 0xA4;
                        outBuf[3] = (char)(device); //Device
                        if (config1 != -1)
                        {
                            outBuf[4] = strToByte(ui->conf1TB->text().mid(2, 2));
                            outBuf[5] = strToByte(ui->conf1TB->text().mid(0, 2));
                            outBuf[18] = 0x01;
                        }
                        if (config2 != -1)
                        {
                            outBuf[6] = strToByte(ui->conf2TB->text().mid(2, 2));
                            outBuf[7] = strToByte(ui->conf2TB->text().mid(0, 2));
                            outBuf[18] = 0x02;
                        }
                        if (config3 != -1)
                        {
                            outBuf[8] = strToByte(ui->conf3TB->text().mid(2, 2));
                            outBuf[9] = strToByte(ui->conf3TB->text().mid(0, 2));
                            outBuf[18] = 0x03;
                        }
                        if (config4 != -1)
                        {
                            outBuf[10] = strToByte(ui->conf4TB->text().mid(2, 2));
                            outBuf[11] = strToByte(ui->conf4TB->text().mid(0, 2));
                            outBuf[18] = 0x04;
                        }
                        if (config5 != -1)
                        {
                            outBuf[12] = strToByte(ui->conf5TB->text().mid(2, 2));
                            outBuf[13] = strToByte(ui->conf5TB->text().mid(0, 2));
                            outBuf[18] = 0x05;
                        }
                        if (config6 != -1)
                        {
                            outBuf[14] = strToByte(ui->conf6TB->text().mid(2, 2));
                            outBuf[15] = strToByte(ui->conf6TB->text().mid(0, 2));
                            outBuf[18] = 0x06;
                        }
                        if (config7 != -1)
                        {
                            outBuf[16] = strToByte(ui->conf7TB->text().mid(2, 2));
                            outBuf[17] = strToByte(ui->conf7TB->text().mid(0, 2));
                            outBuf[18] = 0x07;
                        }
                        serial.write(outBuf, 19);

                        return;
                    }
                    return;
                }
                else
                {
                    outBuf[0] = 0xFE;
                    outBuf[1] = 0x41;
                    outBuf[2] = 0xA8;
                    outBuf[3] = (char)(device);
                    for (int i = 0; i < 64; i++) outBuf[i + 4] = dataW_buf[i + main_counter];
                    main_counter += 64;

                    ui->consoleTB->setText(QString::number(main_counter) + " байт записано");

                    serial.write(outBuf, 68);
                }
                break;
            }
            case (char)0xA9:
            {
                for (int i = 0; i < 64; i ++)
                {
                    dataR_buf[main_counter + i] = inBuf[i + 3];
                }
                main_counter += 64;

                if (main_counter >= ee_size)
                {
                    ui->consoleTB->setText("Чтение завершено");
                    ui->eepromTB->setText(print_array(dataR_buf, ee_size));

                    dataCompareEnable = true;

                    outBuf[0] = 0xFE;
                    outBuf[1] = 0x01;
                    outBuf[2] = 0x3D;
                    outBuf[3] = 0x00;
                    serial.write(outBuf, 4);
                }
                else
                {
                    ui->consoleTB->setText(QString::number(main_counter) + " байт DATA прочитано");

                    outBuf[0] = 0xFE;
                    outBuf[1] = 0x01;
                    outBuf[2] = 0x3D;
                    outBuf[3] = 0x00;
                    serial.write(outBuf, 4);
                }
                break;
            }
            case (char)0xAB:
            {
                if(oscal1 != -1)
                {
                    ui->osc1TB->setText(hbyteToString(inBuf[3]) + hbyteToString(inBuf[4]));
                }
                if (oscal2 != -1)
                {
                    ui->osc2TB->setText(hbyteToString(inBuf[5]) + hbyteToString(inBuf[6]));
                }
                if (oscal3 != -1)
                {
                    ui->osc3TB->setText(hbyteToString(inBuf[7]) + hbyteToString(inBuf[8]));
                }
                break;
            }
            case (char)0xAC:
            {
                ui->consoleTB->setText("Запись Oscal завершена");
                break;
            }
            default:
            {
                break;
            }
        }
    }
}
