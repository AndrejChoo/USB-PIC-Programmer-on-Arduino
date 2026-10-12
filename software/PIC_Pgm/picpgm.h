/*
 * Copyright (C) 2026 AndrejChoo <andrejchukov@yandex.ru>
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef PICPGM_H
#define PICPGM_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class PicPgm; }
QT_END_NAMESPACE

class PicPgm : public QMainWindow
{
    Q_OBJECT

public:
    PicPgm(QWidget *parent = nullptr);
    ~PicPgm();

    void addModels();
    void addParams(QString pname);
    void deassemble_frame();
    QString hbyteToString(uchar hData);
    uchar strToByte(QString hStr);
    QString print_array(char *arr, int count);
    char calcCRC(QString hx);


private slots:
    void on_scanBTN_clicked();

    void on_openBTN_clicked();

    void on_checkBTN_clicked();

    void on_devCB_currentTextChanged(const QString &arg1);

    void readInBuff();

    void on_loadBTN_clicked();

    void on_saveBTN_clicked();

    void on_getIdBTN_clicked();

    void on_readBTN_clicked();

    void on_writeBTN_clicked();

    void on_eraseBTN_clicked();

    void on_verifyBTN_clicked();

    void on_rdCfgBTN_clicked();

    void on_wrCfgBTN_clicked();

    void on_rdOscBTN_clicked();

    void on_wrOscBTN_clicked();

private:
    Ui::PicPgm *ui;
};
#endif // PICPGM_H
