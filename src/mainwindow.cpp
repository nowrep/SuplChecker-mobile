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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parser.h"
#include "nastaveni.h"

SuplChecker::SuplChecker(QWidget *parent) :
    QMainWindow(parent)
    ,DATADIR("e:/data/suplchecker/")
    ,ui(new Ui::SuplChecker)
    ,m_isActualizationDialogShown(false)
    ,m_isLoading(false)
{
    QTranslator translator;
    translator.load(DATADIR+"qt_cs.qm");
    qApp->installTranslator(&translator);

    ui->setupUi(this);
    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowSoftkeysVisibleHint;
    flags &= ~Qt::WindowSoftkeysRespondHint;
    setWindowFlags(flags);
#ifdef Q_OS_SYMBIAN
    showFullScreen();
#endif
    showSoftKeys();
    beginToLoad();
}

void SuplChecker::beginToLoad()
{
    QFile file(DATADIR+"account.txt");
    file.open(QFile::ReadOnly);
    QString fString = file.readAll();
    QString name, password, server_ = "";
    QStringList acc = fString.split(" ");
    if (acc.count() != 3)
        QMessageBox::critical(this, "Chyba", "Špatný formát account.txt souboru.\nSprávně: \"jmeno heslo server\" bez uvozovek.");
    else {
        name = acc.at(0);
        password = acc.at(1);
        server_ = acc.at(2).trimmed();

        zacni_loadovat(name, password, server_);
    }
    file.close();
}

void SuplChecker::showSoftKeys()
{
    QMenu* menuOptions = new QMenu();
    QMenu* menuShow = new QMenu("Zobrazit...");
    menuShow->addAction("Zobrazit tento týden", this, SLOT(showPageOnSymbian()))->setData(SuplChecker::CurrentWeek);
    menuShow->addAction("Zobrazit příští týden", this, SLOT(showPageOnSymbian()))->setData(SuplChecker::NextWeek);
    menuShow->addAction("Zobrazit stálý rozvrh", this, SLOT(showPageOnSymbian()))->setData(SuplChecker::Permanent);
    menuShow->addAction("Zobrazit hodnocení", this, SLOT(showPageOnSymbian()))->setData(SuplChecker::Marks);
    menuOptions->addMenu(menuShow);
    menuOptions->addAction("Načíst znovu", this, SLOT(beginToLoad()));
    menuOptions->addAction("Nastavení", this, SLOT(udaje()));
    menuOptions->addAction("O programu", this, SLOT(info_o_programu()));
    menuOptions->addAction("Konec", this, SLOT(close()));

    m_rightSoftKeyAction = new QAction("SuplChecker", this);
    m_rightSoftKeyAction->setSoftKeyRole(QAction::NegativeSoftKey);
#ifdef QT_SIMULATOR
    connect(m_rightSoftKeyAction, SIGNAL(triggered()), this, SLOT(udaje()));
#endif
    addAction(m_rightSoftKeyAction);

    QAction* leftSoftKeyAction = new QAction("Možnosti", this);
    leftSoftKeyAction->setSoftKeyRole(QAction::PositiveSoftKey);
    leftSoftKeyAction->setMenu(menuOptions);
    addAction(leftSoftKeyAction);
}

void SuplChecker::setLoading(bool set)
{
    m_isLoading = set;
}

void SuplChecker::zacni_loadovat(QString uzjmeno, QString uzheslo, QString server)
{
    if (m_isLoading)
        return;

    spatneUdaje=0;

    ui->webView_1->setUrl(QUrl::fromLocalFile(DATADIR+"loading.html"));
    ui->webView_2->setUrl(QUrl::fromLocalFile(DATADIR+"loading.html"));
    ui->webView_3->setUrl(QUrl::fromLocalFile(DATADIR+"loading.html"));
    ui->webView_4->setUrl(QUrl::fromLocalFile(DATADIR+"loading.html"));

    Parser *vlakno = new Parser(uzjmeno,uzheslo,server);
    connect(vlakno, SIGNAL(jmeno(QString,QString)),this, SLOT(jmeno(QString,QString)),Qt::QueuedConnection);
    connect(vlakno, SIGNAL(done(QString,QByteArray)),this, SLOT(nacti(QString,QByteArray)),Qt::QueuedConnection);
    connect(vlakno, SIGNAL(chyba(QString)),this, SLOT(chyba(QString)),Qt::QueuedConnection);
    connect(vlakno, SIGNAL(loading(bool)),this, SLOT(setLoading(bool)),Qt::QueuedConnection);
    vlakno->start();
}

void SuplChecker::info_o_programu()
{
    QMessageBox msgBox(this);
    msgBox.setText("SuplChecker 0.6<small><br/>Checker suplů a známek<br/>Autor: nowrep<br/>"
                   "(C) 2010-2011 nowrep<br/><a href='http://suplchecker.wz.cz'>http://suplchecker.wz.cz</a></small>");
    msgBox.setIconPixmap(QPixmap(":icons/SuplCheckerMobile.svg"));
    msgBox.exec();
}

void SuplChecker::udaje()
{
    Settings* window = new Settings(this);
    window->show();
}

void SuplChecker::nacti(QString info, QByteArray data)
{
    if (spatneUdaje==0){
    if (info=="tentotyden.html")
        ui->webView_1->page()->mainFrame()->setContent(data, QString(), QUrl::fromLocalFile(DATADIR));
    if (info=="dalsityden.html")
        ui->webView_2->page()->mainFrame()->setContent(data, QString(), QUrl::fromLocalFile(DATADIR));
    if (info=="stalyrozvrh.html")
        ui->webView_3->page()->mainFrame()->setContent(data, QString(), QUrl::fromLocalFile(DATADIR));
    if (info=="znamky.html")
        ui->webView_4->page()->mainFrame()->setContent(data, QString(), QUrl::fromLocalFile(DATADIR));
    }else if (spatneUdaje!=5){
        spatneUdaje=5;
        QMessageBox msgBox;
        msgBox.setWindowTitle("Chyba!");
        msgBox.setText("Nepodařilo se přihlásit na Váš účet!\nZkontrolujte správnost údajů, internetové připojení a dostupnost Bakalářů!");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowIcon(QIcon(":icon.png"));
        msgBox.exec();
    }
}

void SuplChecker::jmeno(QString jmeno, QString trida)
{
    if (jmeno!="") {
        m_rightSoftKeyAction->setText(jmeno+"-"+trida+"");
    } else {
        spatneUdaje=1;
        m_rightSoftKeyAction->setText("Nesprávné údaje!");
    }
}

void SuplChecker::chyba(QString text)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Chyba!");
    msgBox.setText("Při zpracování suplování došlo k chybě:\n"+text);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

void SuplChecker::showPageOnSymbian()
{
    if (QAction *action = qobject_cast<QAction*>(sender())) {
        switch (action->data().toInt()) {
        case SuplChecker::CurrentWeek:
            ui->stackedWidget->setCurrentIndex(0);
            break;
        case SuplChecker::NextWeek:
            ui->stackedWidget->setCurrentIndex(1);
            break;
        case SuplChecker::Permanent:
            ui->stackedWidget->setCurrentIndex(2);
            break;
        case SuplChecker::Marks:
            ui->stackedWidget->setCurrentIndex(3);
            break;
        }
    }
}

SuplChecker::~SuplChecker()
{
    delete ui;
}
