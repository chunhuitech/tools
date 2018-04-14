#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore/QFile>
#include <QByteArray>
#include <QBitArray>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

protected:
    void Zip (QString filename , QString zipfilename);
    void Unzip (QString zipfilename , QString filename);

private slots:
    void on_pushButtonSelFolder_clicked();

    void on_pushButtonModTime_clicked();


    void on_uiPushButtonGenSql_clicked();

    void on_uiPushButtonPress_clicked();

    void on_uiPushButtonUnPress_clicked();

private:
    QString getNewFileNameApp(QString newfileName, int appIndex);
    void getPath1Name1Ext(QString srcFileFullName, QString & path, QString &name, QString & ext);
    QString getSeleFile();
private:
    QString selDirPath;
    QString unPressFileName;
    QString pressFileName;
private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
