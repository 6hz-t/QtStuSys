# QtStuSys - 基于 Qt + MySQL 的学生管理系统

![QtStuSys 系统截图](https://github.com/6hz-t/QtStuSys/blob/main/screenshots/main_window.png?raw=true)  
*系统主界面（示例截图，实际需替换为项目真实截图路径）*

## 项目简介
QtStuSys 是一款基于 **Qt 6.10.0** 框架开发的桌面端学生管理系统，集成 MySQL 数据库存储数据，支持学生信息、课程、成绩的全流程管理，提供 Excel（XLSX）批量导入/导出功能，界面简洁易用、支持 QSS 样式自定义，适用于学校、培训机构的日常学生管理场景。

## 核心功能
| 模块         | 核心功能                                                                 |
|--------------|--------------------------------------------------------------------------|
| 学生管理     | 增删改查、分页查询、条件筛选、逻辑删除、XLSX 批量导入/导出学生信息       |
| 课程管理     | 课程信息维护、选课关联（基于中间表）、课程列表导出为 XLSX                |
| 成绩管理     | 按课程/班级录入成绩、成绩查询修改、作废/恢复、XLSX 批量导入/导出成绩     |
| 系统管理     | 操作日志查看、MySQL 连接配置、QSS 样式自定义、用户权限（待完善）         |

## 技术栈
- **框架**：Qt 6.10.0（支持 Qt 6.x 兼容版本）
- **编译器**：MinGW 64-bit
- **数据库**：MySQL 8.0+（支持配置其他兼容数据库）
- **文件处理**：支持 XLSX 格式导入/导出（依赖 Qt 自带模块或第三方库）
- **UI 美化**：支持 QSS 样式表自定义界面主题

## 环境准备
### 1. 开发环境
- 安装 Qt 6.10.0 及以上版本（含 Qt Creator）
- 安装 MinGW 64-bit 编译器（Qt 安装时可勾选）
- 安装 MySQL 8.0+ 数据库（本地或远程服务器均可）
- （可选）XLSX 支持：若需增强 XLSX 功能，可集成 `QXlsx` 第三方库（[GitHub 地址](https://github.com/QtExcel/QXlsx)）

### 2. 数据库配置
1. 启动 MySQL 服务，创建数据库（默认库名：`qt_stu_sys`，可在配置文件修改）：
   ```sql
   CREATE DATABASE IF NOT EXISTS qt_stu_sys DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
   ```
2. 导入项目根目录下的 `sql/qt_stu_sys_init.sql` 文件初始化数据表（学生表、课程表、成绩表、日志表等）：
   ```sql
   USE qt_stu_sys;
   SOURCE /path/to/qt_stu_sys_init.sql; -- 替换为实际文件路径
   ```
3. 配置系统数据库连接：启动程序后，在「系统管理 → 数据库配置」中填写 MySQL 地址、端口、用户名、密码、库名，测试连接成功后保存。

## 编译与运行
1. 克隆项目到本地：
   ```bash
   git clone https://github.com/6hz-t/QtStuSys.git
   cd QtStuSys
   ```
2. 用 Qt Creator 打开项目文件 `QtStuSys.pro`，选择构建套件（MinGW 64-bit）。
3. （可选）若需 XLSX 功能：
   - 下载 `QXlsx` 库，解压后将 `QXlsx` 文件夹复制到项目根目录。
   - 在 `QtStuSys.pro` 中添加配置：
     ```pro
     include(QXlsx/QXlsx.pri)
     ```
4. 点击「构建」按钮，编译完成后，在 `build-QtStuSys-Desktop_Qt_6_10_0_MinGW_64_bit-Release` 目录下找到可执行文件 `QtStuSys.exe`，双击启动。

## 使用说明
### 1. 数据导入/导出（XLSX）
#### 导出示例
- 学生管理页面：勾选需导出的学生（或全选），点击「导出」按钮，选择保存路径，生成 XLSX 文件（包含学生ID、姓名、性别、班级等字段）。
- 成绩管理页面：筛选目标课程/班级，点击「导出成绩」，生成包含学生信息+对应成绩的 XLSX 文件。

#### 导入示例
- 学生导入：下载项目根目录下的 `template/学生信息导入模板.xlsx`，按模板格式填写学生数据（必填字段：姓名、性别、班级、学号），点击「导入」按钮选择文件，系统自动校验数据并批量添加。
- 成绩导入：下载 `template/成绩导入模板.xlsx`，填写学号、课程ID、成绩字段，导入后系统匹配学生和课程并更新成绩。

### 2. 关键操作截图
#### （1）学生管理页面
![学生管理页面](https://github.com/6hz-t/QtStuSys/blob/main/screenshots/student_manage.png?raw=true)  
*支持分页、搜索、批量操作及 XLSX 导入导出*

#### （2）成绩录入页面
![成绩录入页面](https://github.com/6hz-t/QtStuSys/blob/main/screenshots/score_input.png?raw=true)  
*按课程批量录入成绩，支持导入/导出*

#### （3）数据库配置页面
![数据库配置页面](https://github.com/6hz-t/QtStuSys/blob/main/screenshots/db_config.png?raw=true)  
*配置 MySQL 连接信息，支持测试连接*

### 3. 模板文件
项目提供 XLSX 导入模板，位于 `template/` 目录：
- `学生信息导入模板.xlsx`：学生数据批量导入模板
- `成绩导入模板.xlsx`：成绩批量导入模板
- 模板字段不可随意修改，否则会导致导入失败

## 项目结构
```
QtStuSys/
├── src/                  # 源代码目录
│   ├── model/            # 数据模型（学生、课程、成绩等）
│   ├── view/             # 界面视图
│   ├── controller/       # 业务逻辑控制器
│   ├── utils/            # 工具类（XLSX处理、数据库连接等）
│   └── main.cpp          # 程序入口
├── sql/                  # 数据库脚本
│   └── qt_stu_sys_init.sql  # 数据表初始化脚本
├── template/             # XLSX 导入模板
│   ├── 学生信息导入模板.xlsx
│   └── 成绩导入模板.xlsx
├── screenshots/          # 系统截图（README使用）
├── styles/               # QSS 样式表
├── QtStuSys.pro          # 项目配置文件
└── README.md             # 项目说明文档
```

## 待完善功能
- [ ] 完整的用户认证与角色权限控制（管理员/教师/学生）
- [ ] 课程选课流程优化（学生自主选课、退课）
- [ ] 成绩统计分析功能（平均分、排名、图表展示）
- [ ] 导出文件格式扩展（CSV、PDF）

## 许可证
本项目采用 MIT 许可证开源，详见 [LICENSE](https://github.com/6hz-t/QtStuSys/blob/main/LICENSE) 文件。

## 致谢
- Qt 框架：提供强大的跨平台桌面应用开发能力
- MySQL：稳定可靠的关系型数据库支持
- QXlsx：轻量级 XLSX 文件处理库（可选集成）

---

### 说明
1. 截图路径：请将实际系统截图放入 `screenshots/` 目录，并更新 README 中的图片链接（建议使用 GitHub  raw 链接，格式：`https://github.com/6hz-t/QtStuSys/blob/main/目录/文件名.png?raw=true`）。
2. 模板文件：若项目未包含 `template/` 目录，需手动创建并添加对应 XLSX 模板，或在 README 中说明模板获取方式。
3. 依赖调整：若项目未使用 `QXlsx`，可删除相关配置说明，或替换为 Qt 自带的 `QAxObject`（仅 Windows 支持）处理 XLSX。