#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~ClickableLabel();

public slots:
    void slotClicked();

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);

public:
     QString openFile();
};

#endif // CLICKABLELABEL_H
