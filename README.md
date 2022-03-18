# HealthBrick - 营养计算器

## 项目介绍

### 声明

本项目是我的应用软件课程设计作业，仅作为个人学习应用软件知识、锻炼代码能力的一项练习。

项目内容欢迎学习交流，但严禁未经作者允许私自用于商业用途。

项目解释权归作者所有

### 项目的灵感来源

作者本人有记录每日摄入营养的习惯，但使用Excel或现有的软件无法兼顾功能性和方便性，因此打算打造一个简单的轻量级营养计算软件。

## 软件介绍

### 简述

以日期为结算单位，记录每天的饮食，系统可自动计算出营养含量。每天生成营养报告，供使用者参考。

### 功能描述

#### 饮食记录

进行一次饮食记录，用户输入食物/饮料的名称、净含量、营养含量等，系统自动录入数据库存储。

另外，由于用户的饮食习惯通常具有粘性，因此支持预先记录常用的食品属性，在添加饮食记录时可直接从预设中选择。

#### 体重跟踪

用户可以在系统中录入体重，进行统计分析。

#### 营养目标

根据用户的体重目标制定营养目标，每天营养报告生成前后都会提醒用户是否达到营养目标、距离营养目标还差多少等。

### 开发平台

采用`Qt Desktop` 进行桌面应用程序主体的设计。

数据结构采用`.XML`格式文档保存。

## 备注

本项目是在作者另一个项目 [AnyWhere-VisibleItemsManagementSystem](https://github.com/Vorrad/AnyWhere-VisibleItemsManagementSystem) 的开发基础上进行的。

## 联系

对于任何使用问题或讨论，请联系作者邮箱 15217340356@163.com
