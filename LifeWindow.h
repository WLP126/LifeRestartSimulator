#ifndef LIFEWINDOW_H
#define LIFEWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QTextEdit>
#include <QPropertyAnimation>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QResizeEvent>
#include "LifeEngine.h"

namespace Ui {
class LifeWindow;
}

class LifeWindow : public QMainWindow {
    Q_OBJECT
protected:
    // 在现有代码中添加以下声明
    void resizeEvent(QResizeEvent *event) override;

public:
    explicit LifeWindow(QWidget *parent = nullptr);
    ~LifeWindow();

private slots:
    void onStartClicked();             // 点击“开始人生”按钮
    void onChoiceMade(int index);     // 选择按钮被点击时，传入选项索引
    void onGameUpdated();             // 更新 UI
    void onGameOver(QString summary); // 游戏结束时显示总结信息
    void typeWriterEffect(const QString &text);

private:
    Ui::LifeWindow *ui;
    LifeEngine *m_engine;
    QPushButton *m_choiceButtons[4];
    QStackedWidget *m_stackedWidget;
    QWidget *m_startPage;
    QWidget *m_gamePage;
    QWidget *m_summaryPage;
    QTextEdit *m_summaryTextEdit;
    QString m_currentDisplayText;
    int m_typePos;
    QTimer *m_typeTimer;
    QGraphicsScene *m_scene;
    QGraphicsView *m_bgView;

    void updateUI();
    void showStartPage();
    void showGamePage();
    void showSummaryPage();
    void animateButton(QPushButton* button);
};

#endif // LIFEWINDOW_H

