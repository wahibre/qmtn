#include "mtnjob.h"
#include<QProcess>
#include<QDir>
#include<QCoreApplication>

/******************************************************************************************************/
MtnJob::MtnJob(QStandardItem *parent, int row, SettingsData settingsData, QString outputfilename):
    QRunnable(),
    m_stditem(parent),
    m_row(row),
    m_sett(settingsData),
    m_outputfilename(outputfilename)
{

}

/******************************************************************************************************/
void MtnJob::run()
{

    QProcess mtn;
#ifdef WIN32
    mtn.setProgram("mtn.exe");
#else
    mtn.setProgram("/opt/mtn"); //FIXME add mtn to PATH or find it
#endif
    QStringList args = createArguments();
    args << m_stditem->child(m_row, 1)->text();

    mtn.setWorkingDirectory(QCoreApplication::applicationDirPath());
    mtn.setProcessChannelMode(QProcess::MergedChannels);
    mtn.setArguments(args);
    mtn.start();

    if(mtn.waitForStarted(WAIT_STARTTIMEOUT))
    {
        if(mtn.waitForFinished(WAIT_FINISHTIMEOUT))
        {
            QString vypis, vystup(mtn.readAll());

            vypis = "*** Calling ****\n\n";
            vypis += mtn.program() + "   ";
            vypis += mtn.arguments().join(' ')+ " \n\n";
            vypis += "*** Result ****\n\n";
            vypis += vystup.trimmed();

            m_stditem->child(m_row, dataItemNames::filename )->setIcon(ICON_VIDEO);
            m_stditem->child(m_row, dataItemNames::log      )->setText(vypis);
            m_stditem->child(m_row, dataItemNames::output   )->setText(m_outputfilename);
        }
        else
            m_stditem->child(m_row, dataItemNames::log)->setText(QString("Error: %1").arg(mtn.errorString()));
    }
    else
        m_stditem->child(m_row, dataItemNames::log)->setText(QString("Error startig: %1").arg(mtn.errorString()));
}
/******************************************************************************************************/
QStringList MtnJob::createArguments()
{
    const QString DEFAULT_UNIX_FONT="/usr/share/fonts/google-crosextra-carlito/Carlito-Regular.ttf";

    QStringList args;
    args                                            //         http://moviethumbnail.sourceforge.net/
         << "-P"                                    //    -P : dont pause before exiting; override -p
                                                    //    -p : pause before exiting; default on in win32
         << "-c" << QString::number(m_sett.columns) //    -c 3 : # of column
         << "-r" << QString::number(m_sett.rows)    //    -r 0 : # of rows; >0:override -s
         << "-w" << QString::number(m_sett.width)   //    -w 1024 : width of output image; 0:column * movie width
         << "-g" << QString::number(m_sett.gap);    //    -g 0 : gap between each shot
                                                    //    -a aspect_ratio : override input file's display aspect ratio

    if(qAbs(m_sett.blank_skip-0.8)>0.001)           //    -b 0.80 : skip if % blank is higher; 0:skip all 1:skip really blank >1:off
        args << "-b" << QString::asprintf("%.2f", m_sett.blank_skip);

    if(m_sett.skip_begin > 0.01)                    //    -B 0.0 : omit this seconds from the beginning
        args << "-B" << QString::asprintf("%.1f", m_sett.skip_begin);

                                                    //    -C -1 : cut movie and thumbnails not more than the specified seconds; <=0:off

    if(m_sett.edge_detect>0)                        //    -D 12 : edge detection; 0:off >0:on; higher detects more; try -D4 -D6 or -D8
        args << "-D" << QString::number(m_sett.edge_detect);

    if(m_sett.skip_end > 0.01)                      //    -E 0.0 : omit this seconds at the end
        args << "-E" << QString::asprintf("%.1f", m_sett.skip_end);

                                                    //    -f tahomabd.ttf : font file; use absolute path if not in usual places
    args << "-f" << DEFAULT_UNIX_FONT;

                                                    //    -F RRGGBB:size[:font:RRGGBB:RRGGBB:size] : font format [time is optional]
    args << "-F" << color2hex(m_sett.foreground)+":14";
                                                    //       info_color:info_size[:time_font:time_color:time_shadow:time_size]
                                                    //    -h 150 : minimum height of each shot; will reduce # of column to fit

    if(!m_sett.infotext)                            //    -i : info text off
        args << "-i";
                                                    //    -I : save individual shots too

    if(m_sett.quality != 90)                        //    -j 90 : jpeg quality
        args << "-j" << QString::number(m_sett.quality);

    args << "-k" << color2hex(m_sett.background);   //    -k RRGGBB : background color (in hex)

                                                    //    -L info_location[:time_location] : location of text
                                                    //       1=lower left, 2=lower right, 3=upper right, 4=upper left
                                                    //    -n : run at normal priority
                                                    //    -N info_suffix : save info text to a file with suffix

    if(!m_sett.suffix.isEmpty())
        args << "-o" << m_sett.suffix;              //    -o _s.jpg : output suffix
    if(!m_sett.output_directory.isEmpty())
        args << "-O" << m_sett.output_directory;    //    -O directory : save output files in the specified directory

                                                    //    -s 120 : time step between each shot

    if(!m_sett.timestamp)                           //    -t : time stamp off
        args << "-t";

    if(!m_sett.title.isEmpty())                     //    -T text : add text above output image
        args << "-T" << m_sett.title;
                                                    //    -v : verbose mode (debug)
    if(!m_sett.overwrite)
        args << "-W";                               //    -W : dont overwrite existing files, i.e. update mode
                                                    //    -z : always use seek mode
                                                    //    -Z : always use non-seek mode -- slower but more accurate timing

    return args;
}

QString MtnJob::color2hex(QColor color)
{
    if(color.isValid())
        return QString::asprintf("%02x%02x%02x", color.red(), color.green(), color.blue());

    return QString("FFFFFF");
}
/******************************************************************************************************/
