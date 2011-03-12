/*  SuplChecker - simple program to check a teacher's absencies at the school
    Copyright (C) 2010-2011  nowrep

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef NASTAVENI_H
#define NASTAVENI_H

#include <QWidget>
#include <QMessageBox>
#include <QFile>
#include <QDebug>

namespace Ui {
    class Settings;
}

class SuplChecker;
class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(SuplChecker* mainWindow,QWidget *parent = 0);
    ~Settings();

private:
    Ui::Settings *ui;
    SuplChecker* m_mainWindow;

private slots:
    void save();
};

#endif // NASTAVENI_H
