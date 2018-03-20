#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <QDateTime>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButtonSelFolder_clicked()
{
//    QFileDialog *fileDialog = new QFileDialog(this);
//    fileDialog->setFileMode(QFileDialog::Directory);
    selDirPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    qDebug() << selDirPath;
}

void Dialog::on_pushButtonModTime_clicked()
{
    QDir dir(selDirPath);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    for(int i=0; i<file_list.count() ;i++)
    {
        QFileInfo tmpFileInfo = file_list.at(i);
        if(tmpFileInfo.isFile())
        {
            QString fileName = tmpFileInfo.absoluteFilePath();
            QString filePath = tmpFileInfo.absoluteDir().absolutePath();
            QDateTime lastMdTime = tmpFileInfo.lastModified();
            QString strLastMdTime = lastMdTime.toString("yyyyMMdd_hhmmss");
            QString newfileName;
            QString file_suffix = tmpFileInfo.suffix();
            if( file_suffix.toUpper() == "JPG") {
                newfileName = filePath + "/" + "IMG_" + strLastMdTime+ "." + file_suffix;
            } else if(file_suffix.toUpper() == "MP4"){
                newfileName = filePath + "/" + "VID_" + strLastMdTime+ "." + file_suffix;
            }
            qDebug() << fileName;
            qDebug() << newfileName;

             QString newFileNameApp = getNewFileNameApp(newfileName, 0);
             bool renameOK = QFile::rename(fileName, newFileNameApp);
             if(!renameOK && fileName != newFileNameApp ){
                 for(int i=1; i<=1000; i++){
                     newFileNameApp = getNewFileNameApp(newfileName,i);
                     renameOK = QFile::rename(fileName, newFileNameApp);
                     if(renameOK){
                         break;
                     }
                 }
             }
        }
    }
}

QString Dialog::getNewFileNameApp(QString newfileName, int appIndex)
{
    int first = newfileName.lastIndexOf ("/"); //从后面查找"/"位置
    QString curFileName = newfileName.right (newfileName.length ()-first-1); //从右边截取
    first = newfileName.lastIndexOf (curFileName);
    QString filePath = newfileName.left(first);
    int firstExt = curFileName.indexOf('.');
    QString curFileShortName = curFileName.left (firstExt);
    QString fileExt = curFileName.right(curFileName.length() - curFileShortName.length() -1 );
    QString newFileName;
    if(appIndex == 0){
       newFileName  = filePath + curFileShortName + "." + fileExt;
    } else {
       newFileName  = filePath + curFileShortName + "(" +QString::number(appIndex++, 10)  +")" + "." + fileExt;
    }

    return newFileName;
}
