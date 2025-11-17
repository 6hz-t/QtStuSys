# 学生成绩管理系统数据库表结构说明


## 1. 学生表（students）
| 字段名      | 数据类型        | 约束条件                                               | 默认值                                        | 备注说明                    |
| ----------- | --------------- | ------------------------------------------------------ | --------------------------------------------- | --------------------------- |
| id          | INT             | PRIMARY KEY AUTO_INCREMENT                             | -                                             | 自增主键（开发环境用）      |
| student_id  | VARCHAR(20)     | NOT NULL, UNIQUE                                       | -                                             | 学号（唯一标识）            |
| name        | VARCHAR(20)     | NOT NULL                                               | -                                             | 姓名                        |
| gender      | ENUM('男','女') | NOT NULL                                               | -                                             | 性别（仅支持男/女）         |
| class_name  | VARCHAR(50)     | NOT NULL                                               | -                                             | 班级（如：2023级计算机1班） |
| birth_date  | DATE            | -                                                      | -                                             | 出生日期                    |
| phone       | VARCHAR(20)     | -                                                      | -                                             | 联系方式（手机号/固定电话） |
| create_by   | VARCHAR(20)     | NOT NULL                                               | 'dev'                                         | 创建人（默认dev）           |
| create_time | DATETIME        | NOT NULL                                               | CURRENT_TIMESTAMP                             | 创建时间（默认当前时间）    |
| update_by   | VARCHAR(20)     | NOT NULL                                               | 'dev'                                         | 更新人（默认dev）           |
| update_time | DATETIME        | NOT NULL                                               | CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP | 更新时间（自动更新）        |
| is_deleted  | TINYINT         | NOT NULL                                               | 0                                             | 是否删除（0=未删，1=已删）  |
| 索引        | -               | INDEX idx_class_name(class_name)、INDEX idx_name(name) | -                                             | 优化班级、姓名查询          |

## 2. 课程表（courses）
| 字段名       | 数据类型     | 约束条件                                                           | 默认值                                        | 备注说明                         |
| ------------ | ------------ | ------------------------------------------------------------------ | --------------------------------------------- | -------------------------------- |
| id           | INT          | PRIMARY KEY AUTO_INCREMENT                                         | -                                             | 自增主键（开发环境用）           |
| course_id    | VARCHAR(20)  | NOT NULL, UNIQUE                                                   | -                                             | 课程号（唯一标识）               |
| course_name  | VARCHAR(100) | NOT NULL                                                           | -                                             | 课程名                           |
| credit       | DECIMAL(3,1) | NOT NULL                                                           | -                                             | 学分（如：2.0、3.5）             |
| teacher_name | VARCHAR(50)  | NOT NULL                                                           | -                                             | 授课教师                         |
| semester     | VARCHAR(20)  | NOT NULL                                                           | -                                             | 学期（如：2023-2024学年第1学期） |
| create_by    | VARCHAR(20)  | NOT NULL                                                           | 'dev'                                         | 创建人（默认dev）                |
| create_time  | DATETIME     | NOT NULL                                                           | CURRENT_TIMESTAMP                             | 创建时间（默认当前时间）         |
| update_by    | VARCHAR(20)  | NOT NULL                                                           | 'dev'                                         | 更新人（默认dev）                |
| update_time  | DATETIME     | NOT NULL                                                           | CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP | 更新时间（自动更新）             |
| is_deleted   | TINYINT      | NOT NULL                                                           | 0                                             | 是否删除（0=未删，1=已删）       |
| 索引         | -            | INDEX idx_teacher_name(teacher_name)、INDEX idx_semester(semester) | -                                             | 优化教师、学期查询               |

## 3. 选课中间表（student_courses）
| 字段名      | 数据类型    | 约束条件                                                                                    | 默认值            | 备注说明                   |
| ----------- | ----------- | ------------------------------------------------------------------------------------------- | ----------------- | -------------------------- |
| id          | INT         | PRIMARY KEY AUTO_INCREMENT                                                                  | -                 | 自增主键                   |
| student_id  | VARCHAR(20) | NOT NULL                                                                                    | -                 | 学号（关联学生表）         |
| course_id   | VARCHAR(20) | NOT NULL                                                                                    | -                 | 课程号（关联课程表）       |
| select_time | DATETIME    | NOT NULL                                                                                    | CURRENT_TIMESTAMP | 选课时间（默认当前时间）   |
| create_by   | VARCHAR(20) | NOT NULL                                                                                    | 'dev'             | 创建人（默认dev）          |
| is_deleted  | TINYINT     | NOT NULL                                                                                    | 0                 | 是否删除（0=未删，1=已删） |
| 约束        | -           | UNIQUE KEY uk_student_course(student_id,course_id)                                          | -                 | 一个学生只能选同一门课一次 |
| 外键关联    | -           | FOREIGN KEY(student_id) REFERENCES students(student_id) ON DELETE CASCADE ON UPDATE CASCADE | -                 | 关联学生表，同步删除/更新  |
| 外键关联    | -           | FOREIGN KEY(course_id) REFERENCES courses(course_id) ON DELETE CASCADE ON UPDATE CASCADE    | -                 | 关联课程表，同步删除/更新  |
| 索引        | -           | INDEX idx_student_id(student_id)、INDEX idx_course_id(course_id)                            | -                 | 优化关联查询               |

## 4. 成绩表（scores）
| 字段名       | 数据类型                   | 约束条件                                                                                                             | 默认值                                        | 备注说明                          |
| ------------ | -------------------------- | -------------------------------------------------------------------------------------------------------------------- | --------------------------------------------- | --------------------------------- |
| id           | INT                        | PRIMARY KEY AUTO_INCREMENT                                                                                           | -                                             | 自增主键                          |
| student_id   | VARCHAR(20)                | NOT NULL                                                                                                             | -                                             | 学号（关联学生表）                |
| course_id    | VARCHAR(20)                | NOT NULL                                                                                                             | -                                             | 课程号（关联课程表）              |
| exam_type    | ENUM('期中','期末','总评') | NOT NULL                                                                                                             | -                                             | 考试类型                          |
| score        | DECIMAL(5,1)               | NOT NULL                                                                                                             | -                                             | 成绩（如：85.5、92.0；支持0-100） |
| score_status | TINYINT                    | NOT NULL                                                                                                             | 1                                             | 成绩状态（1=有效，0=作废）        |
| remark       | VARCHAR(200)               | -                                                                                                                    | -                                             | 备注（如：成绩修改说明）          |
| create_by    | VARCHAR(20)                | NOT NULL                                                                                                             | 'dev'                                         | 创建人（默认dev）                 |
| create_time  | DATETIME                   | NOT NULL                                                                                                             | CURRENT_TIMESTAMP                             | 创建时间（默认当前时间）          |
| update_by    | VARCHAR(20)                | NOT NULL                                                                                                             | 'dev'                                         | 更新人（默认dev）                 |
| update_time  | DATETIME                   | NOT NULL                                                                                                             | CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP | 更新时间（自动更新）              |
| 约束         | -                          | UNIQUE KEY uk_student_course_exam(student_id,course_id,exam_type)                                                    | -                                             | 一人一课一考试类型只能有一个成绩  |
| 外键关联     | -                          | FOREIGN KEY(student_id) REFERENCES students(student_id) ON DELETE CASCADE ON UPDATE CASCADE                          | -                                             | 关联学生表，同步删除/更新         |
| 外键关联     | -                          | FOREIGN KEY(course_id) REFERENCES courses(course_id) ON DELETE CASCADE ON UPDATE CASCADE                             | -                                             | 关联课程表，同步删除/更新         |
| 索引         | -                          | INDEX idx_student_course(student_id,course_id)、INDEX idx_exam_type(exam_type)、INDEX idx_score_status(score_status) | -                                             | 优化成绩查询                      |

## 5. 成绩修改日志表（score_change_logs）
| 字段名        | 数据类型                   | 约束条件                                                                                                             | 默认值            | 备注说明                  |
| ------------- | -------------------------- | -------------------------------------------------------------------------------------------------------------------- | ----------------- | ------------------------- |
| id            | INT                        | PRIMARY KEY AUTO_INCREMENT                                                                                           | -                 | 自增主键                  |
| score_id      | INT                        | NOT NULL                                                                                                             | -                 | 成绩表主键（关联成绩表）  |
| student_id    | VARCHAR(20)                | NOT NULL                                                                                                             | -                 | 学号                      |
| course_id     | VARCHAR(20)                | NOT NULL                                                                                                             | -                 | 课程号                    |
| old_score     | DECIMAL(5,1)               | NOT NULL                                                                                                             | -                 | 修改前成绩                |
| new_score     | DECIMAL(5,1)               | NOT NULL                                                                                                             | -                 | 修改后成绩                |
| exam_type     | ENUM('期中','期末','总评') | NOT NULL                                                                                                             | -                 | 考试类型                  |
| change_reason | VARCHAR(200)               | -                                                                                                                    | -                 | 修改原因                  |
| operated_by   | VARCHAR(20)                | NOT NULL                                                                                                             | 'dev'             | 操作人（默认dev）         |
| operated_time | DATETIME                   | NOT NULL                                                                                                             | CURRENT_TIMESTAMP | 操作时间（默认当前时间）  |
| 外键关联      | -                          | FOREIGN KEY(score_id) REFERENCES scores(id) ON DELETE CASCADE ON UPDATE CASCADE                                      | -                 | 关联成绩表，同步删除/更新 |
| 索引          | -                          | INDEX idx_score_id(score_id)、INDEX idx_operated_time(operated_time)、INDEX idx_student_course(student_id,course_id) | -                 | 优化日志查询              |

## 6. 系统操作日志表（operation_logs）
| 字段名            | 数据类型                                               | 约束条件                                                                                                       | 默认值            | 备注说明                                 |
| ----------------- | ------------------------------------------------------ | -------------------------------------------------------------------------------------------------------------- | ----------------- | ---------------------------------------- |
| id                | INT                                                    | PRIMARY KEY AUTO_INCREMENT                                                                                     | -                 | 自增主键                                 |
| module_name       | VARCHAR(50)                                            | NOT NULL                                                                                                       | -                 | 操作模块（如：学生管理、成绩录入）       |
| operation_type    | ENUM('新增','修改','删除','导入','导出','作废','恢复') | NOT NULL                                                                                                       | -                 | 操作类型                                 |
| operation_content | VARCHAR(500)                                           | NOT NULL                                                                                                       | -                 | 操作内容（如：新增学生[学号：20230001]） |
| operated_by       | VARCHAR(20)                                            | NOT NULL                                                                                                       | 'dev'             | 操作人（默认dev）                        |
| operated_ip       | VARCHAR(50)                                            | -                                                                                                              | '127.0.0.1'       | 操作IP（默认本地IP，开发环境用）         |
| operated_time     | DATETIME                                               | NOT NULL                                                                                                       | CURRENT_TIMESTAMP | 操作时间（默认当前时间）                 |
| 索引              | -                                                      | INDEX idx_module_name(module_name)、INDEX idx_operated_by(operated_by)、INDEX idx_operated_time(operated_time) | -                 | 优化日志查询                             |

## 开发环境默认测试数据
| 表名            | 插入数据说明                                                                    |
| --------------- | ------------------------------------------------------------------------------- |
| students        | 20230001（张三，男，2023级计算机1班）、20230002（李四，女，2023级计算机1班）    |
| courses         | CS101（C++程序设计，3.0学分，王老师）、CS102（数据结构，2.5学分，李老师）       |
| student_courses | 20230001选CS101/CS102、20230002选CS101                                          |
| scores          | 20230001（CS101期末88.5）、20230001（CS102期末92.0）、20230002（CS101期末79.0） |
| operation_logs  | 新增学生（20230001）、新增成绩（20230001-CS101-88.5）                           |