#ifndef EMOTICONPANEL_H
#define EMOTICONPANEL_H

#include <QWidget>

class EmoticonPanel : public QWidget
{
    Q_OBJECT
public:
    explicit EmoticonPanel(QWidget *parent = nullptr);

signals:
    void emoticonSelected(const QString &emoticon);
};

#endif // EMOTICONPANEL_H
