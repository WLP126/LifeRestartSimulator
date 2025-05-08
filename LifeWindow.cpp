#include "LifeWindow.h"
#include "ui_LifeWindow.h"
#include <QMessageBox>

LifeWindow::LifeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LifeWindow),
    m_engine(new LifeEngine(this))
{
    ui->setupUi(this);

    // 初始化按钮数组，指向 UI 中的四个选择按钮
    m_choiceButtons[0] = ui->choiceButtons_0;
    m_choiceButtons[1] = ui->choiceButtons_1;
    m_choiceButtons[2] = ui->choiceButtons_2;
    m_choiceButtons[3] = ui->choiceButtons_3;

    // 连接“开始人生”按钮
    connect(ui->startButton, &QPushButton::clicked, this, &LifeWindow::onStartClicked);

    // 连接游戏引擎信号
    connect(m_engine, &LifeEngine::gameUpdated, this, &LifeWindow::onGameUpdated);
    connect(m_engine, &LifeEngine::gameOver, this, &LifeWindow::onGameOver);

    // 连接每个选择按钮的点击事件
    for (int i = 0; i < 4; ++i) {
        connect(m_choiceButtons[i], &QPushButton::clicked, [this, i]() {
            onChoiceMade(i);
        });
    }

    // 初始界面刷新
    onGameUpdated();
}

LifeWindow::~LifeWindow() {
    delete ui;
}

// 点击“开始人生”按钮
void LifeWindow::onStartClicked() {
    m_engine->startNewLife();
}

// 用户做出选择
void LifeWindow::onChoiceMade(int index) {
    m_engine->makeChoice(index);
}

// 游戏更新后刷新界面
void LifeWindow::onGameUpdated() {
    updateUI();
}

// 游戏结束时显示总结信息，并自动开始新游戏
void LifeWindow::onGameOver(QString summary) {
    QMessageBox::information(this, "人生结束", summary);
    onStartClicked(); // 自动重新开始
}

// 更新所有 UI 显示
void LifeWindow::updateUI() {
    Attribute attr = m_engine->currentAttributes();

    ui->ageLabel->setText(QString("年龄: %1").arg(m_engine->currentAge()));
    ui->intelLabel->setText(QString("智力: %1").arg(attr.intelligence));
    ui->wealthLabel->setText(QString("财富: %1").arg(attr.wealth));
    ui->healthLabel->setText(QString("健康: %1").arg(attr.health));
    ui->happyLabel->setText(QString("幸福: %1").arg(attr.happiness));
    ui->appearLabel->setText(QString("颜值: %1").arg(attr.appearance));

    // 显示当前事件
    ui->eventLabel->setText(m_engine->m_currentEvent());

    // 更新选择按钮文本与显示状态
    QVector<QString> choices = m_engine->currentChoices();
    for (int i = 0; i < 4; ++i) {
        if (i < choices.size()) {
            m_choiceButtons[i]->setText(choices[i]);
            m_choiceButtons[i]->setVisible(true);
        } else {
            m_choiceButtons[i]->setVisible(false);
        }
    }

    // 更新人生总结日志
    ui->logText->setPlainText(m_engine->lifeSummary());
}
