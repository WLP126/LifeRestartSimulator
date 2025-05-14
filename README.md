***!!!最终结果请查看最终版2.0版本！！！***
***！！！已上传报告在最后一个！！！***

🧬 LifeRestartSimulator · 人生重开模拟器
一个基于 Qt/C++ 开发的高自由度互动式人生模拟器，随机属性、成长路径、事件抉择，每一次重开都是不一样的命运。

🎮 特性 Features
🌱 随机生成初始属性（智力、财富、健康、幸福、颜值）

📚 覆盖从出生到晚年的完整人生阶段

🧠 多线分支：艺术、恋爱、职场、婚姻、修仙...

🧭 玩家选择决定事件走向与属性成长

📊 结局评分系统 + 图文人生总结

🖼️ 美观 UI（使用 Qt Widgets + QGraphicsView）

🔮 可扩展事件系统（支持 flags/tags 条件判断）

🛠️ 项目结构
LifeRestartSimulator/
├── LifeWindow.h / .cpp     // 主窗口界面及交互
├── LifeEngine.h / .cpp     // 游戏主逻辑引擎
├── LifeEvent.h             // 事件数据结构与枚举
├── main.cpp
├── LifeRestartSimulator.pro
└── README.md
💻 运行方式（本地编译）
🔧 环境要求
Qt 5 或 Qt 6（推荐 Qt 5.15+）

Qt Creator 或命令行工具 qmake + make

C++17 支持

🧪 编译步骤
git clone https://github.com/WLP126/LifeRestartSimulator.git
cd LifeRestartSimulator
qmake
make
./LifeRestartSimulator
📷 截图预览
初始界面	游戏中事件选择	人生总结

📦 TODO / 后续计划
 自定义初始属性模式

 支持更多职业路径（科研 / 宗教 / 黑客等）

 本地保存历史人生记录

 JSON 事件导入导出系统

🧠 灵感来源
本项目灵感来源于 Steam 游戏《人生重开模拟器》与各类文字人生 RPG，旨在提供开放式、多结局的可重复体验。

📄 License
MIT License © 2025 WLP126

---------------------------------HERE IS ENGLISH VIEW---------------------------------------

# LifeRestartSimulator · 人生重开模拟器

A Qt-based interactive life simulation game that lets you relive a fictional life with choices, attributes, and events from birth to old age.

## 🎮 Features

- 🌱 Start a new life with random initial attributes.
- 🧠 Develop intelligence, wealth, health, happiness, and appearance.
- 🏫 Experience school, career, relationships, family, and aging.
- 🔄 Make choices that influence your path and unlock life events.
- 🎨 Beautiful interface with smooth animations and a typewriter effect.
- 🧘 Multiple endings, including immortality and enlightenment!

## 📷 Preview

![Preview Screenshot](preview.png)

## 📦 Requirements

- Qt 5.15+ (or compatible version)
- C++11 or later

## 🚀 Build & Run

### Using Qt Creator:
1. Open the project file `LifeRestartSimulator.pro`.
2. Click "Build" and "Run".

### Or via command line:
```bash
qmake
make
./LifeRestartSimulator
📁 Project Structure
├── LifeEngine.h / .cpp       # Core game logic and event system
├── LifeWindow.h / .cpp       # UI logic and display
├── LifeEvent.h               # Event and attribute data structure
├── LifeWindow.ui             # Qt Designer UI layout
├── preview.png               # Screenshot for README
├── LifeRestartSimulator.pro  # Qt project configuration
└── README.md
🧠 Attributes Explained
intelligence - Affects education and career choices.

wealth - Influences life comfort and options.

health - Decreases over time or due to bad choices.

happiness - Determines emotional wellbeing.

appearance - Minor effects on love and social events.

🎯 Endings
You may:

Live a peaceful life.

Struggle through hardships.

Become rich, wise, or even... immortal.

📜 License
This project is for educational and non-commercial use. Feel free to fork and build your own version!

Have fun restarting your life! 🌟
