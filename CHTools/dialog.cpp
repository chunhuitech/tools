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

void Dialog::Zip(QString filename, QString zipfilename)
{
    QFile infile(filename);
    QFile outfile(zipfilename);
    infile.open(QIODevice::ReadOnly);
    outfile.open(QIODevice::WriteOnly);
    QByteArray uncompressedData = infile.readAll();
    QByteArray compressedData = qCompress(uncompressedData,9);
    outfile.write(compressedData);
    infile.close();
    outfile.close();
}

void Dialog::Unzip(QString zipfilename, QString filename)
{
    QFile infile(zipfilename);
    QFile outfile(filename);
    infile.open(QIODevice::ReadOnly);
    outfile.open(QIODevice::WriteOnly);
    QByteArray uncompressedData = infile.readAll();
    QByteArray compressedData = qUncompress(uncompressedData);
    outfile.write(compressedData);
    infile.close();
    outfile.close();
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

void Dialog::getPath1Name1Ext(QString srcFileFullName, QString &path, QString &name, QString &ext)
{
    int first = srcFileFullName.lastIndexOf ("/"); //从后面查找"/"位置
    QString curFileName = srcFileFullName.right (srcFileFullName.length ()-first-1); //从右边截取
    first = srcFileFullName.lastIndexOf (curFileName);
    QString filePath = srcFileFullName.left(first);
    path = filePath;
    int firstExt = curFileName.indexOf('.');
    QString curFileShortName = curFileName.left (firstExt);
     name = curFileShortName;
     ext = curFileName.right(curFileName.length() - curFileShortName.length() -1 );
}

QString Dialog::getSeleFile()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(tr("打开文件"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("dor txt(*.txt *.dor)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //打印所有选择的文件的路径
    QStringList fileNames;
    if(fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
    }
//    for(auto tmp:fileNames)
//        qDebug()<<tmp<<endl;
    if(fileNames.length() > 0){
        return fileNames.at(0);
    }
    return "";
}

void Dialog::on_uiPushButtonGenSql_clicked()
{
    //INSERT INTO comm_record(class_id,sort_num,title,label,relative_path,file_size,file_type,content_html,content_plain,status,modify_time,create_time) VALUES (5, 0, 'd','D','/chunhui_resource/preschool/letters/d.swf',426966,'SWF','','',0,1523283619000,1523283619000);
    QDir dir(selDirPath);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    for(int i=0; i<file_list.count() ;i++)
    {
        QFileInfo tmpFileInfo = file_list.at(i);
        if(tmpFileInfo.isFile())
        {
            QString fileName = tmpFileInfo.fileName();
            int firstExt = fileName.indexOf('.');
            QString labelName = fileName.left (firstExt);


//            QString file_suffix = tmpFileInfo.suffix().toUpper();
            qint64 fileSize = tmpFileInfo.size();
//            qDebug() << fileName;
//            qDebug() << file_suffix;
//            qDebug() << fileSize;
            //26英文字母
//            QString sql = "INSERT INTO comm_record(class_id,sort_num,title,label,relative_path,file_size,file_type,content_html,content_plain,status,modify_time,create_time)";
//            sql += "VALUES (5, 0, '"+labelName+"','"+labelName.toUpper()+"','/chunhui_resource/preschool/letters/" + fileName + "'," + QString::number(fileSize) +",'SWF','','',0,1523287650000,1523287650000);";
            //26英文字母 客户端
            QString sql = "INSERT INTO record(classId,sortNum,title,label,relativePath,fileSize,fileType,contentHtml,contentPlain,status,modifyTime,createTime)";
            sql += "VALUES (5, 0, '"+labelName+"','"+labelName.toUpper()+"','/chunhui_resource/preschool/letters/" + fileName + "'," + QString::number(fileSize) +",'SWF','','',0,1523287650,1523287650);";
            //汉语拼音
//            QString sql = "INSERT INTO comm_record(class_id,sort_num,title,label,relative_path,file_size,file_type,content_html,content_plain,status,modify_time,create_time)";
//            sql += "VALUES (3, 0, '"+labelName+"','"+labelName+"','/chunhui_resource/preschool/pinyin/" + fileName + "'," + QString::number(fileSize) +",'SWF','','',0,1523287650000,1523287650000);";
            //汉字
//            QString sql = "INSERT INTO comm_record(class_id,sort_num,title,label,relative_path,file_size,file_type,content_html,content_plain,status,modify_time,create_time)";
//            sql += "VALUES (4, 0, '"+labelName+"','"+labelName+"','/chunhui_resource/preschool/hanzi/" + fileName + "'," + QString::number(fileSize) +",'SWF','','',0,1523287650000,1523287650000);";
            qDebug() << sql;
        }
    }
}


void Dialog::on_uiPushButtonPress_clicked()
{
    unPressFileName = getSeleFile();
    QString path, name, ext;
    getPath1Name1Ext(unPressFileName, path, name, ext);
    pressFileName = path + name + ".dor";
    Zip(unPressFileName,pressFileName);
}

void Dialog::on_uiPushButtonUnPress_clicked()
{
    pressFileName =  getSeleFile();
    QString path, name, ext;
    getPath1Name1Ext(pressFileName, path, name, ext);
    unPressFileName = path + name + ".txt";
    Unzip(pressFileName,unPressFileName);
}
