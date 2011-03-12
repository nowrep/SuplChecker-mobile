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
#include "nastaveni.h"
#include "ui_nastaveni.h"
#include "mainwindow.h"

Settings::Settings(SuplChecker* mainWindow,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
    ,m_mainWindow(mainWindow)
{
    setAttribute(Qt::WA_DeleteOnClose);
    Q_UNUSED(mainWindow)
    ui->setupUi(this);
    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowSoftkeysVisibleHint;
    flags &= ~Qt::WindowSoftkeysRespondHint;
    setWindowFlags(flags);
#ifdef Q_OS_SYMBIAN
    showFullScreen();
#endif

    QAction* rightSoftKeyAction = new QAction("Zpět", this);
    rightSoftKeyAction->setSoftKeyRole(QAction::NegativeSoftKey);
    connect(rightSoftKeyAction, SIGNAL(triggered()), this, SLOT(close()));
    addAction(rightSoftKeyAction);

    QAction* leftSoftKeyAction = new QAction("Uložit", this);
    leftSoftKeyAction->setSoftKeyRole(QAction::PositiveSoftKey);
    connect(leftSoftKeyAction, SIGNAL(triggered()), this, SLOT(save()));
    addAction(leftSoftKeyAction);

    QString name, password, server = "";
    QFile file(m_mainWindow->DATADIR+"account.txt");
    file.open(QFile::ReadOnly);
    QString fString = file.readAll();
    QStringList acc = fString.split(" ");
    if (acc.count() == 3) {
        name = acc.at(0);
        password = acc.at(1);
        server = acc.at(2).trimmed();
    }
    file.close();

    ui->jmeno->setText(name);
    ui->heslo->setText(password);
    if (server == "http://gserver/bakaweb/")
        ui->server->setCurrentIndex(1);
}

void Settings::save()
{
    if (ui->heslo->text().isEmpty() || ui->jmeno->text().isEmpty()) {
        QMessageBox::critical(this, "Chyba!", "Musíte vyplnit všechna pole!");
        return;
    }
    QFile file(m_mainWindow->DATADIR+"account.txt");
    file.open(QFile::WriteOnly);
    file.write( QString(ui->jmeno->text() + " " + ui->heslo->text() + " " + ui->server->currentText()).toAscii() );
    file.close();
    close();
}

Settings::~Settings()
{
    qApp->setActiveWindow(m_mainWindow);
    delete ui;
}
