#include "mtnjob.h"
#include<QProcess>
#include<QDir>
#include<QCoreApplication>

MtnJob::MtnJob(QTreeWidgetItem *itm, SettingsData settingsData, QString outputfilename):
    QRunnable(),
    m_item(itm),
    m_sett(settingsData),
    m_outputfilename(outputfilename)
{

}

void MtnJob::run()
{
    QThread::currentThread()->sleep(3);

    m_item->setText(2, "In progress...");    

    QProcess mtn;
#ifdef WIN32
    mtn.setProgram("mtn.exe");
#else
    mtn.setProgram("/opt/mtn");
#endif
    QStringList args = createArguments();
    args << m_item->text(1);                        //    file_or_dir

    //TODO prehodit na QCoreApplication::applicationDirPath()
    //mtn.setWorkingDirectory(QDir::currentPath());
    mtn.setWorkingDirectory(QCoreApplication::applicationDirPath());
    mtn.setProcessChannelMode(QProcess::MergedChannels);
    mtn.setArguments(args);
    mtn.start();

    if(mtn.waitForStarted(10000))
    {
        if(mtn.waitForFinished(10000))
        {
            QString vystup(mtn.readAll());
//            m_item->setText(2, QString(vystup));
            m_item->setData(2, Qt::DisplayRole, vystup.trimmed());
            m_item->setText(3, m_outputfilename);
            m_item->setIcon(0, QIcon::fromTheme("video-x-generic"));
//            m_item->view->viewport()->update();
        }
        else
            m_item->setText(2, QString("Error: %1").arg(mtn.errorString()));
    }
    else
        m_item->setText(2, QString("Error startig: %1").arg(mtn.errorString()));
}

QStringList MtnJob::createArguments()
{
    QStringList args;
    args                                            //         http://moviethumbnail.sourceforge.net/
         << "-P"                                    //    -P : dont pause before exiting; override -p
                                                    //    -p : pause before exiting; default on in win32
         << "-c" << QString::number(m_sett.columns) //    -c 3 : # of column
         << "-r" << QString::number(m_sett.rows)    //    -r 0 : # of rows; >0:override -s
         << "-w" << QString::number(m_sett.width)   //    -w 1024 : width of output image; 0:column * movie width
         << "-g" << QString::number(m_sett.gap);    //    -g 0 : gap between each shot
                                                    //    -a aspect_ratio : override input file's display aspect ratio
                                                    //    -b 0.80 : skip if % blank is higher; 0:skip all 1:skip really blank >1:off
                                                    //    -B 0.0 : omit this seconds from the beginning
                                                    //    -C -1 : cut movie and thumbnails not more than the specified seconds; <=0:off
                                                    //    -D 12 : edge detection; 0:off >0:on; higher detects more; try -D4 -D6 or -D8
                                                    //    -E 0.0 : omit this seconds at the end
    args << "-f" << "/usr/share/fonts/google-crosextra-carlito/Carlito-Regular.ttf";                      //    -f tahomabd.ttf : font file; use absolute path if not in usual places
                                                    //    -F RRGGBB:size[:font:RRGGBB:RRGGBB:size] : font format [time is optional]
                                                    //       info_color:info_size[:time_font:time_color:time_shadow:time_size]
                                                    //    -h 150 : minimum height of each shot; will reduce # of column to fit
                                                    //    -i : info text off
                                                    //    -I : save individual shots too
                                                    //    -j 90 : jpeg quality
                                                    //    -k RRGGBB : background color (in hex)
                                                    //    -L info_location[:time_location] : location of text
                                                    //       1=lower left, 2=lower right, 3=upper right, 4=upper left
                                                    //    -n : run at normal priority
                                                    //    -N info_suffix : save info text to a file with suffix

    if(!m_sett.suffix.isEmpty())
        args << "-o" << m_sett.suffix;              //    -o _s.jpg : output suffix
    if(!m_sett.output_directory.isEmpty())
        args << "-O" << m_sett.output_directory;    //    -O directory : save output files in the specified directory

                                                    //    -s 120 : time step between each shot
                                                    //    -t : time stamp off
                                                    //    -T text : add text above output image
                                                    //    -v : verbose mode (debug)
    if(!m_sett.overwrite)
        args << "-W";                               //    -W : dont overwrite existing files, i.e. update mode
                                                    //    -z : always use seek mode
                                                    //    -Z : always use non-seek mode -- slower but more accurate timing

    return args;
}
