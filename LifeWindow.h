#ifndef LIFEWINDOW_H
#define LIFEWINDOW_H

#include <QMainWindow>
#include <QPushButton>  // 用于定义按钮数组
#include "LifeEngine.h" // 假设你有一个模拟人生引擎类

namespace Ui {
class LifeWindow;
}

class LifeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LifeWindow(QWidget *parent = nullptr);
    ~LifeWindow();

private slots:
    void onStartClicked();             // 点击“开始人生”按钮
    void onChoiceMade(int index);     // 选择按钮被点击时，传入选项索引
    void onGameUpdated();             // 更新 UI
    void onGameOver(QString summary); // 游戏结束时显示总结信息

private:
    Ui::LifeWindow *ui;               // Qt 自动生成的 UI 类
    LifeEngine *m_engine;            // 模拟人生的核心逻辑引擎

    // ✅ 加入：用于封装 4 个选择按钮的数组
    QPushButton* m_choiceButtons[4];

    void updateUI(); // 更新界面上所有属性和事件信息
};

#endif // LIFEWINDOW_H

