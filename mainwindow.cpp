#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    load_dictionary(":/mueller.dict");
    ui->WordList->addItems(words);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load_dictionary(QString path)
{
    if (path == ":/mueller.dict")
        ui->statusbar->showMessage("Стандартный словарь");
    else
        ui->statusbar->showMessage(path);

    QFile inFile(path);

    if (!inFile.open(QIODevice::ReadOnly))
        return;

    QTextStream in(&inFile);

    QString line;
    QString word;
    QString translation;

    while(!in.atEnd()) {
        line = in.readLine();
        if (!line.isEmpty() && !line[0].isSpace()) {
            if (!word.isEmpty()) {
                words.append(word);
                translations.insert(word, translation);
            }
            word = line.simplified();
            translation = word;
        }
        else
            translation = translation + QString("\n") + line;
    }
    if (!word.isEmpty()) {
        words.append(word);
        translations.insert(word, translation);
    }

    inFile.close();
}

void MainWindow::on_WordList_currentTextChanged(const QString &currentText)
{
    ui->TranslationArea->setPlainText(translations[currentText]);
}

void MainWindow::on_SearchLine_textChanged(const QString &arg1)
{
    QRegularExpression rx(arg1);
    ui->WordList->clear();
    ui->WordList->addItems(words.filter(rx));
}

void MainWindow::on_actionOpen_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Open", "", "*.*");

    if (path.isEmpty())
        return;

    words.clear();
    ui->WordList->clear();
    load_dictionary(path);
    ui->WordList->addItems(words);
}
