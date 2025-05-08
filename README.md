人生重启模拟器项目说明
一、项目概述
人生重启模拟器是一个基于 Qt 框架开发的模拟人生游戏。玩家可以体验从出生到死亡的一生，在不同的人生阶段会遇到各种随机事件，并需要做出选择，这些选择会影响玩家角色的各项属性，最终决定人生的评价。
二、项目结构
1. 主要文件
LifeEngine.h 和 LifeEngine.cpp：核心逻辑引擎，负责管理游戏的状态、加载事件、触发随机事件、处理玩家选择等。
LifeWindow.h 和 LifeWindow.cpp：主窗口类，负责与用户交互，显示游戏界面，处理按钮点击事件，更新 UI 显示。
main.cpp：程序入口，创建并显示主窗口。
2. 其他文件
.gitignore：用于忽略不需要纳入版本控制的文件。
三、类和结构体说明
1. Attribute 结构体
用于表示角色的各项属性，包括智力、财富、健康、幸福和颜值，取值范围为 0 - 100。

cpp
struct Attribute {
    int intelligence;  // 智力 (0-100)
    int wealth;        // 财富 (0-100)
    int health;        // 健康 (0-100)
    int happiness;     // 幸福 (0-100)
    int appearance;    // 颜值 (0-100)

    Attribute(int intel = 0, int w = 0, int h = 0, int happy = 0, int app = 0)
        : intelligence(intel), wealth(w), health(h), happiness(happy), appearance(app) {}
};
2. LifeEvent 结构体
表示一个人生事件，包含事件描述、最小触发年龄、最大触发年龄、可选操作和每个操作对应的属性影响。

cpp
struct LifeEvent {
    QString description;       // 事件描述
    int minAge;               // 最小触发年龄
    int maxAge;               // 最大触发年龄
    QVector<QString> choices; // 可选操作
    QVector<Attribute> effects; // 每个操作对应的属性影响

    LifeEvent(const QString& desc = "",
             int min = 0, int max = 100,
             const QVector<QString>& ch = QVector<QString>(),
             const QVector<Attribute>& eff = QVector<Attribute>())
        : description(desc), minAge(min), maxAge(max), choices(ch), effects(eff) {}
};
3. LifeEngine 类
核心逻辑引擎类，负责游戏的控制和状态管理。
主要成员函数
startNewLife()：开始新的人生，初始化随机属性，触发第一个事件。
makeChoice(int choiceIndex)：处理玩家的选择，应用选择的效果，检查游戏结束条件，进入下一年。
nextYear()：进入下一年，处理每年的自然属性变化，有 60% 的概率触发随机事件。
triggerRandomEvent()：触发符合当前年龄的随机事件。
lifeSummary()：生成人生总结，包括人生记录和最终评价。
4. LifeWindow 类
主窗口类，负责与用户交互和 UI 显示。
主要成员函数
onStartClicked()：点击 “开始人生” 按钮时调用，开始新的人生。
onChoiceMade(int index)：玩家做出选择时调用，将选择传递给 LifeEngine 处理。
onGameUpdated()：游戏状态更新时调用，更新 UI 显示。
onGameOver(QString summary)：游戏结束时调用，显示人生总结信息，并自动开始新游戏。
updateUI()：更新界面上的所有属性和事件信息。
四、使用说明
1. 编译和运行
确保已经安装了 Qt 开发环境，然后使用 Qt Creator 打开项目，编译并运行。
2. 游戏流程
点击 “开始人生” 按钮，游戏开始，角色出生并获得随机属性。
在不同的年龄阶段，可能会遇到各种随机事件，每个事件有多个可选操作。
点击相应的选择按钮做出选择，选择会影响角色的属性。
游戏会持续进行，直到角色的健康降到 0 或活到 100 岁，游戏结束后会显示人生总结，并自动开始新游戏。
五、注意事项
代码中的事件是硬编码的，可以根据需要在 LifeEngine::loadEvents() 函数中添加或修改事件。
确保 UI 界面中的按钮和标签名称与代码中的引用一致，否则可能会导致 UI 显示异常。
