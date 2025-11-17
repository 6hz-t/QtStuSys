-- ------------------------------
-- 1. 学生表（students）：存储学生基础信息
-- ------------------------------
CREATE TABLE IF NOT EXISTS students (
    id INT PRIMARY KEY AUTO_INCREMENT COMMENT '自增主键（开发环境用）',
    student_id VARCHAR(20) NOT NULL COMMENT '学号（唯一标识）',
    name VARCHAR(20) NOT NULL COMMENT '姓名',
    gender ENUM('男', '女') NOT NULL COMMENT '性别（仅支持男/女）',
    class_name VARCHAR(50) NOT NULL COMMENT '班级（如：2023级计算机1班）',
    birth_date DATE COMMENT '出生日期',
    phone VARCHAR(20) COMMENT '联系方式（手机号/固定电话）',
    create_by VARCHAR(20) NOT NULL DEFAULT 'dev' COMMENT '创建人（默认dev）',
    create_time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间（默认当前时间）',
    update_by VARCHAR(20) NOT NULL DEFAULT 'dev' COMMENT '更新人（默认dev）',
    update_time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间（自动更新）',
    is_deleted TINYINT NOT NULL DEFAULT 0 COMMENT '是否删除（0=未删，1=已删；默认未删）',
    -- 唯一约束：学号不能重复
    UNIQUE KEY uk_student_id (student_id),
    -- 索引优化：班级、姓名查询
    INDEX idx_class_name (class_name),
    INDEX idx_name (name)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='学生信息表';

-- ------------------------------
-- 2. 课程表（courses）：存储课程基础信息
-- ------------------------------
CREATE TABLE IF NOT EXISTS courses (
    id INT PRIMARY KEY AUTO_INCREMENT COMMENT '自增主键（开发环境用）',
    course_id VARCHAR(20) NOT NULL COMMENT '课程号（唯一标识）',
    course_name VARCHAR(100) NOT NULL COMMENT '课程名',
    credit DECIMAL(3,1) NOT NULL COMMENT '学分（如：2.0、3.5）',
    teacher_name VARCHAR(50) NOT NULL COMMENT '授课教师',
    semester VARCHAR(20) NOT NULL COMMENT '学期（如：2023-2024学年第1学期）',
    create_by VARCHAR(20) NOT NULL DEFAULT 'dev' COMMENT '创建人（默认dev）',
    create_time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间（默认当前时间）',
    update_by VARCHAR(20) NOT NULL DEFAULT 'dev' COMMENT '更新人（默认dev）',
    update_time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间（自动更新）',
    is_deleted TINYINT NOT NULL DEFAULT 0 COMMENT '是否删除（0=未删，1=已删；默认未删）',
    -- 唯一约束：课程号不能重复
    UNIQUE KEY uk_course_id (course_id),
    -- 索引优化：教师、学期查询
    INDEX idx_teacher_name (teacher_name),
    INDEX idx_semester (semester)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='课程信息表';

-- ------------------------------
-- 3. 选课中间表（student_courses）：学生-课程多对多关联
-- ------------------------------
CREATE TABLE IF NOT EXISTS student_courses (
    id INT PRIMARY KEY AUTO_INCREMENT COMMENT '自增主键',
    student_id VARCHAR(20) NOT NULL COMMENT '学号（关联学生表）',
    course_id VARCHAR(20) NOT NULL COMMENT '课程号（关联课程表）',
    select_time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '选课时间（默认当前时间）',
    create_by VARCHAR(20) NOT NULL DEFAULT 'dev' COMMENT '创建人（默认dev）',
    is_deleted TINYINT NOT NULL DEFAULT 0 COMMENT '是否删除（0=未删，1=已删；默认未删）',
    -- 唯一约束：一个学生只能选同一门课一次
    UNIQUE KEY uk_student_course (student_id, course_id),
    -- 外键关联：保证关联数据存在
    FOREIGN KEY (student_id) REFERENCES students (student_id) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (course_id) REFERENCES courses (course_id) ON DELETE CASCADE ON UPDATE CASCADE,
    -- 索引优化：关联查询
    INDEX idx_student_id (student_id),
    INDEX idx_course_id (course_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='学生选课关联表';

-- ------------------------------
-- 4. 成绩表（scores）：存储学生课程成绩（核心表）
-- ------------------------------
CREATE TABLE IF NOT EXISTS scores (
    id INT PRIMARY KEY AUTO_INCREMENT COMMENT '自增主键',
    student_id VARCHAR(20) NOT NULL COMMENT '学号（关联学生表）',
    course_id VARCHAR(20) NOT NULL COMMENT '课程号（关联课程表）',
    exam_type ENUM('期中', '期末', '总评') NOT NULL COMMENT '考试类型',
    score DECIMAL(5,1) NOT NULL COMMENT '成绩（如：85.5、92.0；支持0-100）',
    score_status TINYINT NOT NULL DEFAULT 1 COMMENT '成绩状态（1=有效，0=作废；默认有效）',
    remark VARCHAR(200) COMMENT '备注（如：成绩修改说明）',
    create_by VARCHAR(20) NOT NULL DEFAULT 'dev' COMMENT '创建人（默认dev）',
    create_time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间（默认当前时间）',
    update_by VARCHAR(20) NOT NULL DEFAULT 'dev' COMMENT '更新人（默认dev）',
    update_time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间（自动更新）',
    -- 唯一约束：一人一课一考试类型只能有一个成绩
    UNIQUE KEY uk_student_course_exam (student_id, course_id, exam_type),
    -- 外键关联：保证学生和课程存在
    FOREIGN KEY (student_id) REFERENCES students (student_id) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (course_id) REFERENCES courses (course_id) ON DELETE CASCADE ON UPDATE CASCADE,
    -- 索引优化：成绩查询
    INDEX idx_student_course (student_id, course_id),
    INDEX idx_exam_type (exam_type),
    INDEX idx_score_status (score_status)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='学生成绩表';

-- ------------------------------
-- 5. 成绩修改日志表（score_change_logs）：记录成绩修改历史
-- ------------------------------
CREATE TABLE IF NOT EXISTS score_change_logs (
    id INT PRIMARY KEY AUTO_INCREMENT COMMENT '自增主键',
    score_id INT NOT NULL COMMENT '成绩表主键（关联成绩表）',
    student_id VARCHAR(20) NOT NULL COMMENT '学号',
    course_id VARCHAR(20) NOT NULL COMMENT '课程号',
    old_score DECIMAL(5,1) NOT NULL COMMENT '修改前成绩',
    new_score DECIMAL(5,1) NOT NULL COMMENT '修改后成绩',
    exam_type ENUM('期中', '期末', '总评') NOT NULL COMMENT '考试类型',
    change_reason VARCHAR(200) COMMENT '修改原因',
    operated_by VARCHAR(20) NOT NULL DEFAULT 'dev' COMMENT '操作人（默认dev）',
    operated_time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '操作时间（默认当前时间）',
    -- 外键关联：关联成绩表
    FOREIGN KEY (score_id) REFERENCES scores (id) ON DELETE CASCADE ON UPDATE CASCADE,
    -- 索引优化：日志查询
    INDEX idx_score_id (score_id),
    INDEX idx_operated_time (operated_time),
    INDEX idx_student_course (student_id, course_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='成绩修改日志表';

-- ------------------------------
-- 6. 系统操作日志表（operation_logs）：记录系统核心操作
-- ------------------------------
CREATE TABLE IF NOT EXISTS operation_logs (
    id INT PRIMARY KEY AUTO_INCREMENT COMMENT '自增主键',
    module_name VARCHAR(50) NOT NULL COMMENT '操作模块（如：学生管理、成绩录入）',
    operation_type ENUM('新增', '修改', '删除', '导入', '导出', '作废', '恢复') NOT NULL COMMENT '操作类型',
    operation_content VARCHAR(500) NOT NULL COMMENT '操作内容（如：新增学生[学号：20230001]）',
    operated_by VARCHAR(20) NOT NULL DEFAULT 'dev' COMMENT '操作人（默认dev）',
    operated_ip VARCHAR(50) DEFAULT '127.0.0.1' COMMENT '操作IP（默认本地IP，开发环境用）',
    operated_time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '操作时间（默认当前时间）',
    -- 索引优化：日志查询
    INDEX idx_module_name (module_name),
    INDEX idx_operated_by (operated_by),
    INDEX idx_operated_time (operated_time)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='系统操作日志表';

-- ------------------------------
-- 开发环境默认数据（可选）：添加测试数据方便开发
-- ------------------------------
-- 插入测试学生
INSERT INTO students (student_id, name, gender, class_name, birth_date, phone)
VALUES
('20230001', '张三', '男', '2023级计算机1班', '2005-09-15', '13800138000'),
('20230002', '李四', '女', '2023级计算机1班', '2006-03-22', '13900139000');

-- 插入测试课程
INSERT INTO courses (course_id, course_name, credit, teacher_name, semester)
VALUES
('CS101', 'C++程序设计', 3.0, '王老师', '2023-2024学年第1学期'),
('CS102', '数据结构', 2.5, '李老师', '2023-2024学年第1学期');

-- 插入测试选课记录
INSERT INTO student_courses (student_id, course_id)
VALUES
('20230001', 'CS101'),
('20230001', 'CS102'),
('20230002', 'CS101');

-- 插入测试成绩
INSERT INTO scores (student_id, course_id, exam_type, score)
VALUES
('20230001', 'CS101', '期末', 88.5),
('20230001', 'CS102', '期末', 92.0),
('20230002', 'CS101', '期末', 79.0);

-- 插入测试操作日志
INSERT INTO operation_logs (module_name, operation_type, operation_content)
VALUES
('学生管理', '新增', '新增学生[学号：20230001，姓名：张三]'),
('成绩管理', '新增', '新增成绩[学号：20230001，课程：CS101，成绩：88.5]');
