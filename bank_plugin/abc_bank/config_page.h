#ifndef CONFIG_PAGE_H
#define CONFIG_PAGE_H

#include <QMainWindow>
#include <QTimerEvent>

struct trade_meta{
    QString customer_name;
    QString timestamp;
    QString reason;
    double cash = 0;
};
QT_BEGIN_NAMESPACE
namespace Ui { class config_page; }
QT_END_NAMESPACE

class config_page : public QMainWindow
{
    Q_OBJECT

public:
    config_page(QWidget *parent = nullptr);
    ~config_page();

private slots:
    void on_start_btn_clicked();

private:
    Ui::config_page *ui;
    bool is_runing = false;
    int m_timer_id = -1;
    QString m_last_timestamp;
    void log_exec(const QString &_log);
    QString send_and_recv_bank_info(const QString &_req);
    virtual void timerEvent(QTimerEvent *_event);
    QString make_bank_req();
    QList<trade_meta> parse_bank_resp(const QString &_resp);
    void publish_balance(const QList<trade_meta> &_tm_list);
};
#endif // CONFIG_PAGE_H
