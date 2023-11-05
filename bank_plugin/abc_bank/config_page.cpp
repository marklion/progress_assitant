#include "config_page.h"
#include "./ui_config_page.h"
#include <QTcpSocket>
#include <QDateTime>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>


config_page::config_page(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::config_page)
{
    ui->setupUi(this);
}

config_page::~config_page()
{
    delete ui;
}

QString config_page::send_and_recv_bank_info(const QString &_req)
{
    QString ret;

    QString head = "0";
    head += QString::number( _req.length());
    for (int var = head.length(); var < 7; ++var) {
        head += " ";
    }
    QTcpSocket sck;
    sck.connectToHost(QHostAddress("127.0.0.1"), 15999);
    if (true == sck.waitForConnected(3000))
    {
        sck.write((head + _req).toStdString().c_str());
        auto resp = sck.read(65535);
        ret = QString::fromStdString( resp.toStdString());
    }
    else
    {
        log_exec("failed to connect");
    }
    return ret;
}

void config_page::on_start_btn_clicked()
{
    is_runing = !is_runing;
    if (is_runing)
    {
        log_exec("开始运行");
        if (-1 == m_timer_id)
        {
            m_timer_id = startTimer(5000);
        }
        ui->start_btn->setText(tr("停止"));
    }
    else
    {
        log_exec("停止运行");
        if (-1 != m_timer_id)
        {
            killTimer(m_timer_id);
        }
        ui->start_btn->setText(tr("开始"));
    }
}

void config_page::log_exec(const QString &_log)
{
    QDateTime dt = QDateTime::currentDateTime();
    auto dt_string = dt.toString("yyyy.MM.dd hh:mm:ss.zzz");
    ui->log_br->append(dt_string + _log);
}

void config_page::timerEvent(QTimerEvent *_event)
{
    log_exec("enter timer");
    if (is_runing)
    {
        auto resp = send_and_recv_bank_info(make_bank_req());
        auto tm_list = parse_bank_resp(resp);
        publish_balance(tm_list);
    }
}

QString config_page::make_bank_req()
{
    QString req_string;
    QDateTime dt = QDateTime::currentDateTime();
    auto dt_string = dt.toString("yyyyMMdd");
    auto dt_time_string = dt.toString("hhmmss");
    QXmlStreamWriter req_xml(&req_string);
    req_xml.setAutoFormatting(true);
    req_xml.writeStartElement("ap");
    req_xml.writeTextElement("CCTransCode", "CQRA10");
    req_xml.writeTextElement("ProductID", "ICC");
    req_xml.writeTextElement("ChannelType", "ERP");
    req_xml.writeTextElement("CorpNo", "05374153300005786");
    req_xml.writeTextElement("OpNo", "");
    req_xml.writeTextElement("AuthNo", "");
    req_xml.writeTextElement("ReqSeqNo", QString::number( time(nullptr)).last(8));
    req_xml.writeTextElement("ReqDate", dt_string);
    req_xml.writeTextElement("Sign", "");
    req_xml.writeTextElement("ReqTime", dt_time_string);
    req_xml.writeStartElement("Corp");
    req_xml.writeTextElement("StartDate", dt_string);
    req_xml.writeTextElement("EndDate", dt_string);
    req_xml.writeEndElement();
    req_xml.writeStartElement("Cmp");
    req_xml.writeTextElement("DbAccNo", "374101040011425");
    req_xml.writeTextElement("DbProv", "05");
    req_xml.writeTextElement("DbCur", "01");
    if (!m_last_timestamp.isEmpty())
    {
        req_xml.writeTextElement("StartTime", m_last_timestamp);
    }
    req_xml.writeEndElement();

    req_xml.writeEndElement();
    return req_string;
}

QList<trade_meta> config_page::parse_bank_resp(const QString &_resp)
{
    QList<trade_meta> tm_list;
    QXmlStreamReader resp_xml(_resp);
    while (!resp_xml.atEnd())
    {
        resp_xml.readNext();
        if ( QString( "BatchFileName") == resp_xml.qualifiedName())
        {
            auto file_name = resp_xml.readElementText();
            QFile trade_file("D:/detail/" + file_name);
            if (trade_file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream in(&trade_file);
                while (!in.atEnd())
                {
                    auto one_record = in.readLine();
                    auto or_list = one_record.split("|");
                    trade_meta tmp;
                    tmp.cash = or_list[18].toDouble() - or_list[19].toDouble();
                    tmp.customer_name = or_list[12];
                    tmp.reason = or_list[29];
                    tmp.timestamp = or_list[4];
                    tm_list.push_back(tmp);
                }
                trade_file.remove();
            }
            else
            {
                log_exec("faild to open trade file");
            }
            break;
        }
    }

    return tm_list;
}

void config_page::publish_balance(const QList<trade_meta> &_tm_list)
{
    for (auto &itr:_tm_list)
    {
        log_exec(itr.customer_name + "-->增加余额-->" + QString::number(itr.cash) + "-->原因-->" + itr.reason);
    }
}

